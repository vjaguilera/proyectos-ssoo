#include "directory.h"
#include "../helpers/bitExtract.h"
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

void get_bits1(int num)
{
    int i = 0;
    unsigned int size = sizeof(int);
    unsigned int maxPow = 1 << (size * 8 - 1);
    for (; i < size * 8; ++i)
    {
        // print last bit and shift left.
        printf("%u", num & maxPow ? 1 : 0);
        num = num << 1;
    }
    printf("\n");
}

void set_directory_data(Directory *directory, char *diskname, unsigned int initial)
{
    FILE *file = NULL;
    char buffer[2048]; // array of bytes, not pointers-to-bytes  => 2KB

    file = fopen(diskname, "r");
    printf("Iniciar en byte %u %ld\n", initial, sizeof(initial));
    // get_bits1(initial);
    // printf("---\n");
    lseek(fileno(file), initial, SEEK_SET);
    printf("Posicion actual %ld\n", ftell(file));
    // fseek(file, 1, SEEK_SET);
    if (file != NULL)
    {
        // read up to sizeof(buffer) bytes
        fread(buffer, 1, sizeof(buffer), file);
    }

    int x = 64;
    printf("Primeras %d entradas directorio.\n", x);
    for (int i = 0; i < 32 * x; i += 32)
    {
        char name[28];
        int entrada = i / 32;
        printf("Entrada %d:\n", entrada);
        unsigned char validez = buffer[i];
        printf("\tPrimer byte: %d\n", validez);
        printf("Primer byte: %d\n", buffer[i + 1]);
        printf("Segundo byte: %d\n", buffer[i + 2]);
        printf("Tercer byte: %d\n", buffer[i + 3]);
        unsigned int primer_bloque_relativo = (buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]);
        primer_bloque_relativo = bitExtracted(primer_bloque_relativo, 17, 1);
        // printf("\tPrimer bloque relativo: %ld\n", primer_bloque_relativo);

        // PUEDE QUE ESTO ESTE RARO (guardar el nombre)
        // printf("%d\n", i);
        for (int j = 0; j < 28; j++)
        {
            // printf("%ld %c\n", (int)i + (int)4 + (int)j, buffer[i + 4 + j]);
            name[j] = buffer[i + 4 + j];
        }
        //printf("\n");
        // printf("-- %d\n", (unsigned int) buffer[i + 4 + 27]);
        // printf("--- %c\n", buffer[i + 4 + 27 + 4]);
        name[28] = '\0';
        //printf("\tNombre archivo %s\n", name);
        // Lo deje como 0 el absoluto mientras
        EntAr *ent_ar = entar_init(
            validez,
            primer_bloque_relativo,
            primer_bloque_relativo + directory->indentificador_bloque + directory->cantidad_bloques_bitmap,
            name);
        directory->entradas_archivos[entrada] = ent_ar;

        // CREAR INDICE
        // Extraer indice absoluto desde el directorio
        FILE *file = NULL;
        char buffer[5]; // array of bytes, to store 5 bytes of file size

        // Open disk to read bytes
        file = fopen(diskname, "r");
        // Utilizar identificador absoluto de EntAr asociado para comenzar lectura
        unsigned int initial = ent_ar->identificador_absoluto;
        printf("Iniciar en byte %u %ld\n", initial, sizeof(initial));
        // get_bits1(initial);
        // printf("---\n");
        fseek(file, initial, SEEK_SET);
        printf("Posicion actual %ld\n", ftell(file));
        // fseek(file, 1, SEEK_SET);
        if (file != NULL)
        {
            // Leer 5 bytes y guardarlos en el buffer
            fread(buffer, 1, 5, file);
        }
        fclose(file);

        // Convert buffer to unsigned int and assign it as tamano
        unsigned int tamano = ((buffer[i + 1] << 32) | (buffer[i + 2] << 24) | (buffer[i + 3] << 16) | (buffer[i + 4] << 8) | (buffer[i + 5]));
        tamano = bitExtracted(tamano, 40, 1);

        // Init Indice
        Indice *indice = indice_init(tamano, ent_ar->identificador_relativo, initial);

        // Assign Indice to EntAr
        assign_indice(ent_ar, indice);
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
