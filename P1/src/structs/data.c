#include "data.h"
#include <stdlib.h>

Data* data_init(char* array_bytes) {
    Data* data = malloc(sizeof(Data));
    data -> byte_array = array_bytes;
    return data;
}

void data_clean(Data* data) {
    free(data -> byte_array);
    free(data);
}