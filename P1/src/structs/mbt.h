#pragma once
#include "ent_dir.h"

typedef struct mbt {
    EntDir** lista_de_particiones;
} MBT;

MBT* mbt_init();

void assign_lista_de_particiones(MBT* mbt, EntDir* entdir, int k);

void mbt_clean(MBT* mbt);
