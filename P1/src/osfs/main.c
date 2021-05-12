#include <stdio.h>
#include "os_API.h"

int main(int argc, char **argv)
{
    // ./osfs simdisk.bin
    char* nombre_disco = argv[1];
    printf("Nombre disco: %s\n", nombre_disco);

    // https://stackoverflow.com/a/28197753

    FILE *file = NULL;
    unsigned char buffer[1024];  // array of bytes, not pointers-to-bytes  => 1KB
    size_t bytesRead = 0;

    file = fopen(nombre_disco, "rb");   

    if (file != NULL) {
        // read up to sizeof(buffer) bytes
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
            // process bytesRead worth of data in buffer
        }
    }
    printf("%ld\n", sizeof buffer / sizeof(char));
    for (int i = 0; i < 1024; i++) {
        printf("%d\n", buffer[i]);    
    }

    // Definir el MBT

    // Montar
    os_mount(nombre_disco, 1);
    return 0;
}