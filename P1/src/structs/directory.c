#include "directory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// FUNCTIONS

Directory* directory_init(unsigned long int indentificador_bloque, int cantidad_bloques_bitmap) {
    Directory* directory = malloc(sizeof(Directory));
    directory -> indentificador_bloque = indentificador_bloque;
    directory -> cantidad_bloques_bitmap = cantidad_bloques_bitmap;
    directory -> cantidad_archivos = 0; // inicial
    directory -> entradas_archivos = malloc(sizeof(EntAr*) * 64);
    return directory;
};

void set_directory_data(Directory* directory, char* diskname) {
    FILE *file = NULL;
    unsigned char buffer[2048];  // array of bytes, not pointers-to-bytes  => 1KB

    file = fopen(diskname, "rb");  
    // printf("Identificador bloque %ld \n", directory -> indentificador_bloque);
    fseek(file, directory -> indentificador_bloque, SEEK_SET); 
    // fseek(file, 1, SEEK_SET); 
    if (file != NULL) {
        // read up to sizeof(buffer) bytes
        fread(buffer, 1, sizeof(buffer), file);
    }

    int x = 64;
    // printf("Primeras %d entradas directorio.\n", x);
    for (int i = 0; i < 32 * x; i += 32 ) {
        char name[28];
        int entrada = i / 32;
        // printf("Entrada %d:\n", entrada);
        char validez = buffer[i];
        // printf("\tPrimer byte: %d\n", validez);
        unsigned long int primer_bloque_relativo = (buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]);
        // printf("\tPrimer bloque relativo: %ld\n", primer_bloque_relativo);
        
        // PUEDE QUE ESTO ESTE RARO (guardar el nombre)
        for (int j = 0; j < 28; j++) {
            name[j] = buffer[i + 4 + j];
        }
        name[20] = '\0';
        // printf("Nombre archivo %s\n", name);
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


