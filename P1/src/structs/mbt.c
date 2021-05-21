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

    file = fopen(diskname, "r"); 
    if (file != NULL) {
        // read up to sizeof(buffer) bytes
        fread(buffer, 1, sizeof(buffer), file);
    }
    fclose(file);

    // 48  -> 00110000
    // 103 -> 01100111
    // 102 -> 01100110
    // 1.075.046 -> 00110000 01100111 01100110

    // 00110000 01100111 01100110
    //    10000 01100111 01100110 -> 1.075.046 -> usar (1)
    //    XXX10 00001100 11101100 -> 396.524 -> usar (4) 21 bits
    // 00110000 01100111 01100110 -> 3.172.198 pasa el limite de 2.097.152

    // NO USAR    01100 00011001 11011001 -> 793.049 -> usar (3) 22 bits
    // NO USAR    11000 00110011 10110011 -> 1.586.099 -> usar (2) 23 bits

    // bitExtracted(primer_bloque, 21, 1) -> 1.075.046
    // bitExtracted(primer_bloque, 21, 2) -> 1.586.099 = 110000011001110110011
    // bitExtracted(primer_bloque, 21, 3) -> 793.049   = 11000001100111011001
    // bitExtracted(primer_bloque, 21, 4) -> 396.524   = 01100000110011101100


    // 111 -> 01101111
    // 108 -> 01101100
    // 100 -> 01100100
    // 101 -> 01100101
    // 1869374565 = 01101111 01101100 01100100 01100101

    // 61  -> 00111101
    // 121 -> 01111001
    // 109 -> 01101101
    // 1931629 -> 000 - 11101 01111001 01101101


    int x = 128;
    // printf("Primeras %d entradas.\n", x);
    for (int i = 0; i < 8 * x; i += 8 ) {

        // TODO CORTADO DE DER A IZQ
        
        // printf("Entrada %d:\n", i / 8);
        unsigned char first = buffer[i];
        char validez = first >> 7;
        // printf("\tPrimer bit: %d\n", validez);
        unsigned char mask = (1 << 7) - 1;
        int seven = buffer[i] & mask;
        // printf("\tParticion: %d\n", seven);
        // printf("%d %d %d\n", buffer[i + 1], buffer[i + 2], buffer[i + 3]);
        unsigned int primer_bloque = ((buffer[i + 1] << 16) | (buffer[i + 2] << 8) | (buffer[i + 3]));
        unsigned int primer_bloque2 = bitExtracted(primer_bloque, 21, 1); // der a izq
        if (primer_bloque2 != 0) {
            // printf("%d %d %d\n", buffer[i + 1], buffer[i + 2], buffer[i + 3]);
            // printf("%d %d\n", buffer[i], i / 8);
            printf("\tPrimer bit: %d\n", validez);
            printf("\tParticion: %d\n", seven);
            printf("\tPrimer bloque 1: %d\n", primer_bloque2);
        }
        // primer_bloque2 = bitExtracted(primer_bloque, 21, 4); // izq a derecha
        // if (primer_bloque2 != 0) {
            // printf("\tPrimer bloque 4: %d\n", primer_bloque2);
        // }
        // printf("\n");
        // unsigned int cantidad_bloquesr = ((buffer[i + 5] << 16) | (buffer[i + 6] << 8) | buffer[i + 7]);
        // unsigned int cantidad_bloquesl = ((buffer[i + 4] << 16) | (buffer[i + 5] << 8) | buffer[i + 6]);
        unsigned int cantidad_bloquesg = ((buffer[i + 4] << 24) | (buffer[i + 5] << 16) | (buffer[i + 6] << 8) | buffer[i + 7]);
        unsigned cantidad_bloques2 = bitExtracted(cantidad_bloquesg, 17, 1); // der a izq 131.072 limite
        if (cantidad_bloques2 != 0) {
            printf("\tCantidad bloques G 1: %d\n", cantidad_bloques2);
        }
        // cantidad_bloques2 = bitExtracted(cantidad_bloquesg, 17, 8); // izq a der 131.072 limite
        // if (cantidad_bloques2 != 0) {
            // printf("\tCantidad bloques G 8: %d\n", cantidad_bloques2);
        // }
        if (validez) {
            EntDir* entdir = entdir_init(validez, seven, primer_bloque2, cantidad_bloques2);
            assign_lista_de_particiones(mbt, entdir, seven);
        }
    }
    printf("\n");
}

void assign_lista_de_particiones(MBT* mbt, EntDir* entdir, int k) {
    if (mbt -> particiones_validas[k] == 0) {
        mbt -> lista_de_particiones[k] = entdir;
        mbt -> particiones_validas[k] = 1;
        // printf( "%d %d\n", k, entdir -> identificador_directorio);
    }
}

void mbt_clean(MBT* mbt) {
    for (int i = 0; i < 128; i++) {
        entdir_clean(mbt -> lista_de_particiones[i]);
    };
    free(mbt -> lista_de_particiones);
    free(mbt -> particiones_validas);
    free(mbt);
}











