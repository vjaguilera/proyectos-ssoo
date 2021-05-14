#include "directory.h"
#include "../helpers/bitExtract.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// FUNCTIONS

Directory* directory_init(unsigned long int indentificador_bloque, int cantidad_bloques_bitmap) {
    Directory* directory = malloc(sizeof(Directory));
    directory -> indentificador_bloque = indentificador_bloque;
    directory -> cantidad_bloques_bitmap = cantidad_bloques_bitmap;
    directory -> cantidad_archivos = 0; // inicial
    directory -> entradas_archivos = malloc(sizeof(EntAr*) * 64);
    return directory;
};

void set_directory_data(Directory* directory, char* diskname, long int initial) {
    FILE *file = NULL;
    unsigned char buffer[2048];  // array of bytes, not pointers-to-bytes  => 1KB

    file = fopen(diskname, "r");  
    printf("Iniciar en byte %ld\n", initial);
    lseek(fileno(file), initial + (long) 0, SEEK_CUR); 
    // fseek(file, 1, SEEK_SET); 
    if (file != NULL) {
        // read up to sizeof(buffer) bytes
        fread(buffer, 1, sizeof(buffer), file);
    }

    int x = 37;
    // printf("Primeras %d entradas directorio.\n", x);
    for (int i = 35 * 32; i < 32 * x; i += 32 ) {
        printf("0: %c\n",buffer[i + 3]);
        printf("1: %c\n",buffer[i + 4]);
        printf("2: %c\n",buffer[i + 5]);
        printf("3: %c\n",buffer[i + 6]);
        printf("4: %c\n",buffer[i + 7]);
        printf("5: %c\n",buffer[i + 8]);
        printf("6: %c\n",buffer[i + 9]);
        printf("7: %c\n",buffer[i + 10]);
        printf("8: %c\n",buffer[i + 11]);
        char name[28];
        int entrada = i / 32;
        printf("Entrada %d:\n", entrada);
        unsigned char validez = buffer[i];
        printf("\tPrimer byte: %d\n", validez);
        unsigned long int primer_bloque_relativo = (buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]);
        primer_bloque_relativo = bitExtracted(primer_bloque_relativo, 21, 1);
        // printf("\tPrimer bloque relativo: %ld\n", primer_bloque_relativo);
        
        // PUEDE QUE ESTO ESTE RARO (guardar el nombre)
        for (int j = 0; j < 28; j++) {
            printf("%c", buffer[i + 3 + j]);
            name[j] = buffer[i + 3 + j];
        }
        printf("\n");
        name[28] = '\0';
        printf("Nombre archivo -> %s\n", name);
        // Lo deje como 0 el absoluto mientras
        EntAr* ent_ar = entar_init(
            validez,
            primer_bloque_relativo,
            primer_bloque_relativo + directory -> indentificador_bloque + directory ->cantidad_bloques_bitmap,
            name
        );
        directory -> entradas_archivos[entrada] = ent_ar;
    }
}


void directory_clean(Directory* directory) {
    for (int i = 0; i < directory -> cantidad_archivos; i++) {
        free(directory -> entradas_archivos[i]);
    }
    free(directory -> entradas_archivos);
    free(directory);
};


