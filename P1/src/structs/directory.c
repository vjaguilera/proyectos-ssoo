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
    unsigned char buffer[2048];  // array of bytes, not pointers-to-bytes  => 1KB

    file = fopen(diskname, "r");  
    printf("Iniciar en byte %u %ld\n", initial, sizeof(initial));
    // get_bits1(initial);
    long int initial_2 = (long int) initial * 2048 + 1024;
    fseek(file, initial_2, SEEK_SET); 
    printf("Posicion actual %ld %ld\n", ftell(file), initial_2);
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

    if (file != NULL) {
        fread(buffer, 1, 2048, file);
    }

    fclose(file);  

    int x = 64;
    for (int i = 0; i < 32 * x; i += 32 ) {
        char name[28];
        int entrada = i / 32;
        unsigned char validez = buffer[i];
        unsigned int primer_bloque_relativo = (buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]);
        primer_bloque_relativo = bitExtracted(primer_bloque_relativo, 17, 1);
        
        for (int j = 0; j < 28; j++) {
            name[j] = buffer[i + 4 + j];
        }
        name[28] = '\0';
        
        EntAr* ent_ar = entar_init(
            validez,
            primer_bloque_relativo,
            primer_bloque_relativo + directory -> indentificador_bloque + directory ->cantidad_bloques_bitmap,
            name
        );
        directory -> entradas_archivos[entrada] = ent_ar;

        if (validez == 1) {
            printf("Entrada %d:\n", entrada);
            printf("\tPrimer byte: %d\n", validez);
            printf("\tPrimer bloque relativo: %d\n", primer_bloque_relativo);
            printf("\tNombre archivo %s\n", name);
            printf("\t%d %s %s\n", entrada, name, ent_ar -> nombre_archivo);
            directory -> cantidad_archivos += 1;
        }
    }
}


void directory_clean(Directory* directory) {
    for (int i = 0; i < directory -> cantidad_archivos; i++) {
        free(directory -> entradas_archivos[i]);
    }
    free(directory -> entradas_archivos);
    free(directory);
};


