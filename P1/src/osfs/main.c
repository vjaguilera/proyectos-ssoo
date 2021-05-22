#include <stdio.h>
#include <stdbool.h>
#include "../structs/mbt.h"
#include "os_API.h"

// // https://stackoverflow.com/a/46648676
// char get_bits(char a, char no_of_bits) {
//     return a & ((no_of_bits << 1) - 1);
// }


int main(int argc, char **argv)
{
    // ./osfs /mnt/d/SSOO/simdiskfilled.bin
    char* nombre_disco = argv[1];
    printf("Nombre disco: %s\n", nombre_disco);

    // printf("Directorio particion 8 = %ld\n", mbt -> lista_de_particiones[8] -> identificador_directorio);

    // Montar
    os_mount(nombre_disco, 1);
    printf("[=] ------------------------\n");
    os_mbt();
    printf("[=] ------------------------\n");
    os_ls();
    printf("[=] ------------------------\n");
    os_exists("hola.txt");
    printf("[=] ------------------------\n");
    os_bitmap(2);
    printf("[=] ------------------------\n");
    os_delete_partition(5); // No tiene archivos
    printf("[=] ------------------------\n");
    os_mbt();
    printf("[=] ------------------------\n");

    return 0;
}