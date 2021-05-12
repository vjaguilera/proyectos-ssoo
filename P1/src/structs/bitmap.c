#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>

BitMap* bitmap_init(int cantidad_bloques) {
    BitMap* bitmap = malloc(sizeof(BitMap));
    bitmap -> bloques = calloc(cantidad_bloques, sizeof(int));
    bitmap -> cantidad_bloques = cantidad_bloques;
    return bitmap;
}

void assign_bit_block(BitMap* bitmap, int bit, int indice) {
    // printf("%d", bit);
    bitmap -> bloques[indice] = bit;
};

void set_bitmap_data(BitMap* bitmap, char* diskname, int initial, unsigned long int bits) {
    FILE *file = NULL;
    unsigned char buffer[2048];  // array of bytes, not pointers-to-bytes  => 1KB

    file = fopen(diskname, "rb");  
    fseek(file, initial, SEEK_SET); 
    // fseek(file, 1, SEEK_SET); 
    if (file != NULL) {
        // read up to sizeof(buffer) bytes
        fread(buffer, 1, sizeof(buffer), file);
    }
    // printf("Setear %ld bits en bitmap de %d bits\n", bits, bitmap -> cantidad_bloques);
    int bytes_revisar = bits / 8;
    for (int i = 0; i < bytes_revisar; i += 1 ) {
        for (int j = 0; j < 8; j += 1 ) {
            assign_bit_block(bitmap, ((buffer[i] >> j) & 0x01), i * 8 + j);
        }
    }
    int diff = bits - bytes_revisar * 8;
    if (diff > 0) {
        for (int j = 0; j < diff; j += 1 ) {
            assign_bit_block(bitmap, ((buffer[bytes_revisar + 1] >> j) & 0x01), (bytes_revisar) * 8 + j);
        }
    }
    // printf("\n");
    
}

void bitmap_clean(BitMap* bitmap) {
    free(bitmap -> bloques);
    free(bitmap);
};