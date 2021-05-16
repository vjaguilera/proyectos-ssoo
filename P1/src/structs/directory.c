#include "directory.h"
#include "../helpers/bitExtract.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// FUNCTIONS

Directory* directory_init(unsigned int indentificador_bloque, int cantidad_bloques_bitmap) {
    Directory* directory = malloc(sizeof(Directory));
    directory -> indentificador_bloque = indentificador_bloque;
    directory -> cantidad_bloques_bitmap = cantidad_bloques_bitmap;
    directory -> cantidad_archivos = 0; // inicial
    directory -> entradas_archivos = malloc(sizeof(EntAr*) * 64);
    return directory;
};

void get_bits1(int num) {
    int i=0;
    unsigned int size = sizeof(int);
    unsigned int maxPow = 1<<(size*8-1);
    for(;i<size*8;++i){
        // print last bit and shift left.
        printf("%u",num&maxPow ? 1 : 0);
        num = num<<1;
    }
    printf("\n");
}

void set_directory_data(Directory* directory, char* diskname, unsigned int initial) {
    FILE *file = NULL;
    char buffer[2048];  // array of bytes, not pointers-to-bytes  => 1KB

    file = fopen(diskname, "rb");  
    printf("Iniciar en byte %u %ld\n", initial, sizeof(initial));
    // get_bits1(initial);
    // fseek(file, initial, SEEK_SET); 
    // printf("Posicion actual %ld\n", ftell(file));
    // fseek(file, 1, SEEK_SET); 

    if (file != NULL) {
        fread(buffer, 1, 1024, file); // avanzar 1024
        for (int i = 0; i < initial; i++) {
            // ir bloque a bloque
            fread(buffer, 1, 2048, file); // ir bloque a bloque
        }
        // leer directorio
        fread(buffer, 1, 2048, file);
    }
    fclose(file);  

    int x = 64;
    // printf("Primeras %d entradas directorio.\n", x);
    for (int i = 0; i < 32 * x; i += 32 ) {
        char name[28];
        int entrada = i / 32;
        // printf("Entrada %d:\n", entrada);
        unsigned char validez = buffer[i];
        // printf("\tPrimer byte: %d\n", validez);
        unsigned int primer_bloque_relativo = (buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]);
        primer_bloque_relativo = bitExtracted(primer_bloque_relativo, 17, 1);
        // printf("\tPrimer bloque relativo: %ld\n", primer_bloque_relativo);
        
        // PUEDE QUE ESTO ESTE RARO (guardar el nombre)
        // printf("%d\n", i);
        for (int j = 0; j < 28; j++) {
            // printf("%ld %c\n", (int)i + (int)4 + (int)j, buffer[i + 4 + j]);
            name[j] = buffer[i + 4 + j];
        }
        // printf("\n");
        // printf("-- %d\n", (unsigned int) buffer[i + 4 + 27]);
        // printf("--- %c\n", buffer[i + 4 + 27 + 4]);
        name[28] = '\0';
        // printf("\tNombre archivo %s\n", name);
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


