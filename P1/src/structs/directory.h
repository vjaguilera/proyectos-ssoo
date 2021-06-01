#pragma once
#include "ent_ar.h"
#include "bitmap.h"

typedef struct directory {
    unsigned int indentificador_bloque;
    EntAr** entradas_archivos;
    int cantidad_archivos;
    int cantidad_bloques_bitmap;
} Directory;

// FUNCTIONS

Directory* directory_init(unsigned int indentificador_bloque, int cantidad_bloques_bitmap);

void write_file_directory(Directory *directory, EntAr *ent_ar);

void set_directory_data(Directory* directory, char* diskname, unsigned int initial);

void directory_clean(Directory* directory);

void write_bitmap(Directory* directory, BitMap* bitmap, int index_bitmap);

// void assign_ent_ar(EntAr* ent_ar, Indice* this_indice);








