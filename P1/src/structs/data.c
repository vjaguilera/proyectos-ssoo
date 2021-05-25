#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include "../helpers/writeBytes.h"

Data *data_init(int identificador)
{
    Data *data = malloc(sizeof(Data));
    data -> identificador_absoluto = identificador;
    return data;
}

void set_data_block(Data *data_block, char *diskname, unsigned int initial)
{
    FILE *file = NULL;
    char buffer[2048]; // array of bytes, not pointers-to-bytes  => 2KB

    file = fopen(diskname, "r");

    fseek(file, (initial * 2048) + 1024, SEEK_SET);
    // printf("Posicion actual %ld\n", ftell(file));
    // fseek(file, 1, SEEK_SET);
    if (file != NULL)
    {
        // Leer de 1 byte y guardar 2043 de estos en el buffer
        fread(buffer, 1, sizeof(buffer), file);
    }
    // Assign data buffer to Data struct
    data_block->byte_array = buffer;
}

void read_bytes(Data *data_block, char *buffer, int buffer_pos, int bytes_to_read, unsigned int initial)
{
    for (int i = 0; i < bytes_to_read; i++)
    {
        // Actualizar buffer con los datos del bloque de datos
        buffer[i + buffer_pos] = data_block->byte_array[i + initial];
    }
    // Actualizar la ultima posicion del buffer de respuesta
    buffer_pos += bytes_to_read;
}

void data_clean(Data *data)
{
    free(data->byte_array);
    free(data);
}

void write_data(Data* data) {
    printf("[g] Guardar data\n");
    unsigned char bytes_array[2048];
    for (int i = 0; i < 2048; i++)
    {
        bytes_array[i] = (unsigned char) data->byte_array[i];
    }
    writeBytes(data -> identificador_absoluto, 0, bytes_array, 2048);
}