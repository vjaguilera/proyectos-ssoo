#pragma once
#include "bitmap.h"

typedef struct ent_dir {
    int validez; // es un bit 0 o 1
    int identificador_particion;
    unsigned long int identificador_directorio;
    unsigned long int cantidad_bloques_particion;
    BitMap** lista_de_bitmaps;
} EntDir;

EntDir* entdir_init(char validez,
                    int identificador_particion,
                    unsigned long int identificador_directorio,
                    unsigned long int cantidad_bloques_particion);


void entdir_clean(EntDir* entdir);