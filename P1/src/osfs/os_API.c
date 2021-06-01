#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "os_API.h"
#include "../structs/directory.h"
#include "../structs/osFile.h"
#include "../helpers/sort.h"
#include "../helpers/writeBytes.h"
#include "../helpers/bitExtract.h"
#include "../helpers/errors.h"
#include <math.h>

// GENERALES

char *NOMBRE_DISCO;
int PARTICION;
int RESET;
MBT *mbt;
Directory *directory;

void os_mount(char *diskname, int partition)
{
    printf("Se ha montado la partición %d del disco %s.\n", partition, diskname);
    NOMBRE_DISCO = diskname;
    PARTICION = partition;
    RESET = 0;
    set_disk_name(diskname); // para escritura
    set_mbt();
    printf("[*] Se asigna partición %d\n", partition);
    if (mbt->particiones_validas[partition] == 1)
    {
        set_directory();
        set_bitmap();
    }
    else
    {
        OS_ERROR = EPERM;
        os_strerror(EPERM, "Partición inválida");
    }
    // char bytes_to_modify[8] = "\x81\x00\x00\xC8\x00\x00\x4E\x20";
    // writeBytesMBT(0, bytes_to_modify, 8);
    // - edito el primer byte y le pongo un 129 -> particion válida de id 1
    // - directorio en 200 y tamaño particion 20000

    // unsigned char bytes_to_modify[13] = "\x01\x00\x03\xE8\x68\x6f\x6c\x61\x2e\x74\x78\x74\x00";
    // writeBytes(200, 32, bytes_to_modify, 13);
    // - edito el bloque con id 200 y escribo a partir
    // - del byte 32, es decir, la segunda entrada de archivos
    // - archivo valido, indice en bloque relativo 1000 nombre hola.txt

    // unsigned char bytes_to_modify[8] = "\x00\x00\x00\x00\x1E\x00\x07\xD0";
    // writeBytes(1200, 0, bytes_to_modify, 8);
    // - Edito el tamaño del indice en 1202 y le pongo 30
    // - Puntero a 2000
    // - 1200 = 200 (inicial) + 1000 (relativo)

    // unsigned char bytes_to_modify[20] = "\x68\x6f\x6c\x61\x20\x6d\x65\x20\x6c\x6c\x61\x6d\x6f\x20\x63\x61\x72\x6c\x6f\x73";
    // writeBytes(2200, 0, bytes_to_modify, 20);
    // - Edito el bloque 2200 con 20 bytes de texto
    // - Hola me llamo carlos
    // - 2200 = 200 (inicial) + 2000 (relativo)
};

void os_bitmap(unsigned num)
{
    // Revisar foro https://github.com/IIC2333/Foro-2021-1/discussions/211
    printf("Número bitmap seleccionado %d\n", num);
    if (num >= mbt->lista_de_particiones[PARTICION]->cantidad_bitmaps + 1)
    {
        // El input debe ser menor a la cantidad de bloques de bitmaps de la particion
        OS_ERROR = EPERM;
        os_strerror(EPERM, "Bitmap inválido");
        printf("Ingrese valor menor o igual %d\n", mbt->lista_de_particiones[PARTICION]->cantidad_bitmaps);
    }
    else if (num == 0)
    {
        // Retorna toda la info del bitmap
        // Es posible que se pida el binario completo de todos los bloques de bitmap
        for (int i = 0; i < mbt->lista_de_particiones[PARTICION]->cantidad_bitmaps; i++)
        {
            printf("Bitmap n° %d\n", i + 1);
            int ocupado = 0;
            int largo_bitmap = mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[i]->cantidad_bloques;
            printf("El largo bitmap es %d\n", largo_bitmap); // Revisar largo bitmap n8   !!!!!!!!!!!!!!!!!!!!!
            char estado_bloque[largo_bitmap + 1]; 
            for (int j = 0; j < largo_bitmap; j++){
                ocupado += mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[i]->bloques[j];
                // sprintf(estado_bloque, "%d", mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[num] -> bloques[j]);
                if(mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[i]->bloques[j]){
                    estado_bloque[j] = *"1"; // Tata crack
                }
                else{
                    estado_bloque[j] = *"0"; // Tata crack
                }
            }
            int desocupado = largo_bitmap - ocupado;
            // int value = (int)strtol(estado_bloque, NULL, 2);
            // // convert integer to hex string
            // char hexString[32768]; // 32768 = 2048 * 8 * 8 / 4
            // char result[largo_bitmap + 1]; // where SIZE is big enough to hold any converted value
            long long int val;
            for (int i = 0; i < 512; i++){
                // lista[32*i: 32* i + 32]
                char estado_bloque_aux[32];
                for (int j = 0; j < 32; j++){
                    // printf("la posicion es %d\n", 32 * i + j);
                    estado_bloque_aux[j] = estado_bloque[32 * i + j];
                }
                val += (int) strtol(estado_bloque_aux, NULL, 0);
                // printf("El val actual es %d\n", val);
            }
            // printf("El val es %d\n", val);
            // sprintf(result, "%x", val);
            printf("Estado bloque: %s\n", estado_bloque);
            // printf("Value: %x\n", val);
            // sprintf(hexString, "%x", value);
            fprintf(stderr, "Estado bitmap en Hexa: %llx\n", val); 
            printf("Bits ocupados: %i\nBits desocupados: %i\n\n", ocupado, desocupado);
        }
        // fprintf( stderr, "El bitmap es %p\n", "string format", 30);
    }
    else{
        // Retorna solo el estado actual
        // Se asume que lo que se quiere es la info del estado del bloque bitmap[num] de la particion actual
        // Y no el bit correspondiente a num
        int ocupado = 0;
        num -= 1;
        int largo_bitmap = mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[num]->cantidad_bloques;
        char estado_bloque[largo_bitmap];
        for (int j = 0; j < largo_bitmap; j++)
        {
            ocupado += mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[num]->bloques[j];
            // sprintf(estado_bloque, "%d", mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[num] -> bloques[j]);
            // estado_bloque[0] = (char)mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[num]->bloques[j];
            if(mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[num]->bloques[j]){
                estado_bloque[j] = *"1"; // Tata crack
            }
            else{
                estado_bloque[j] = *"0"; // Tata crack
            }
        }
        // estado_bloque se espera que termine como un char "0100101"
        // char result[largo_bitmap + 1]; // where SIZE is big enough to hold any converted value
        long long int val;
        for (int i = 0; i < 512; i++){
            // lista[32*i: 32* i + 32]
            char estado_bloque_aux[32];
            for (int j = 0; j < 32; j++){
                // printf("la posicion es %d\n", 32 * i + j);
                estado_bloque_aux[j] = estado_bloque[32 * i + j];
            }
            // val += pow(2, 511-i) * (int) strtol(estado_bloque_aux, NULL, 0); // se esta elevando a la potencia correspondiente 
            val += (int) strtol(estado_bloque_aux, NULL, 0);
            // printf("El val actual es %d\n", val);
        }
        // sprintf(result, "%x", val);
        printf("Estado bloque: %s\n", estado_bloque);
        // printf("Value: %x\n", val); 
        // sprintf(hexString, "%x", value);
        fprintf(stderr, "Estado bitmap en Hexa: %llx\n", val); 
        int desocupado = largo_bitmap - ocupado;
        printf("Bits ocupados: %i\nBits desocupados: %i\n", ocupado, desocupado);
    }
};

int os_exists(char *filename)
{
    // printf("Vamos a revisar los archivos y buscar el %s\n", filename);
    for (int i = 0; i < 64; i++){
        if (directory->entradas_archivos[i]->validez == 1){
            int ex = 1;
            for (int j = 0; j < strlen(filename); j++){
                if (directory->entradas_archivos[i]->nombre_archivo[j] != filename[j]){
                    ex = 0;
                    break;
                }
            }
            if (ex == 1){
                printf("El archivo %s existe\n", filename);
                return 1;
            }
        }
    }
    OS_ERROR = ENOENT;
    os_strerror(ENOENT, filename);
    printf("El archivo %s NO existe os_exists\n", filename);
    return 0;
};

void os_ls(){
    if (directory != NULL){
        printf("Archivos válidos: %d\n", directory->cantidad_archivos);
        int val = 0;
        for (int i = 0; i < 64; i++){
            if (directory->entradas_archivos[i]->validez == 1){
                val += 1;
                printf(" %d.- Indice: %d | Tamaño: %d | Nombre: ", i, directory->entradas_archivos[i]->identificador_relativo, directory->entradas_archivos[i]->indice->tamano);
                printf("%s\n", directory->entradas_archivos[i]->nombre_archivo);
            }
        }
        if (val == 0) {
            OS_ERROR = ENOENT;
            os_strerror(ENOENT, "Directorio");
        }
    } else {
        OS_ERROR = EPERM;
        os_strerror(EPERM, "No hay directorio");
    }
};

// Master Boot Table

void os_mbt()
{
    int val = 0;
    printf("Particiones válidas:\n");
    for (int i = 0; i < 128; i++)
    {
        if (mbt->particiones_validas[i] == 1)
        {
            val += 1;
            printf(" - %d - Tienen %d bitmaps\n", mbt->lista_de_particiones[i]->identificador_particion, mbt->lista_de_particiones[i]->cantidad_bitmaps);
            // write_bitmap(directory, mbt->lista_de_particiones[i] -> lista_de_bitmaps[7], 7); // ---> PARA GUARDAR BitMap
        }
    }
    if (val == 0) {
        OS_ERROR = ENODATA;
        os_strerror(ENODATA, "");
    }
};

void os_create_partition(int id, int size)
{
    // Revisar foro si hay respuesta en https://github.com/IIC2333/Foro-2021-1/discussions/213

    // Revisar si el id está disponible
    if (mbt->particiones_validas[id])
    {
        OS_ERROR = EPERM;
        os_strerror(EPERM, "Id ingresado existe");
        printf("El id ingresado ya existe, entregue uno válido\n");
        return;
    }

    unsigned int *lista_id_particiones = malloc(sizeof(unsigned int) * 128);
    int cant_particiones_validas = 0;
    int flag = 0;
    for (int i = 0; i < 128; i++)
    {

        // Particiones validas
        if (mbt->particiones_validas[i])
        {
            cant_particiones_validas += 1;

            // crear arreglo de tuplas (id_dir, cantidad_bloques, i) ordenadas por id_dir menor a mayor
            lista_id_particiones[i] = mbt->lista_de_particiones[i]->identificador_directorio;
        }
        else
        {
            if (flag == 0)
            {
                // int prox_part = i;
                flag = 1;
            }
        }
    }
    selectionSort(lista_id_particiones, cant_particiones_validas);
    // recorrer la lista de id particiones
    // sumar al id_dir la cantidad de bloques, y el resultado restarlo al id_dir del siguiente.
    // caso borde -> ultimo bloque

    // caso borde de primer bloque que no parte en el bloque con id 0
    if (lista_id_particiones[0] != 0)
    {
        if (lista_id_particiones[0] > size)
        {
            EntDir *entdir = entdir_init('1', id, 0, size, 10); // ESE 10 DEBERIA SER EL NÚMERO DE LA ENTRADA DEL MBT ENTRE 0 Y 127
            assign_lista_de_particiones(mbt, entdir, id);
            printf("Crear particion %d de tamaño %d.\n", id, size);
            return;
        }
    }

    for (int j = 0; j < cant_particiones_validas; j++)
    {
        for (int p = 0; p < 128; p++)
        {
            // recorro todos las posibles particiones
            if (mbt->particiones_validas[p])
            {
                // revisar si la particion es valida para buscar el identificador
                unsigned long int posicion_total;
                if (mbt->lista_de_particiones[p]->identificador_directorio == lista_id_particiones[j])
                {
                    // aqui se suma el identificador con la cantidad de bloques
                    // posicion total es el identificador + la cantidad de bloques de la particion anterior al espacio candidato
                    posicion_total = mbt->lista_de_particiones[p]->identificador_directorio + mbt->lista_de_particiones[p]->cantidad_bloques_particion;
                    if (j == cant_particiones_validas - 1)
                    {
                        // si no entra aca, no cabe en ningun lado
                        if (posicion_total + size <= 2097151)
                        { // ultimo bloque posible
                            EntDir *entdir = entdir_init('1', id, posicion_total, size, 10);
                            assign_lista_de_particiones(mbt, entdir, id);
                            printf("Crear particion %d de tamaño %d.\n", id, size);
                            return;
                        }
                        else
                        {
                            OS_ERROR = ENOMEM;
                            os_strerror(ENOMEM, "");
                            printf("No cabe en ningun lado\n");
                            return;
                        }
                    }
                    unsigned long int inicio_sig_part = lista_id_particiones[j + 1]; // revisar caso borde del final
                    printf("%ld %ld %ld %d\n", inicio_sig_part, posicion_total, inicio_sig_part - posicion_total, inicio_sig_part - posicion_total > 0);
                    if (inicio_sig_part - posicion_total > 0 && inicio_sig_part - posicion_total >= size)
                    {
                        // crear la nueva particion
                        EntDir *entdir = entdir_init('1', id, posicion_total, size, 10);
                        assign_lista_de_particiones(mbt, entdir, id);
                        write_partition_mbt(entdir);
                        printf("Crear particion %d de tamaño %d.\n", id, size);
                        return;
                    }
                }
            }
        }
    }
    OS_ERROR = ENOMEM;
    os_strerror(ENOMEM, "");
    printf("No se pudo crear la particion, no cabe en ninguna parte\n");
};

void os_delete_partition(int id)
{
    printf("Eliminar particion %d.\n", id);
    if (RESET == 0 && PARTICION == id) {
        OS_ERROR = EPERM;
        os_strerror(EPERM, "Partición siendo utilizada");
        printf("Estás usando esta partición\n");
        return;
    }
    if (id < 0 || id > 127) {
        OS_ERROR = EPERM;
        os_strerror(EPERM, "Número partición no es válido");
        printf("Número no es válido: %d\n", id);
    }
    if (mbt->particiones_validas[id] == 1)
    {
        mbt->particiones_validas[id] = 0;
        mbt->lista_de_particiones[id]->validez = 0;
        write_partition_mbt(mbt->lista_de_particiones[id]);
    }
};

void os_reset_mbt()
{
    RESET = 1;
    // Por cada particion válida del mbt
    // Se llama a os_delete_partition de esta, con el identificador de la particion dada
    for (int i = 0; i < 128; i++)
    {
        if (mbt->lista_de_particiones[i])
        {
            os_delete_partition(mbt->lista_de_particiones[i]->identificador_particion);
        }
    };
    os_mbt();
};

// ====================
//      Manejo archivos
// ====================

osFile *os_open(char *filename, char mode)
/* Función para abrir un archivo. Si mode es ‘r’,
busca el archivo con nombre filename y retorna un osFile* que lo representa. Si mode es ‘w’, se verifica
que el archivo no exista y se retorna un nuevo osFile* que lo representa.*/
{
    // Check valid mode
    if (mode != 'r' && mode != 'w')
    {
        OS_ERROR = EPERM;
        os_strerror(EPERM, "Modo incorrecto");
        printf("INVALID MODE\n");
        return NULL;
    }

    if (strlen(filename) > 28) {
        OS_ERROR = ENAMETOOLONG;
        os_strerror(ENAMETOOLONG, filename);
        return NULL;
    }

    if (directory == NULL) {
        OS_ERROR = EPERM;
        os_strerror(EPERM, "No hay directorio");
        return NULL;
    }

    switch (mode)
    {
    case 'r':
        // READ MODE
        printf("Buscando archivo de nombre %s:\n", filename);
        // printf("Vamos a revisar los archivos y buscar el %s\n", filename);
        for (int i = 0; i < 64; i++)
        {
            if (directory->entradas_archivos[i]->validez == 1)
            {
                printf("ARCHIVO: %s\n", directory->entradas_archivos[i]->nombre_archivo);
                int ex = 1;
                for (int j = 0; j < strlen(filename); j++)
                {
                    if (directory->entradas_archivos[i]->nombre_archivo[j] != filename[j])
                    {
                        ex = 0;
                        break;
                    }
                }
                if (ex == 1)
                {
                    printf("El archivo %s fue encontrado\n", filename);
                    // Init osFile
                    osFile *osFile = osfile_init(mode, filename);
                    // Assign Indice to osFile
                    assign_osfile_indice(osFile, directory->entradas_archivos[i]->indice);
                    return osFile;
                }
            }
        }        
        OS_ERROR = ENOENT;
        os_strerror(ENOENT, filename);    
        printf("[ X ] - Archivo NO EXISTE en el directorio \n");
        return NULL;
                

    case 'w':
        // WRITE MODE
        printf("Verificando que el archivo %s NO exista \n", filename);
        for (int i = 0; i < 64; i++)
        {
            if (directory->entradas_archivos[i]->validez == 1)
            {
                int ex = 1;
                for (int j = 0; j < strlen(filename); j++)
                {
                    if (directory->entradas_archivos[i]->nombre_archivo[j] != filename[j])
                    {
                        ex = 0;
                        break;
                    }
                }
                if (ex == 1)
                {
                    OS_ERROR = ENOTUNIQ;
                    os_strerror(ENOTUNIQ, filename);
                    printf("[ X ] - Archivo EXISTE en el directorio \n");
                    return NULL;
                }
            }
        }
        // Archivo no encontrado en el directorio, se puede crear uno nuevo
        OS_ERROR = ENOENT;
        // Init osFile without Indice
        osFile *osFile = osfile_init(mode, filename);
        return osFile;
    default:
        break;
    }
    OS_ERROR = EPERM;
    os_strerror(EPERM, "Modo incorrecto");
    printf("MODE NOT RECOGNIZED\n");
    return NULL;
};

int os_read(osFile *file_desc, void *buffer, int nbytes)
/*Función para leer archivos.
Lee los siguientes nbytes desde el archivo descrito por file desc y los guarda en la dirección apuntada
por buffer. Debe retornar la cantidad de Bytes efectivamente leı́dos desde el archivo. Esto es importante si
nbytes es mayor a la cantidad de Bytes restantes en el archivo. La lectura de read se efectúa desde la posición
del archivo inmediatamente posterior a la última posición leı́da por un llamado a read.*/
{
    if (file_desc->indice->tamano == 0) {
        printf("Esta vacío\n");
        return 0;
    }
    // Utilizar Indice apuntando por osFile para hacer lectura de Datas
    // Extraer indice absoluto desde el directorio
    FILE *file = NULL;
    char data_buffer[2043]; // array of bytes, to store 2043 bytes of file Data

    // Open disk to read bytes
    file = fopen(NOMBRE_DISCO, "r");
    // Utilizar identificador absoluto de EntAr asociado para comenzar lectura
    unsigned int initial = (file_desc->indice->identificador_absoluto * 2048 + 1024) + 5; // Identificador absoluto + 5 bytes de tamano de archivo
    // printf("Iniciar en bloque %u\n", file_desc->indice->identificador_absoluto);
    // get_bits1(initial);
    // printf("---\n");
    fseek(file, initial, SEEK_SET);
    // printf("Posicion actual %ld\n", ftell(file));
    // fseek(file, 1, SEEK_SET);
    if (file != NULL)
    {
        // Leer de 1 byte y guardar 2043 de estos en el buffer
        fread(data_buffer, 1, 2043, file);
    } else {
        OS_ERROR = ENODATA;
        os_strerror(ENODATA, "");
        return 0;
    }

    // Iterar por los Bytes de Buffer generando struct Datas y Asignandolas al Indice
    // Comenzando desde el last read byte
    // int x = 681; // SE USARÁ?
    // Last read byte
    unsigned int LRB = file_desc->indice->last_read_byte;
    // Get starting point to read from the buffer
    int starting_point = (int)(LRB / 2048); // Math floor of LRB and 2KB from Data Block
    // Get ending point based on nbytes to read
    int ending_point = (int)((LRB + nbytes) / 2048) + 1; // Math floor of LRB + nbytes to read and 2KB from Data Block

    // Set bytes already read
    int bytes_read = 0;
    // Set buffer position to write on
    int buffer_pos = 0;
    // Set bytes left to read
    int n_bytes_left = nbytes;
    // Iterate in Indice Data Block segments
    for (int i = starting_point; i < 3 * ending_point; i += 3)
    {
        // Identificador relativo de Bloque de datos
        //printf("\tPrimer byte: %d\n", buffer[i]);
        //printf("\tSegundo byte: %d\n", buffer[i + 1]);
        //printf("\tTercer byte: %d\n", buffer[i + 2]);
        uint32_t identificador_bloque_datos = ((data_buffer[i] << 16) | (data_buffer[i + 1] << 8) | (data_buffer[i + 2]));
        // identificador_bloque_datos = bitExtracted(identificador_bloque_datos, 21, 1);

        // Inicializar Bloque de Datos y asignarlo al indice
        Data *data_block = data_init(identificador_bloque_datos + directory->indentificador_bloque);
        // printf("La posicion absoluta del Data os_read %d\n", data_block->identificador_absoluto);
        // Assign 2048 bytes to data byte array
        set_data_block(data_block, NOMBRE_DISCO, data_block->identificador_absoluto);
        // Asignar bloque de datos al indice
        // ES NECESARIO???

        // Hacer lectura de los nbytes desde bloque data y actualizar contador de bytes leidos
        if (n_bytes_left >= 0)
        {
            // Get bytes dispo to read in data block based on last readed byte
            int bytes_able_to_read = (((i + 1) * 2048) - LRB);
            // Declare bytes to be read
            int bytes_to_read = n_bytes_left < bytes_able_to_read ? n_bytes_left : bytes_able_to_read;
            // Set starting point to read from Data block
            int data_block_initial = (LRB - (i * 2048));
            read_bytes(data_block, buffer, buffer_pos, bytes_to_read, data_block_initial);
            // Actualizar ultimo byte leido desde el indice
            file_desc->indice->last_read_byte = LRB + bytes_to_read;
            // Actualizar bytes leidos desde el bloque de datos
            bytes_read += bytes_to_read;
            // Actualizar nbytes restantes por leer
            n_bytes_left -= bytes_to_read;
        }
        else
        {
            break;
        }
    }
    return bytes_read;
};

int os_write(osFile *file_desc, void *buffer, int nbytes)
/* Función para escribir archivos.
Escribe en el archivo descrito por file desc los nbytes que se encuentren en la dirección indicada
por buffer. Retorna la cantidad de Bytes escritos en el archivo. Si se produjo un error porque no pudo seguir
escribiendo, ya sea porque el disco se llen´o o porque el archivo no puede crecer más, este número puede ser
menor a nbytes (incluso 0). Cabe recalcar que los archivos parten con tama˜no 0 y crecen a medida que se
escribe en estos.*/
{
    // unsigned int initial = (file_desc->indice->identificador_absoluto * 2048 + 1024) + 5; // Identificador absoluto + 5 bytes de tamano de archivo

    if (directory->cantidad_archivos == 64){
        OS_ERROR = ENOMEM;
        os_strerror(ENOMEM, "");
        printf("EL directorio está lleno");
        return 1;
    }

    FILE *file = NULL;
    unsigned char buffer_aux[3]; // array of bytes, not pointers-to-bytes  => 3 Bytes

    file = fopen(NOMBRE_DISCO, "r");
    if (file != NULL)
    {
        // read up to sizeof(buffer) bytes
        fread(buffer_aux, 1, sizeof(buffer_aux), file);
    }


    EntAr* this_entar;
    int entrada;
    Indice* this_indice;

    for (int i = 0; i < 64; i++){
        if (!directory->entradas_archivos[i]->validez){
            // El primero que no sea válido
            printf("cantidad de archivos inicial: %d\n", directory->cantidad_archivos);
            // Identificadores de la ENT AR
            // unsigned int iden_absoluto = (directory->indentificador_bloque * 2048 + 1024) + i * 32;
            // unsigned int iden_relativo = i * 32;
            entrada = i;

            // Crear el Indice
            // REVISAR
            unsigned int identificador_en_bits = ((buffer_aux[0] << 16) | (buffer_aux[1] << 8) | (buffer_aux[2]));
            unsigned int identificador_en_int = bitExtracted(identificador_en_bits, 21, 1); // der a izq
            unsigned int iden_relativo_indice = identificador_en_int;
            unsigned int iden_absoluto_indice = identificador_en_int + directory->indentificador_bloque;

            this_indice = indice_init(0, iden_relativo_indice, iden_absoluto_indice);
            // Asociar el EntAr con el identificador del indice
            this_entar = entar_init(1, iden_relativo_indice, iden_absoluto_indice, file_desc->nombre_archivo, entrada);
            // assign_ent_ar(this_entar, this_indice);
            // Asignar indice al Ent Ar
            assign_indice(this_entar, this_indice);

            // Cambios en el directorio
            directory->cantidad_archivos += 1;
            directory->entradas_archivos[i] = this_entar;

            printf("cantidad de archivos final: %d\n", directory->cantidad_archivos);
            printf("identificador del directorio: %d\n", directory->indentificador_bloque);
            printf("entrada: %i\nidentificador relativo: %i\nidentificador absoluto: %i\n", entrada, iden_relativo_indice, iden_absoluto_indice);
            break;
        }
    }
    // asignarlo al OsFile 
    assign_osfile_indice(file_desc, this_indice);

    // Crear los Data del indice
    // revisar en cada uno si cabe el siguiente

    // Assign pointers
    unsigned int pointer;
    unsigned char buffer_pointer[3]; // array of bytes

    // Nos paramos donde comienza el ent ar + 5 bytes de tamaño
    fseek(file, this_entar->identificador_absoluto * 2048 + 1024 + 5, SEEK_SET); 
    
    // Asignar punteros al indice
    for (int index = 0; index < 681; index++){
        if (file != NULL){
            // Leer 3 bytes y guardarlos en el buffer
            fread(buffer_pointer, 1, 3, file);
            pointer = ((buffer_pointer[0] << 16) | (buffer_pointer[1] << 8) | (buffer_pointer[2]));
            if (pointer != 0) {
                printf("[i]\tAssign pointer %d\n", pointer);
            }
            assign_pointer(this_indice, pointer, index);
        }
    }
    fclose(file);
    
    // Crear los Data
    int cant_bloques_data = (int)nbytes / 2048 + 1;
    int contador_bytes_escritos = 0;
    int contador_bytes_por_escribir = nbytes;

    char* buffer_copy;
    buffer_copy= (char*)buffer;
    this_indice -> lista_de_punteros[0] = 15; // Se deberia definir por bitmap
    for (int i = 0; i < cant_bloques_data; i++){
        // ESTE EJEMPLO CONSIDERA QUE HAY UN PUNTERO
        // printf("\nidentificador del bloque%d\n", (directory -> indentificador_bloque));
        // printf("\nIdentificador del indice %d\n", (this_indice -> identificador_absoluto));
        // printf("\nLista de puntero %d\n", (this_indice -> lista_de_punteros[i]));
        // printf("\nidentificador del Ent Ar %d\n", (this_entar -> indice -> tamano));

        Data* data_ejemplo = data_init((directory -> indentificador_bloque + this_indice -> lista_de_punteros[i]));
        // set_data_block(data_ejemplo, NOMBRE_DISCO, directory -> indentificador_bloque + this_indice -> lista_de_punteros[i]);
        // data_ejemplo->byte_array = buffer;
        // Esto para guardar

        // Asignar data array al Data
        // Este seria el ultimo Data
        if(contador_bytes_por_escribir < 2048){
            for (int k = 0; k < contador_bytes_por_escribir; k++) {
                // printf("%c\n", buffer_copy[k + i * 2043]);
                data_ejemplo->byte_array[k] = buffer_copy[k + i * 2048];
            }
            contador_bytes_escritos += contador_bytes_por_escribir;
            contador_bytes_por_escribir = 0;
        }

        // Aun cabe en mas de un Data
        else{
            for (int k = 0; k < 2048; k++) {
                data_ejemplo->byte_array[k] = buffer_copy[k + i * 2048];
            }
            contador_bytes_escritos += 2048;
            contador_bytes_por_escribir -= 2048;
        }
        printf("[E]\t");
        for (int d = 0; d < 166; d++) {
            printf("%c", data_ejemplo->byte_array[d]);
        }
        printf("\n");
        assing_data_list(this_indice, data_ejemplo, i);
        // this_indice->cantidad_bloques += 1; // Se suma uno por cada iteracion
        // printf("[os_write]La cantidad de bloques es %d\n", file_desc -> indice->cantidad_bloques);
    }
    this_indice->tamano += contador_bytes_escritos;
    this_indice->cantidad_bloques = this_indice->tamano / 2048;
    if (this_indice -> cantidad_bloques * 2048 < this_indice->tamano) {
        this_indice -> cantidad_bloques += 1;
    }
    return contador_bytes_escritos;
};

int os_close(osFile *file_desc)
/*Función para cerrar archivos. Cierra el archivo indicado por
file desc. Debe garantizar que cuando esta función retorna, el archivo se encuentra actualizado en disco.*/
{
    if(file_desc->mode == 'r'){
        printf("Se cierra el archivo que se leyo\n");
        return 0;
    }
    // printf("LA cantidad de bloques es %d\n", file_desc -> indice->cantidad_bloques);
    printf("El tamano final es %d\n", file_desc -> indice->tamano);
    for (int i = 0; i < file_desc -> indice->cantidad_bloques; i++){
        // printf("Data identificador absoluto os_close %d\n", file_desc->indice->lista_de_datos[i]->identificador_absoluto);
        write_data(file_desc -> indice -> lista_de_datos[i]); // ---> Guarda la información de Data en su bloque correspondiente
    }
    // Guardar el indice
    write_indice(file_desc->indice);

    // Guardar el Ent Ar
    for (int i = 0; i < 64; i++){
        printf("Entrada %d\n", i);
        if (directory->entradas_archivos[i]->validez != 0){
            int ex = 1;
            printf("El nombre es %s\n", file_desc->nombre_archivo);
            for (int j = 0; j < strlen(file_desc->nombre_archivo); j++){
                if (directory->entradas_archivos[i]->nombre_archivo[j] != file_desc->nombre_archivo[j]){
                    ex = 0;
                    break;
                }
            }
            if (ex == 1){
                printf("El archivo %s existe en la posicion %d\n", file_desc->nombre_archivo, i);
                write_file_directory(directory, directory->entradas_archivos[i]);
                break;
            }
        }
    }
    OS_ERROR = ENOENT;
    os_strerror(ENOENT, file_desc->nombre_archivo);
    printf("El archivo %s NO existe os_close\n", file_desc->nombre_archivo);
    return 0;
};

int os_rm(char* filename) {
    if (directory == NULL) {
        OS_ERROR = EPERM;
        os_strerror(EPERM, "No hay directorio");
        return 0;
    }
    for (int i = 0; i < directory -> cantidad_archivos; i++){
        int ex = 1;
        for (int j = 0; j < strlen(filename); j++) {
            if (directory -> entradas_archivos[i]->nombre_archivo[j] != filename[j]){
                ex = 0;
                break;
            }
        }
        // Este es el archivo a eliminar
        // Los bloques que estaban siendo usados por el archivo deben quedar libres.
        if (ex == 1) {
            directory->cantidad_archivos -= 1;
            directory->entradas_archivos[i] -> validez = 0;
            // int cantidad_bitmaps = directory->cantidad_bloques_bitmap;
            // se busca el primer y ultimo bloque asociado al archivo
            int cantidad_bitmaps = directory->entradas_archivos[i] -> indice -> cantidad_bloques;
            int inicio = directory ->entradas_archivos[i] -> indice -> identificador_relativo;
            // int ultimo_bloque = cantidad_bitmaps + inicio;
            // en este for debemos buscar los bitmaps y cambiarlos a 0

            /// REVISAR
            // for (int j = inicio; j < cantidad_bitmaps; j++){
            //     // para ver que bitmap se utiliza para 
            //     float aux_bitmap = j / 2048;
            //     int que_bitmap;
            //     que_bitmap = (int)aux_bitmap;
            //     int que_bloque = j - 2048 * que_bitmap;
            //     mbt -> lista_de_particiones[PARTICION] -> lista_de_bitmaps[que_bitmap] -> bloques[que_bloque] = 0;
            // }
            printf("Se encontro el %s\n", filename);
            return 0;
        }
    }
    OS_ERROR = ENOENT;
    os_strerror(ENOENT, filename);
    printf("No se encontro el %s\n", filename);
    return 1;
};

// HELPERS

void set_mbt()
{
    MBT *_mbt = mbt_init();
    set_mbt_data(_mbt, NOMBRE_DISCO);
    mbt = _mbt;
    printf("[*] Se cargó el mbt\n");
};

void get_bits2(unsigned int num)
{
    int i = 0;
    unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1 << (size * 8 - 1);
    for (; i < size * 8; ++i)
    {
        // print last bit and shift left.
        printf("%u", num & maxPow ? 1 : 0);
        num = num << 1;
    }
    printf("\n");
}

void set_directory()
{
    Directory *_directory = directory_init(mbt->lista_de_particiones[PARTICION]->identificador_directorio, mbt->lista_de_particiones[PARTICION]->cantidad_bitmaps);
    // printf("Bloque %d\n", _directory -> indentificador_bloque);
    // get_bits2(_directory -> indentificador_bloque);
    // printf("byte del bloque a buscar %ld\n", ((long int) _directory -> indentificador_bloque) * 2048 + 1024); // * 2KB dir + 1KB mbt
    // get_bits2(_directory -> indentificador_bloque * 2048 + 1024);
    // get_bits3((long int) _directory -> indentificador_bloque);
    set_directory_data(_directory, NOMBRE_DISCO, (_directory->indentificador_bloque));
    directory = _directory;
    printf("[*] Se cargó el directorio\n");
}

void set_bitmap()
{
    unsigned int cantidad_bloques_particion = mbt->lista_de_particiones[PARTICION]->cantidad_bloques_particion;
    if (cantidad_bloques_particion < 16384)
    {
        set_bitmap_data(
            mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[0],
            NOMBRE_DISCO,
            mbt->lista_de_particiones[PARTICION]->identificador_directorio + 1,
            cantidad_bloques_particion);
    }
    else
    {
        int cantidad_bloques = cantidad_bloques_particion;
        unsigned int cantidad_bits_setear = 0;
        for (int i = 0; i < mbt->lista_de_particiones[PARTICION]->cantidad_bitmaps; i++)
        {
            if (cantidad_bloques > 16384)
            {
                cantidad_bits_setear = 16384;
            }
            else
            {
                cantidad_bits_setear = cantidad_bloques;
            }
            set_bitmap_data(
                mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[i],
                NOMBRE_DISCO,
                mbt->lista_de_particiones[PARTICION]->identificador_directorio + i + 1, // directorio
                cantidad_bits_setear);
            cantidad_bloques -= 16384;
        }
    }
    printf("[*] Se cargaron los bitmaps\n");
}