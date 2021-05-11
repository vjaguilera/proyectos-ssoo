#include "directory.h"
#include <stdlib.h>

// FUNCTIONS

Directory* directory_init(unsigned long int indentificador_bloque, int cantidad_archivos) {
    Directory* directory = malloc(sizeof(Directory));
    directory -> indentificador_bloque = indentificador_bloque;
    directory -> cantidad_archivos = cantidad_archivos;
    directory -> entradas_archivos = malloc(sizeof(EntAr*)*cantidad_archivos);
    return directory;
};

void assing_entradas_archivos(Directory* directory, EntAr** entradas_archivos) {
    directory -> entradas_archivos = entradas_archivos;
}

void directory_clean(Directory* directory) {
    for (int i = 0; i < directory -> cantidad_archivos; i++) {
        free(directory -> entradas_archivos[i]);
    }
    free(directory -> entradas_archivos);
    free(directory);
};


