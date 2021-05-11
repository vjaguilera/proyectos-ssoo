#include "bitmap.h"
#include <stdlib.h>

BitMap* bitmap_init(int cantidad_bloques) {
    BitMap* bitmap = malloc(sizeof(BitMap));
    bitmap -> bloques = calloc(cantidad_bloques, sizeof(int));
    bitmap -> cantidad_bloques = cantidad_bloques;
    return bitmap;
}

void assign_bit_block(BitMap* bitmap, int bit, int indice) {
    bitmap -> bloques[indice] = bit;
};

void bitmap_clean(BitMap* bitmap) {
    free(bitmap -> bloques);
    free(bitmap);
};