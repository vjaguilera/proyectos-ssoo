#pragma once
#include "ent_ar.h"

typedef struct directory {
    unsigned int indentificador_bloque;
    EntAr** entradas_archivos;
    int cantidad_archivos;
    int cantidad_bloques_bitmap;
} Directory;

// FUNCTIONS

Directory* directory_init(unsigned int indentificador_bloque, int cantidad_bloques_bitmap);

void set_directory_data(Directory* directory, char* diskname, unsigned int initial);

void directory_clean(Directory* directory);









