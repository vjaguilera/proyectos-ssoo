#include "bitmap.h"
#include "ent_dir.h"
#include <stdlib.h>

EntDir* entdir_init(char validez, int identificador_particion, unsigned long int identificador_directorio, unsigned long int cantidad_bloques_particion) {
    EntDir* entdir = malloc(sizeof(EntDir));
    entdir -> validez = (int) validez;
    entdir -> identificador_particion = identificador_particion;
    entdir -> identificador_directorio = identificador_directorio;
    entdir -> cantidad_bloques_particion = cantidad_bloques_particion;
    int cantidad_bitmaps = cantidad_bloques_particion / 16384; // 2kb = 2048 x 8 bytes
    entdir -> lista_de_bitmaps = malloc(sizeof(BitMap*) * cantidad_bitmaps);
    if (cantidad_bloques_particion < 16384) {
        entdir -> lista_de_bitmaps[0] = bitmap_init(cantidad_bloques_particion);    
    } else {
        int cantidad_bloques = 0;
        for (int i = 0; i < cantidad_bitmaps; i ++) {
            if (cantidad_bloques > cantidad_bloques_particion) {
                entdir -> lista_de_bitmaps[i] = bitmap_init(cantidad_bloques - cantidad_bloques_particion);
            } else {
                entdir -> lista_de_bitmaps[i] = bitmap_init(16384);
            }
            cantidad_bloques += 16384;
        }
    }
    return entdir;
};

void entdir_clean(EntDir* entdir) {
    free(entdir);
}