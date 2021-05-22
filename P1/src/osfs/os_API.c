#include "os_API.h"
#include "../helpers/writeBytes.h"
#include <stdio.h>

// GENERALES

char* NOMBRE_DISCO;
int PARTICION;
MBT* mbt;
Directory* directory;

void os_mount(char* diskname, int partition) {
    printf("Se ha montado la partición %d del disco %s.\n", partition, diskname);
    NOMBRE_DISCO = diskname;
    PARTICION = partition;
    set_disk_name(diskname); // para escritura
    set_mbt();
    if (mbt -> particiones_validas[partition] == 1) {
        set_directory();
        set_bitmap();
    }
    // char bytes_to_modify[8] = "\x81\x00\x00\xC8\x00\x00\x4E\x20";      
    // writeBytesMBT(0, bytes_to_modify, 8);
    // - edito el primer byte y le pongo un 129 -> particion válida de id 1
    // - directorio en 200 y tamaño particion 9000

    // unsigned char bytes_to_modify[13] = "\x01\x00\x03\xE8\x68\x6f\x6c\x61\x2e\x74\x78\x74\x00";
    // writeBytes(200, 32, bytes_to_modify, 13);
    // - edito el bloque con id 200 y escribo a partir
    // - del byte 32, es decir, la segunda entrada de archivos
    // - archivo valido, indice en bloque 1000 nombre hola.txt
};


void os_bitmap(unsigned num) {
    for (int i = 0; i < mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[num] -> cantidad_bloques; i++) {
        printf("%d", mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[num] -> bloques[i]);
    };
    printf("\n");
    // Revisar mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps
    // Revisar mbt -> lista_de_particiones[PARTICION] -> cantidad_bitmaps
};

int os_exists(char* filename) {
    return 0;
};

void os_ls() {
    if (directory != NULL) {
        printf("Archivos válidos: %d\n", directory -> cantidad_archivos);
        for (int i = 0; i < 64; i++) {
            if (directory -> entradas_archivos[i] -> validez == 1) {
                printf(" %d.- Indice: %d Nombre: ", i, directory -> entradas_archivos[i]->identificador_relativo); 
                for (int j = 0; j < 28; j ++) {
                    EntAr* entar =  directory -> entradas_archivos[i];
                    printf("%c", entar -> nombre_archivo[j]);
                }
                printf("\n");
            }
        }
    }
};

// Master Boot Table

void os_mbt() {
    printf("Particiones válidas:\n");
    for (int i = 0; i < 128; i++) {
        if (mbt -> particiones_validas[i] == 1) {
           printf(" - %d - Bitmaps: %d\n", mbt -> lista_de_particiones[i] -> identificador_particion, mbt -> lista_de_particiones[i] -> cantidad_bitmaps); 
        }
    }

};

void os_create_partition(int id, int size) {
    printf("Crear particion %d de tamaño %d.\n", id, size);
};

void os_delete_partition(int id) {
    printf("Eliminar particion %d.\n", id);
    if (mbt -> particiones_validas[id] == 1) {
        mbt -> particiones_validas[id] = 0;
        mbt -> lista_de_particiones[id] -> validez = 0;
    }
};

void os_reset_mbt() {
    
};

// Manejo archivos

osFile* os_open(char* filename, char mode) {
    
    // Indice* indice = indice_init(); .. COMPLETAR
    // osFile* osfile = osfile_init(mode, filename, indice);
    return 0; // osfile
};

int os_read(osFile* file_desc, void* buffer, int nbytes) {
    return 0;
};

int os_write(osFile* file_desc, void* buffer, int nbytes) {
    return 0;
};

int os_close(osFile* file_desc) {
    return 0;
};

int os_rm(char* filename) {
    return 0;
};

// HELPERS

void set_mbt() {
    MBT* _mbt = mbt_init();
    set_mbt_data(_mbt, NOMBRE_DISCO);
    mbt = _mbt;
    printf("\tSe cargó el mbt\n");
};

void get_bits2(unsigned int num) {
    int i=0;
    unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1<<(size*8-1);
    for(;i<size*8;++i){
        // print last bit and shift left.
        printf("%u",num&maxPow ? 1 : 0);
        num = num<<1;
    }
    printf("\n");
}

void set_directory() {
    Directory* _directory = directory_init(mbt -> lista_de_particiones[PARTICION] -> identificador_directorio, mbt -> lista_de_particiones[PARTICION] -> cantidad_bitmaps);
    printf("Bloque %d\n", _directory -> indentificador_bloque);
    // get_bits2(_directory -> indentificador_bloque);
    printf("byte del bloque a buscar %ld\n", ((long int) _directory -> indentificador_bloque) * 2048 + 1024); // * 2KB dir + 1KB mbt
    // get_bits2(_directory -> indentificador_bloque * 2048 + 1024);
    // get_bits3((long int) _directory -> indentificador_bloque);
    set_directory_data(_directory, NOMBRE_DISCO, (_directory -> indentificador_bloque));
    directory = _directory;
    printf("\tSe cargó el directorio\n");
}

void set_bitmap() {
    unsigned int cantidad_bloques_particion =  mbt -> lista_de_particiones[PARTICION] -> cantidad_bloques_particion;
    if (cantidad_bloques_particion < 16384) {
        set_bitmap_data(
            mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[0],
            NOMBRE_DISCO,
            mbt -> lista_de_particiones[PARTICION] -> identificador_directorio + 1,
            cantidad_bloques_particion
        );
    } else {
        int cantidad_bloques = cantidad_bloques_particion;
        unsigned int cantidad_bits_setear = 0;
        for (int i = 0; i < mbt -> lista_de_particiones[PARTICION] -> cantidad_bitmaps; i ++) {
            if (cantidad_bloques > 16384) {
                cantidad_bits_setear = 16384;
            } else {
                cantidad_bits_setear = cantidad_bloques;
            }
            set_bitmap_data(
                mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[i],
                NOMBRE_DISCO,
                mbt -> lista_de_particiones[PARTICION] -> identificador_directorio + i + 1, // directorio
                cantidad_bits_setear
            );
            cantidad_bloques -= 16384;
        }
    }
    printf("\tSe cargaron los bitmaps\n");
}