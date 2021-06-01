#include "bitmap_read.h"

int get_pos_bitmap(Directory* directory, MBT* mbt, int partition)
{
    // for (int que_bitmap = 0; que_bitmap < directory -> cantidad_bloques_bitmap; que_bitmap++)
    // {
    int k = 0;
    for (int i = 0; i < mbt -> lista_de_particiones[partition] -> cantidad_bloques_particion;i++) {
        float aux_bitmap = i / 2048;
        int que_bitmap;
        que_bitmap = (int)aux_bitmap;
        int que_bloque = i - 2048 * que_bitmap;
        if (mbt -> lista_de_particiones[partition] -> lista_de_bitmaps[que_bitmap] -> bloques[que_bloque] == 0) {
            if (k == 1) {
                // para no volver a leerlo despues
                mbt -> lista_de_particiones[partition] -> lista_de_bitmaps[que_bitmap] -> bloques[que_bloque] = 1;
                return i;
            }
            k += 1;
        };
    }
    return 0;
    // }
}
// entrega la posicion relativa