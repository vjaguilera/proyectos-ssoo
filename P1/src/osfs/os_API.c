#include "os_API.h"
#include "../helpers/writeBytes.h"
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
    if (directory != NULL) {
        printf("Archivos válidos: %d\n", directory -> cantidad_archivos);
        for (int i = 0; i < 64; i++) {
            if (directory -> entradas_archivos[i] -> validez == 1) {
                printf(" %d.- Indice: %d Nombre: ", i, directory -> entradas_archivos[i] -> identificador_relativo); 
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
    if (mbt -> particiones_validas[id] == 1) {
        mbt -> particiones_validas[id] = 0;
        mbt -> lista_de_particiones[id] -> validez = 0;
    }
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