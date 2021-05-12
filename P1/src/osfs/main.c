#include <stdio.h>
#include <stdbool.h>
#include "os_API.h"

// https://stackoverflow.com/a/46648676
char get_bits(char a, char no_of_bits) {
    return a & ((no_of_bits << 1) - 1);
}


int main(int argc, char **argv)
{
    // ./osfs D:/SSOO/simdiskfilled.bin
    char* nombre_disco = argv[1];
    printf("Nombre disco: %s\n", nombre_disco);

    // https://stackoverflow.com/a/28197753

    FILE *file = NULL;
    unsigned char buffer[1024];  // array of bytes, not pointers-to-bytes  => 1KB
    size_t bytesRead = 0;

    file = fopen(nombre_disco, "r");   

    if (file != NULL) {
        // read up to sizeof(buffer) bytes
        bytesRead = fread(buffer, 1, sizeof(buffer), file);
    }
    printf("asdsad %ld\n", bytesRead);
    printf("1 %d \n", buffer[0]);
    printf("2 %d \n", buffer[1]);
    printf("3 %d \n", buffer[2]);
    printf("4 %d \n", buffer[3]);
    printf("5 %d \n", buffer[4]);
    printf("6 %d \n", buffer[5]);
    printf("7 %d \n", buffer[6]);
    printf("8 %d \n", buffer[7]);


    // bytesRead = fread(&buffer, 1, sizeof(buffer), file);

    if (true) { // mostrar particiones
        int x = 2;
        printf("Primeras %d entradas.\n", x);
        for (int i = 0; i < 8 * x; i += 8 ) {
            printf("Entrada %d:\n", i / 8);
            unsigned char first = buffer[i];
            printf("%d \n", buffer[i]);
            printf("\tPrimer bit: %d\n",first >> 7);
            unsigned char mask = (1 << 7) - 1;
            unsigned char seven = buffer[i] & mask;
            printf("\tParticion: %d\n", seven);
            printf("1%d %d\n", buffer[i + 1], buffer[i + 1] & ((1 << 7) - 1));
            printf("2%d %d\n", buffer[i + 2], buffer[i + 2] & ((1 << 7) - 1));
            printf("3%d %d\n", buffer[i + 3], buffer[i + 3] & ((1 << 7) - 1));
            long int primer_bloque = buffer[i + 3] | (buffer[i + 2] << 8) | (buffer[i + 1] << 16);
            printf("\tPrimer bloque: %ld\n", primer_bloque);
            // long int cantidad_bloques = (buffer[i + 4] << 24) | (buffer[i + 5] << 16) | (buffer[i + 6] << 8) | buffer[i + 7];
            // printf("\tCantidad bloques: %ld\n", cantidad_bloques);

        }
    }

    // Definir el MBT

    // Montar
    os_mount(nombre_disco, 1);

    return 0;
}