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
    set_bitmap();
    // Definir directorio y bitmaps
};


void os_bitmap(unsigned num) {
    // Revisar foro https://github.com/IIC2333/Foro-2021-1/discussions/211
    // Revisar mbt -> lista_de_particiones[PARTICION] -> lista_bitmaps
    // Revisar mbt -> lista_de_particiones[PARTICION] -> cantidad_bitmaps
    if (num >= mbt ->lista_de_particiones[PARTICION] ->cantidad_bitmaps){
        // El input debe ser menor a la cantidad de bloques de bitmaps de la particion 
        printf("Ingrese valor menor a %d", mbt ->lista_de_particiones[PARTICION] ->cantidad_bitmaps);
    }
    else if(num == 0){
        // Retorna toda la info del bitmap
        // Es posible que se pida el binario completo de todos los bloques de bitmap
        for (int i = 0; i < mbt -> lista_de_particiones[PARTICION] -> cantidad_bitmaps; i ++){
            int ocupado = 0;
            char* estado_bloque;
            int largo_bitmap = mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[i] -> cantidad_bloques;
            for (int j = 0; j < largo_bitmap; j++){
                ocupado +=  mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[i] -> bloques[j];
                sprintf(estado_bloque, "%d", mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[num] -> bloques[j]);
            }
            int desocupado = largo_bitmap - ocupado; 
            int value = (int)strtol(estado_bloque, NULL, 2);
            // convert integer to hex string
            char hexString[2048]; // QUE WEA PONEMOS AQUI -> Era [12] porque asumía 32bits
            sprintf(hexString, "%x", value);
            fprintf( stderr, "%x\n", hexString);
            printf("Bits ocupados: %i\nBits desocupados: %i\n", ocupado, desocupado);
        }
        // fprintf( stderr, "El bitmap es %p\n", "string format", 30);
    }
    else{
        // Retorna solo el estado actual
        // Se asume que lo que se quiere es la info del estado del bloque bitmap[num] de la particion actual
        // Y no el bit correspondiente a num
        int ocupado = 0;
        int largo_bitmap = mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[num] -> cantidad_bloques;
        char* estado_bloque;
        for (int j = 0; j < largo_bitmap; j++){
            ocupado +=  mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[num] -> bloques[j];
            sprintf(estado_bloque, "%d", mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[num] -> bloques[j]);
        }
        // estado_bloque se espera que termine como un char "0100101"

        int value = (int)strtol(estado_bloque, NULL, 2);
        // convert integer to hex string
        char hexString[2048]; // QUE WEA PONEMOS AQUI -> Era [12] porque asumía 32bits
        sprintf(hexString, "%x", value);
        fprintf( stderr, "%x\n", hexString);
        int desocupado =  largo_bitmap - ocupado; 
        printf("Bits ocupados: %i\nBits desocupados: %i\n", ocupado, desocupado);
    }

};

int os_exists(char* filename) {
    printf("Vamos a revisar los archivos y buscar el %s\n", filename);
    for (int i = 0; i < 64; i++) {
        if (directory -> entradas_archivos[i] -> nombre_archivo == filename) {
            printf("El archivo %s existe\n", directory -> entradas_archivos[i] -> nombre_archivo); 
            return 1;
        }
    }
    printf("El archivo %s no existe\n", filename);
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
           printf(" - %d - Bitmaps: %ld\n", mbt -> lista_de_particiones[i] -> identificador_particion, mbt -> lista_de_particiones[i] -> cantidad_bitmaps); 
        }
    }

};

void os_create_partition(int id, int size) {
    // Revisar foro si hay respuesta en https://github.com/IIC2333/Foro-2021-1/discussions/213

    // Revisar si el id está disponible

    // Revisar donde cabe el size

    // Crear el directorio para asignar la particion
    printf("Crear particion %d de tamaño %d.\n", id, size);
    char validez = "1";
    // EntDir* entdir_init(validez, id, unsigned long int identificador_directorio, unsigned long int cantidad_bloques_particion);

    // Se creó efectivamente 
    // k va a ser la primera poscionn de mbt -> particiones_validas[i] que sea 0
    // mbt -> lista_de_particiones[k] = entdir;
    // assign_lista_de_particiones(mbt, EntDir* entdir, int k)
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
    printf("\tSe cargó el mbt\n");
};

void set_directory() {
    Directory* _directory = directory_init(mbt -> lista_de_particiones[PARTICION] -> identificador_directorio, mbt -> lista_de_particiones[PARTICION] -> cantidad_bitmaps);
    set_directory_data(_directory, NOMBRE_DISCO, (long int)((_directory -> indentificador_bloque) * 2048 + 1024));
    directory = _directory;
    printf("\tSe cargó el directorio\n");
}

void set_bitmap() {
    unsigned long int cantidad_bloques_particion =  mbt -> lista_de_particiones[PARTICION] -> cantidad_bloques_particion;
    if (cantidad_bloques_particion < 16384) {
        set_bitmap_data(
            mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[0],
            NOMBRE_DISCO,
            mbt -> lista_de_particiones[PARTICION] -> identificador_directorio * 2048 + 2048 + 1024,
            cantidad_bloques_particion
        );
    } else {
        int cantidad_bloques = cantidad_bloques_particion;
        unsigned long int cantidad_bits_setear = 0;
        for (int i = 0; i < mbt -> lista_de_particiones[PARTICION] -> cantidad_bitmaps; i ++) {
            if (cantidad_bloques > 16384) {
                cantidad_bits_setear = 16384;
            } else {
                cantidad_bits_setear = cantidad_bloques;
            }
            set_bitmap_data(
                mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[i],
                NOMBRE_DISCO,
                mbt -> lista_de_particiones[PARTICION] -> identificador_directorio + 2048 + i * 2048 + 1024, // directorio + tamaño directorio + bitmaps leidos
                cantidad_bits_setear
            );
            cantidad_bloques -= 16384;
        }
    }
    printf("\tSe cargaron los bitmaps\n");
}