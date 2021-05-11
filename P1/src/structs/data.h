#pragma once

typedef struct data {
    char* byte_array;
} Data;

// FUNCTIONS

Data* data_init();

void data_clean(Data* data);