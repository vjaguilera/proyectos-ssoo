#pragma once

typedef struct data {
    int identificador_absoluto;
    char *byte_array;
} Data;

// FUNCTIONS

Data *data_init(int identificador);

void set_data_block(Data *data_block, char *diskname, unsigned int initial);

void read_bytes(Data *data_block, char *buffer, int buffer_pos, int bytes_to_read, unsigned int initial);

void data_clean(Data *data);

void write_data(Data *data);
