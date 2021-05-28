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
    char *nombre_disco = argv[1];
    printf("Nombre disco: %s\n", nombre_disco);


    int case_1 = 1;
    int case_2 = 0;
    int case_3 = 0;
    int case_4 = 0;
    int case_5 = 0;

    // ---------------------------- CASO 1 ----------------------------------

    if (case_1 == 1)
    {
        os_mount(nombre_disco, 2);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        os_ls();
        printf("[=] ----------------------------------\n");
        os_exists("nenepez.txt");
        printf("[=] ----------------------------------\n");
        os_exists("nene.txt");
        printf("[=] ----------------------------------\n");
        osFile *osfile = os_open("nene.txt", 'r');
        if (osfile == NULL)
        {
            printf("[=] FILE NOT FOUND ------------------------\n");
        }
        else
        {
            osfile_print(osfile);
            printf("[=] ----------------------------------\n");
            char buffer[20];
            os_read(osfile, buffer, 20);
            for (int i = 0; i < 20; i++) {
                printf("%c", buffer[i]);
            }
            printf("\n[=] ----------------------------------\n");
        }
    }


    // ---------------------------- CASO 2 ----------------------------------

    else if (case_2 == 1)
    {
        os_mount(nombre_disco, 3);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        os_delete_partition(3);
        printf("[=] ----------------------------------\n");
        os_delete_partition(4);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        os_bitmap(4);
        printf("[=] ----------------------------------\n");
        os_bitmap(0);
        printf("[=] ----------------------------------\n");
    }

    // ---------------------------- CASO 3 ----------------------------------

    else if (case_3 == 1)
    {
        os_mount(nombre_disco, 3);
        printf("[=] ----------------------------------\n");
        osFile* file_desc = os_open("test.txt", 'w');
        printf("[=] ----------------------------------\n");
        char* buffer_write =
            "Esta herramienta online se utiliza"
            " para contar caracteres de un texto"
            " y cantidad de palabras, calcula también"
            " la densidad de palabras claves quitando"
            " las stop words.";
        os_write(file_desc, buffer_write, 300); // Escribe el archivo en el disco.
        printf("[=] ----------------------------------\n");
        os_ls();
        printf("[=] ----------------------------------\n");
        os_close(file_desc); // Cierra el archivo. Ahora deberíaa estar actualizado en el disco.
        printf("[=] ----------------------------------\n");
        osFile* file_desc_2 = os_open("test.txt", 'r');
        printf("[=] ----------------------------------\n");
        char buffer[165];
        os_read(file_desc_2, buffer, 165);
        for (int i = 0; i < 20; i++) {
            printf("%c", buffer[i]);
        }
        printf("\n[=] ----------------------------------\n");
    }

    // ---------------------------- CASO 4 ----------------------------------    

    else if (case_4 == 1)
    {
        os_mount(nombre_disco, 67);
        printf("[=] ----------------------------------\n");
        os_bitmap(0);
        printf("[=] ----------------------------------\n");
    }

    // ---------------------------- CASO 5 ----------------------------------

    else if (case_5 == 1)
    {
        os_mount(nombre_disco, 3);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        os_create_partition(3, 500);
        printf("[=] ----------------------------------\n");
        os_create_partition(67, 100000000);
        printf("[=] ----------------------------------\n");
        os_create_partition(67, 1000);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        os_bitmap(0);
        printf("[=] ----------------------------------\n");
        os_reset_mbt();
    }

    return 0;
}