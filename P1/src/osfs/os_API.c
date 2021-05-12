#include "os_API.h"
#include <stdio.h>

// GENERALES

volatile char* NOMBRE_DISCO;
volatile int PARTICION;

void os_mount(char* diskname, int partition) {
    NOMBRE_DISCO = diskname;
    PARTICION = partition;
    // Definir directorio y bitmaps
};

void os_bitmap(unsigned num) {

};

int os_exists(char* filename) {
    return 0;
};

void os_ls() {

};

// Master Boot Table

void os_mbt() {

};

void os_create_partition(int id, int size) {

};

void os_delete_partition(int id) {

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

