#pragma once

typedef struct data
{
    char *byte_array;
} Data;

// FUNCTIONS

Data *data_init();

void set_data_block(Data *data_block, char *diskname, unsigned int initial);

void read_bytes(Data *data_block, char *buffer, int buffer_pos, int bytes_to_read, unsigned int initial);

void data_clean(Data *data);