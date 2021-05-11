#pragma once
#include "ent_ar.h"

typedef struct directory {
    unsigned long int indentificador_bloque;
    EntAr** entradas_archivos;
    int cantidad_archivos;
} Directory;

// FUNCTIONS

Directory* directory_init(unsigned long int indentificador_bloque, int cantidad_archivos);

void assing_entradas_archivos(Directory* directory, EntAr** entradas_archivos);

void directory_clean(Directory* directory);









