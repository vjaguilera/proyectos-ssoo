#include "data.h"
#include <stdlib.h>
#include "../helpers/writeBytes.h"

Data* data_init(char* array_bytes, int identificador) {
    Data* data = malloc(sizeof(Data));
    data -> byte_array = array_bytes;
    data -> identificador_absoluto = identificador;
    return data;
}

void data_clean(Data* data) {
    free(data -> byte_array);
    free(data);
}

void write_data(Data* data) {
    unsigned int size = sizeof(data -> byte_array);
    
    printf("Guardar %d bytes de data\n", size);
    writeBytes(data -> identificador_absoluto, 0, data -> byte_array, size);
}