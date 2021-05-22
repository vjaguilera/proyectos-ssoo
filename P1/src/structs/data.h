#pragma once

typedef struct data {
    int identificador_absoluto;
    char* byte_array;
} Data;

// FUNCTIONS

Data* data_init(char* array_bytes, int identificador);

void data_clean(Data* data);

void write_data(Data* data);