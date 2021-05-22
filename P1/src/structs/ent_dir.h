#pragma once
#include "bitmap.h"

typedef struct ent_dir {
    int validez; // es un bit 0 o 1
    int identificador_particion;
    unsigned int identificador_directorio;
    unsigned int cantidad_bloques_particion;
    unsigned int cantidad_bitmaps;
    int entrada;
    BitMap** lista_de_bitmaps;
} EntDir;

EntDir* entdir_init(char validez,
                    int identificador_particion,
                    unsigned int identificador_directorio,
                    unsigned int cantidad_bloques_particion,
                    int entrada);


void entdir_clean(EntDir* entdir);