#include "os_API.h"
#include <stdio.h>
#include "../helpers/sort.h"

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
            char hexString[32768]; // 32768 = 2048 * 8 * 8 / 4 
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
        char hexString[32768]; // 32768 = 2048 * 8 * 8 / 4 
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
    if (mbt->particiones_validas[id]){
        printf("El id ingresado ya existe, entregue uno valido");
        return;
    }
    
    unsigned long int* lista_id_particiones[128];
    int cant_particiones_validas = 0;
    int prox_part;
    int flag = 0;
    for (int i = 0; i < 128; i++){

        // Particiones validas
        if (mbt->particiones_validas[i]){
            cant_particiones_validas += 1;

            // crear arreglo de tuplas (id_dir, cantidad_bloques, i) ordenadas por id_dir menor a mayor 
            lista_id_particiones[i] = mbt->lista_de_particiones[i]->identificador_directorio;
            }
        else{
            if (flag == 0){
                prox_part = i;
                flag = 1;
            }

        }
    }
    selectionSort(lista_id_particiones, cant_particiones_validas);
    // recorrer la lista de id particiones
    // sumar al id_dir la cantidad de bloques, y el resultado restarlo al id_dir del siguiente.
    // caso borde -> ultimo bloque

    // caso borde de primer bloque que no parte en el bloque con id 0
    if (lista_id_particiones[0] != 0){
        if(lista_id_particiones[0] > size){
            EntDir* entdir = entdir_init("1", id, 0, size);
            assign_lista_de_particiones(mbt, entdir, id);
            printf("Crear particion %d de tamaño %d.\n", id, size);
            return;

        }
    }

    for (int j = 0; j < cant_particiones_validas; j++){
        for (int p = 0; p < 128; p++){
            // recorro todos las posibles particiones
            if (mbt->particiones_validas[p]){
                // revisar si la particion es valida para buscar el identificador
                unsigned long int posicion_total;
                if (mbt->lista_de_particiones[p]->identificador_directorio == lista_id_particiones[j]){
                    // aqui se suma el identificador con la cantidad de bloques
                    // posicion total es el identificador + la cantidad de bloques de la particion anterior al espacio candidato
                    posicion_total = mbt->lista_de_particiones[p]->identificador_directorio + mbt->lista_de_particiones[p]->cantidad_bloques_particion;
                    if (j == cant_particiones_validas - 1){
                        // si no entra aca, no cabe en ningun lado
                        if (posicion_total + size <= 2097151){ // ultimo bloque posible
                            EntDir* entdir = entdir_init("1", id, posicion_total, size);
                            assign_lista_de_particiones(mbt, entdir, id);
                            printf("Crear particion %d de tamaño %d.\n", id, size);
                            return;
                        }
                        else{
                            printf("No cabe en ningun lado");
                            return;
                        }

                    }
                    unsigned long int inicio_sig_part = lista_id_particiones[j+1]; // revisar caso borde del final
                    if (inicio_sig_part - posicion_total >= size){
                        // crear la nueva particion
                        EntDir* entdir = entdir_init("1", id, posicion_total, size);
                        assign_lista_de_particiones(mbt, entdir, id);
                        printf("Crear particion %d de tamaño %d.\n", id, size);
                        return;
                    }
                    }

            }

        }  
    }
    printf("No se pudo crear la particion, no cabe en ninguna parte");
    
};

void os_delete_partition(int id) {
    printf("Eliminar particion %d.\n", id);
    mbt -> particiones_validas[id] = 0;
    mbt -> lista_de_particiones[id] -> validez = 0;
};

void os_reset_mbt() {
    // Por cada particion válida del mbt
    // Se llama a os_delete_partition de esta, con el identificador de la particion dada
    for (int i = 0; i < 128; i++) {
        if(mbt -> lista_de_particiones[i]){
            os_delete_partition(mbt -> lista_de_particiones[i] -> identificador_particion);
        }
    };
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
    for (int i = 0; i < directory -> cantidad_archivos; i++){
        if (directory -> entradas_archivos[i]->nombre_archivo == filename){
            // Este es el archivo a eliminar
            // Los bloques que estaban siendo usados por el archivo deben quedar libres.
            directory->cantidad_archivos -= 1;
            directory->entradas_archivos[i] -> validez = 0;
            for (int j = 0; j < directory->entradas_archivos[i] -> indice -> cantidad_bloques; j++){\

                // El indice apunta a las mismas direcciones del bitmap
                // Ahi si se pasa, esta en el siguiente

                // id del bitmap va a ser indice -> iden relat + j - k * cantidad de bloques en un bitmap
                // Nos falta conocer el k, el bitmap que
                // mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[k] -> bloques[iden relat + j];
            }
            
            // entar_clean(directory -> entradas_archivos[i]);
        }
    }
    
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