#include "mbt.h"
#include <stdlib.h>


MBT* mbt_init() {
    MBT* mbt = malloc(sizeof(MBT));
    mbt -> lista_de_particiones = malloc(sizeof(EntDir*) * 128);
    return mbt;
};

void assign_lista_de_particiones(MBT* mbt, EntDir* entdir, int k) {
    mbt -> lista_de_particiones[k] = entdir;
}

void mbt_clean(MBT* mbt) {
    for (int i = 0; i < 128; i++) {
        free(mbt -> lista_de_particiones[i]);
    };
    free(mbt -> lista_de_particiones);
    free(mbt);
}











