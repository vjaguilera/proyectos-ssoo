#include "data.h"
#include <stdio.h>
#include <stdlib.h>

Data *data_init()
{
    Data *data = malloc(sizeof(Data));
    return data;
}

void set_data_block(Data *data_block, char *diskname, unsigned int initial)
{
    FILE *file = NULL;
    char buffer[2048]; // array of bytes, not pointers-to-bytes  => 2KB

    file = fopen(diskname, "r");

    lseek(fileno(file), initial, SEEK_SET);
    printf("Posicion actual %ld\n", ftell(file));
    // fseek(file, 1, SEEK_SET);
    if (file != NULL)
    {
        // Leer de 1 byte y guardar 2043 de estos en el buffer
        fread(buffer, 1, sizeof(buffer), file);
    }
    // Assign data buffer to Data struct
    data_block->byte_array = buffer;
}

void read_bytes(Data *data_block, char *buffer, int buffer_pos, int bytes_to_read)
{
    for (int i = 0; i < bytes_to_read; i++)
    {
        // Actualizar buffer con los datos del bloque de datos
        buffer[i + buffer_pos] = data_block->byte_array[i];
    }
    // Actualizar la ultima posicion del buffer de respuesta
    buffer_pos += bytes_to_read;
}

void data_clean(Data *data)
{
    free(data->byte_array);
    free(data);
}