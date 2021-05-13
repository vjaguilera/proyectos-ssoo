#include "mbt.h"
#include <stdio.h>
#include <stdlib.h>
#include "../helpers/bitExtract.h"


MBT* mbt_init() {
    MBT* mbt = malloc(sizeof(MBT));
    mbt -> lista_de_particiones = malloc(sizeof(EntDir*) * 128);
    mbt -> particiones_validas = calloc(sizeof(int), 128);
    return mbt;
};

void set_mbt_data(MBT* mbt, char* diskname) {
    // https://stackoverflow.com/a/28197753
    FILE *file = NULL;
    unsigned char buffer[1024];  // array of bytes, not pointers-to-bytes  => 1KB

    file = fopen(diskname, "rb"); 
    if (file != NULL) {
        // read up to sizeof(buffer) bytes
        fread(buffer, 1, sizeof(buffer), file);
    }

    int x = 128;
    // printf("Primeras %d entradas.\n", x);
    for (int i = 0; i < 8 * x; i += 8 ) {
        // printf("Entrada %d %d:\n", i / 8, i);
        unsigned char first = buffer[i];
        // printf("%d \n", buffer[i]);
        char validez = first >> 7;
        // printf("\tPrimer bit: %d\n", validez);
        unsigned char mask = (1 << 7) - 1;
        int seven = buffer[i] & mask;
        // printf("\tParticion: %d\n", seven);
        signed int primer_bloque = ((buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]));
        primer_bloque = bitExtracted(primer_bloque, 21, 4);
        // printf("\tPrimer bloque: %ld\n", primer_bloque);
        signed int cantidad_bloques = ((buffer[i + 5] << 16) | (buffer[i + 6] << 8) | buffer[i + 7]);
        cantidad_bloques = bitExtracted(cantidad_bloques, 17, 8);
        // printf("\tCantidad bloques: %ld\n", cantidad_bloques);
        if (validez) {
            EntDir* entdir = entdir_init(validez, seven, primer_bloque, cantidad_bloques);
            assign_lista_de_particiones(mbt, entdir, seven);
        }
    }
}

void assign_lista_de_particiones(MBT* mbt, EntDir* entdir, int k) {
    mbt -> lista_de_particiones[k] = entdir;
    mbt -> particiones_validas[k] = 1;
}

void mbt_clean(MBT* mbt) {
    for (int i = 0; i < 128; i++) {
        entdir_clean(mbt -> lista_de_particiones[i]);
    };
    free(mbt -> lista_de_particiones);
    free(mbt -> particiones_validas);
    free(mbt);
}











