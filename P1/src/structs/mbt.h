#pragma once
#include "ent_dir.h"

typedef struct mbt {
    EntDir** lista_de_particiones;
    int* particiones_validas;
} MBT;

MBT* mbt_init();

void set_mbt_data(MBT* mbt, char* diskname);

void assign_lista_de_particiones(MBT* mbt, EntDir* entdir, int k);

void mbt_clean(MBT* mbt);
