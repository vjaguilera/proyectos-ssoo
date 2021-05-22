#include "os_API.h"
#include <stdio.h>

// GENERALES

char *NOMBRE_DISCO;
int PARTICION;
MBT *mbt;
Directory *directory;

void os_mount(char *diskname, int partition)
{
    printf("Se ha montado la partición %d del disco %s.\n", partition, diskname);
    NOMBRE_DISCO = diskname;
    PARTICION = partition;
    set_mbt();
    set_directory();
    set_bitmap();
    // Definir directorio y bitmaps
};

void os_bitmap(unsigned num){
    // Revisar mbt -> lista_de_particiones[PARTICION] -> lista_bitmaps
    // Revisar mbt -> lista_de_particiones[PARTICION] -> cantidad_bitmaps
};

int os_exists(char *filename)
{
    return 0;
};

void os_ls()
{
    printf("Archivos válidos:\n");
    for (int i = 0; i < 64; i++)
    {
        if (directory->entradas_archivos[i]->validez == 1)
        {
            printf(" - %s\n", directory->entradas_archivos[i]->nombre_archivo);
        }
    }
};

// Master Boot Table

void os_mbt()
{
    printf("Particiones válidas:\n");
    for (int i = 0; i < 128; i++)
    {
        if (mbt->particiones_validas[i] == 1)
        {
            printf(" - %d - Bitmaps: %d\n", mbt->lista_de_particiones[i]->identificador_particion, mbt->lista_de_particiones[i]->cantidad_bitmaps);
        }
    }
};

void os_create_partition(int id, int size)
{
    printf("Crear particion %d de tamaño %d.\n", id, size);
};

void os_delete_partition(int id)
{
    printf("Eliminar particion %d.\n", id);
    mbt->particiones_validas[id] = 0;
    mbt->lista_de_particiones[id]->validez = 0;
};

void os_reset_mbt(){

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
    if (mode != 'r' || mode != 'w')
    {
        return NULL;
    }

    switch (mode)
    {
    case 'r':
        // READ MODE
        printf("Buscando archivo de nombre %s:\n", filename);
        for (int i = 0; i < 64; i++)
        {
            // Archivo encontrado en el directorio
            if (directory->entradas_archivos[i]->nombre_archivo == filename)
            {
                printf("Archivo encontrado en el directorio \n");

                // Init osFile
                osFile *osFile = osfile_init(mode, filename);
                // Assign Indice to osFile
                assign_osfile_indice(osFile, directory->entradas_archivos[i]->indice);
                return osFile;
            }
        }
        break;

    case 'w':
        // WRITE MODE
        printf("Verificando que el archivo %s NO exista \n", filename);
        for (int i = 0; i < 64; i++)
        {
            // Archivo encontrado en el directorio
            if (directory->entradas_archivos[i]->nombre_archivo == filename)
            {
                printf("[ X ] - Archivo EXISTE en el directorio \n");
                return NULL;
            }
        }
        // Archivo no encontrado en el directorio, se puede crear uno nuevo

        // Init osFile without Indice
        osFile *osFile = osfile_init(mode, filename);
        return osFile;
    default:
        break;
    }
    return NULL;
};

int os_read(osFile *file_desc, void *buffer, int nbytes)
/*Función para leer archivos.
Lee los siguientes nbytes desde el archivo descrito por file desc y los guarda en la dirección apuntada
por buffer. Debe retornar la cantidad de Bytes efectivamente leı́dos desde el archivo. Esto es importante si
nbytes es mayor a la cantidad de Bytes restantes en el archivo. La lectura de read se efectúa desde la posición
del archivo inmediatamente posterior a la última posición leı́da por un llamado a read.*/
{
    // Utilizar Indice apuntando por osFile para hacer lectura de Datas
    // Extraer indice absoluto desde el directorio
    FILE *file = NULL;
    char data_buffer[2043]; // array of bytes, to store 2043 bytes of file Data

    // Open disk to read bytes
    file = fopen(NOMBRE_DISCO, "r");
    // Utilizar identificador absoluto de EntAr asociado para comenzar lectura
    unsigned int initial = (file_desc->indice->identificador_absoluto * 2048 + 1024) + 5; // Identificador absoluto + 5 bytes de tamano de archivo
    printf("Iniciar en byte %u %ld\n", initial, sizeof(initial));
    // get_bits1(initial);
    // printf("---\n");
    fseek(file, initial, SEEK_SET);
    printf("Posicion actual %ld\n", ftell(file));
    // fseek(file, 1, SEEK_SET);
    if (file != NULL)
    {
        // Leer de 1 byte y guardar 2043 de estos en el buffer
        fread(buffer, 1, 2043, file);
    }

    // Iterar por los Bytes de Buffer generando struct Datas y Asignandolas al Indice
    // Comenzando desde el last read byte
    int x = 681;
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
        unsigned int identificador_bloque_datos = ((data_buffer[i] << 16) | (data_buffer[i + 1] << 8) | (data_buffer[i + 2]));
        identificador_bloque_datos = bitExtracted(identificador_bloque_datos, 24, 1);

        // Inicializar Bloque de Datos y asignarlo al indice
        Data *data_block = data_init();
        // Assign 2048 bytes to data byte array
        set_data_block(data_block, NOMBRE_DISCO, identificador_bloque_datos);
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
{
    return 0;
};

int os_close(osFile *file_desc)
{
    return 0;
};

int os_rm(char *filename)
{
    return 0;
};

// HELPERS

void set_mbt()
{
    MBT *_mbt = mbt_init();
    set_mbt_data(_mbt, NOMBRE_DISCO);
    mbt = _mbt;
    printf("\tSe cargó el mbt\n");
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
    // printf("Iden %d\n", _directory -> indentificador_bloque);
    // get_bits2(_directory -> indentificador_bloque);
    // get_bits2(_directory -> indentificador_bloque * 2048 + 1024);
    // get_bits3((long int) _directory -> indentificador_bloque);
    printf("Iden %d\n", _directory->indentificador_bloque);
    printf("Iden bytes %d\n", _directory->indentificador_bloque * 2048 + 1024);
    set_directory_data(_directory, NOMBRE_DISCO, (_directory->indentificador_bloque * 2048 + 1024));
    directory = _directory;
    printf("\tSe cargó el directorio\n");
}

void set_bitmap()
{
    unsigned int cantidad_bloques_particion = mbt->lista_de_particiones[PARTICION]->cantidad_bloques_particion;
    if (cantidad_bloques_particion < 16384)
    {
        set_bitmap_data(
            mbt->lista_de_particiones[PARTICION]->lista_de_bitmaps[0],
            NOMBRE_DISCO,
            mbt->lista_de_particiones[PARTICION]->identificador_directorio * 2048 + 2048 + 1024,
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
                mbt->lista_de_particiones[PARTICION]->identificador_directorio + 2048 + i * 2048 + 1024, // directorio + tamaño directorio + bitmaps leidos
                cantidad_bits_setear);
            cantidad_bloques -= 16384;
        }
    }
    printf("\tSe cargaron los bitmaps\n");
}