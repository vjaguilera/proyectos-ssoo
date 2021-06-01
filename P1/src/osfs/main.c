#include <stdio.h>
#include <stdbool.h>
#include "../structs/mbt.h"
#include "os_API.h"

// // https://stackoverflow.com/a/46648676
// char get_bits(char a, char no_of_bits) {
//     return a & ((no_of_bits << 1) - 1);
// }

// https://www.rapidtables.com/convert/number/ascii-hex-bin-dec-converter.html
int main(int argc, char **argv)
{
    // ./osfs /mnt/d/SSOO/simdiskfilled.bin
    char *nombre_disco = argv[1];
    printf("Nombre disco: %s\n", nombre_disco);

    int case_7 = 0;
    int case_1 = 0;
    
    int case_2 = 0;
    int case_3 = 0;
    int case_4 = 0;
    int case_5 = 0;
    int case_6 = 0;
    int case_8 = 0;
    int case_9 = 1;

    // int case_7 = 0;
    // int case_1 = 1;
    // ---------------------------- CASO 1 ----------------------------------

    if (case_1 == 1)
    {
        printf("estoy en el caso 1\n");
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
        printf("estoy en el caso 2\n");
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
        // os_bitmap(4);
        printf("[=] ----------------------------------\n");
        // os_bitmap(0);
        printf("[=] ----------------------------------\n");
    }

    // ---------------------------- CASO 3 ----------------------------------

    else if (case_3 == 1)
    {
        printf("estoy en el caso 3\n");
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
        printf("estoy en el caso 4\n");
        os_mount(nombre_disco, 67);
        printf("[=] ----------------------------------\n");
        os_bitmap(0);
        printf("[=] ----------------------------------\n");
    }

    // ---------------------------- CASO 5 ----------------------------------

    else if (case_5 == 1)
    {
        printf("estoy en el caso 5\n");
        os_mount(nombre_disco, 3);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        os_create_partition(3, 500);
        printf("[=] ----------------------------------\n");
        os_create_partition(67, 1000000);
        printf("[=] ----------------------------------\n");
        os_create_partition(67, 1000);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        // os_bitmap(0);
        printf("[=] ----------------------------------\n");
        // os_reset_mbt();
    }
    else if (case_6 == 1)
    {
        printf("estoy en el caso 6\n");
        os_mount(nombre_disco, 2);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        osFile* file_desc = os_open("test.txt", 'w');
        printf("[=] ----------------------------------\n");
        char* buffer_write =
            "Esta herramienta online se utiliza"
            " para contar caracteres de un texto"
            " y cantidad de palabras, calcula también"
            " la densidad de palabras claves quitando"
            " las stop words.\n";
        os_write(file_desc, buffer_write, 300); // Escribe el archivo en el disco.
        printf("Sali de os write\n\n");
    }
    else if (case_7 == 1)
    {
        printf("estoy en el caso 7\n");
        os_mount(nombre_disco, 2);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------BORRAR------------------------\n");
        os_rm("test.txt");
        printf("[=] ----------------------------------\n");
        osFile* file_desc = os_open("test.txt", 'w');
        if (file_desc == NULL) {
            return 0;
        }
        printf("[=] ----------------------------------\n");
        char* buffer_write =
            "Esta herramienta online se utiliza"
            " para contar caracteres de un texto"
            " y cantidad de palabras, calcula también"
            " la densidad de palabras claves quitando"
            " las stop words.\n";
        os_write(file_desc, buffer_write, 300); // Escribe el archivo en el disco.
        os_close(file_desc);  // revisar write_file_directory en directory.c
        os_ls();
        // Revisar porque la suma del identificador del data init es asi
        // linea 593 os_API
        // printf("Sali de os write\n\n");
        osFile* file_desc_2 = os_open("test.txt", 'r');
        char buffer[165];
        os_read(file_desc_2, buffer, 166); 
        // printf("Hola mundo\n");
        for (int i = 0; i < 166; i++) {
            printf("%c", buffer[i]);
        }
        printf("\n");
        os_close(file_desc_2);
    }
        else if (case_8 == 1)
    {
        printf("estoy en el caso 8\n");
        os_mount(nombre_disco, 2);
        printf("[=] ----------------------------------\n");
        os_mbt();
        printf("[=] ----------------------------------\n");
        // os_ls(); // Tira tamano 0 para los archivos que estan creados
        // os_exists("nene.txt"); // Funciona de pana
        os_bitmap(0);
    }
        else if (case_9 == 1)
    {
        os_mount(nombre_disco, 2);
        printf("[=] ----------------------------------\n");
        osFile* file_desc_3 = os_open("tesñhasdlkjsahdlkjsahdlkjsahdkljhaslkdhkt.txt", 'r');
        osFile* file_desc_2 = os_open("test.txt", 'r');
        if (file_desc_2 != NULL) {
            char buffer[165];
            os_read(file_desc_2, buffer, 166); 
            // printf("Hola mundo\n");
            for (int i = 0; i < 166; i++) {
                printf("%c", buffer[i]);
            }
            printf("\n");
            os_close(file_desc_2);
        }
    }
    return 0;
}