#include "directory.h"
#include "../helpers/bitExtract.h"
#include "../helpers/writeBytes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// FUNCTIONS

Directory *directory_init(unsigned int indentificador_bloque, int cantidad_bloques_bitmap)
{
    Directory *directory = malloc(sizeof(Directory));
    directory->indentificador_bloque = indentificador_bloque;
    directory->cantidad_bloques_bitmap = cantidad_bloques_bitmap;
    directory->cantidad_archivos = 0; // inicial
    directory->entradas_archivos = malloc(sizeof(EntAr *) * 64);
    return directory;
};

void write_file_directory(Directory *directory, EntAr *ent_ar)
{
    // Supone que el MBT tiene una entrada particion con la entrada indicada de 0 a 127
    unsigned char bytes_array[32];
    unsigned int size;
    char *response;
    // Primer byte
    unsigned int nume = ent_ar->validez;
    unsigned char numero = (unsigned char)nume;
    bytes_array[0] = numero;

    // POSICION RELATIVA INDICE
    int j = ent_ar->identificador_relativo;
    size = 1;
    while (j > 255)
    {
        size += 1;
        j /= 255;
    }
    response = calloc(1, size);
    get_bits(response, ent_ar->identificador_relativo, 0);
    while (size < 3)
    {
        bytes_array[1 + 2 - size] = 0;
        size += 1;
    }
    int arraySize = strlen(response);
    int i = 0;
    char subset[8];
    while (i * 8 < arraySize)
    {
        for (int j = 0; j < 8; j++)
        {
            subset[j] = response[arraySize - (i + 1) * 8 + j];
        }
        i += 1;
        bytes_array[1 + size - i] = binarioADecimal(subset, 8);
        for (int i = 0; i < 8; i++)
        {
            subset[i] = 0;
        }
    }
    printf("-- %d %d %d %d \n", bytes_array[0], bytes_array[1], bytes_array[2], bytes_array[3]);

    // NOMBRE
    arraySize = 28;
    i = 0;
    for (i = 0; i < arraySize; i++)
    {
        bytes_array[4 + i] = ent_ar->nombre_archivo[i];
    }
    printf("-- %d %d %d %d \n", bytes_array[4], bytes_array[5], bytes_array[6], bytes_array[7]);
    printf("-- %c %c %c %c \n", bytes_array[4], bytes_array[5], bytes_array[6], bytes_array[7]);
    // writeBytes(directory->indentificador_bloque, ent_ar->entrada * 32, bytes_array, 32);
}

void set_directory_data(Directory *directory, char *diskname, unsigned int initial)
{
    FILE *file = NULL;
    unsigned char buffer[2048]; // array of bytes, not pointers-to-bytes  => 1KB
    unsigned char buffer_pointer[3]; // array of bytes

    file = fopen(diskname, "r");
    printf("[d] Directorio en bloque %u \n", initial);
    long int initial_2 = (long int)initial * 2048 + 1024;
    fseek(file, initial_2, SEEK_SET);
    // printf("Posicion actual %ld %ld\n", ftell(file), initial_2);
    // fseek(file, 1, SEEK_SET);

    // if (file != NULL) {
    //     fread(buffer, 1, 1024, file); // avanzar 1024
    //     for (int i = 0; i < initial; i++) {
    //         // ir bloque a bloque
    //         fread(buffer, 1, 2048, file); // ir bloque a bloque
    //     }
    //     // leer directorio
    //     fread(buffer, 1, 2048, file);
    // }

    if (file != NULL)
    {
        fread(buffer, 1, 2048, file);
    }
    fclose(file);

    int x = 64;
    for (int i = 0; i < 32 * x; i += 32)
    {
        char name[28];
        int entrada = i / 32;
        unsigned char validez = buffer[i];
        unsigned int primer_bloque_relativo = (buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]);
        primer_bloque_relativo = bitExtracted(primer_bloque_relativo, 17, 1);

        for (int j = 0; j < 28; j++)
        {
            name[j] = buffer[i + 4 + j];
        }
        name[28] = '\0';

        EntAr *ent_ar = entar_init(
            validez,
            primer_bloque_relativo,
            primer_bloque_relativo + directory->indentificador_bloque + directory->cantidad_bloques_bitmap,
            name,
            entrada);
        directory->entradas_archivos[entrada] = ent_ar;

        if (validez == 1)
        {
            printf("[d] Entrada %d:\n", entrada);
            printf("[d] \tPrimer byte: %d\n", validez);
            printf("[d] \tPrimer bloque relativo: %d\n", primer_bloque_relativo);
            printf("[d] \tNombre archivo %s\n", name);
            directory->cantidad_archivos += 1;

            // CREAR INDICE
            // Extraer indice absoluto desde el directorio
            FILE *file = NULL;
            char buffer[5]; // array of bytes, to store 5 bytes of file size

            // Open disk to read bytes
            file = fopen(diskname, "r");
            // Utilizar identificador absoluto de EntAr asociado para comenzar lectura
            unsigned int initial = ent_ar->identificador_absoluto;
            printf("[d] \tIndice en bloque %u\n", initial);
            // get_bits1(initial);
            // printf("---\n");
            fseek(file, initial, SEEK_SET);
            // fseek(file, 1, SEEK_SET);
            if (file != NULL)
            {
                // Leer 5 bytes y guardarlos en el buffer
                fread(buffer, 1, 5, file);
            }

            // Convert buffer to unsigned int and assign it as tamano
            unsigned long int tamano = (((unsigned long int)buffer[i + 1] << 32) | ((unsigned long int)buffer[i + 2] << 24) | ((unsigned long int)buffer[i + 3] << 16) | ((unsigned long int)buffer[i + 4] << 8) | ((unsigned long int)buffer[i + 5]));
            printf("[d] \tTamaño %lu\n", tamano);

            // Init Indice
            Indice *indice = indice_init(tamano, ent_ar->identificador_relativo, initial);

            // Assign pointers
            unsigned int pointer;
            for (int index = 0; index < 681; index++)
            {
                if (file != NULL)
                {
                    // Leer 3 bytes y guardarlos en el buffer
                    fread(buffer_pointer, 1, 3, file);
                    pointer = ((buffer_pointer[0] << 16) | (buffer_pointer[1] << 8) | (buffer_pointer[2]));
                    if (pointer != 0) {
                        printf("%d Pointer %d\n", index, pointer);
                    }
                    assign_pointer(indice, pointer, index);
                }
            }

            fclose(file);

            // Assign Indice to EntAr
            assign_indice(ent_ar, indice);

            write_indice(indice); //  ---> PARA GUARDAR INDICE
        }

        // write_file_directory(directory, ent_ar); ---> PARA GUARDAR EntAr
    }
}

void directory_clean(Directory *directory)
{
    for (int i = 0; i < directory->cantidad_archivos; i++)
    {
        free(directory->entradas_archivos[i]);
    }
    free(directory->entradas_archivos);
    free(directory);
};
