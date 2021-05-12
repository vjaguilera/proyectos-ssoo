#include "os_API.h"
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
    set_mbt();
    set_directory();
    // Definir directorio y bitmaps
};


void os_bitmap(unsigned num) {

};

int os_exists(char* filename) {
    return 0;
};

void os_ls() {
    printf("Archivos válidos:\n");
    for (int i = 0; i < 64; i++) {
        if (directory -> entradas_archivos[i] -> validez == 1) {
           printf(" - %s\n", directory -> entradas_archivos[i] -> nombre_archivo); 
        }
    }
};

// Master Boot Table

void os_mbt() {
    printf("Particiones válidas:\n");
    for (int i = 0; i < 128; i++) {
        if (mbt -> particiones_validas[i] == 1) {
           printf(" - %d\n", mbt -> lista_de_particiones[i] -> identificador_particion); 
        }
    }

};

void os_create_partition(int id, int size) {
    printf("Crear particion %d de tamaño %d.\n", id, size);
};

void os_delete_partition(int id) {
    printf("Eliminar particion %d.\n", id);
    mbt -> particiones_validas[id] = 0;
    mbt -> lista_de_particiones[id] -> validez = 0;
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
};

void set_directory() {
    Directory* _directory = directory_init(mbt -> lista_de_particiones[PARTICION] -> identificador_directorio);
    set_directory_data(_directory, NOMBRE_DISCO);
    directory = _directory;
}