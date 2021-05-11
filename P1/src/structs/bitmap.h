#pragma once


typedef struct bit_map {
    // cada bitmap alcanza para 2kb = 2048 * 8 = 16384 bits o bloques en este caso
    int cantidad_bloques;
    int* bloques;
} BitMap;

BitMap* bitmap_init(int cantidad_bloques);

void assign_bit_block(BitMap* bitmap, int bit, int indice);

void bitmap_clean(BitMap* bitmap);