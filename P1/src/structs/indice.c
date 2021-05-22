#include "indice.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../helpers/writeBytes.h"
#include "../helpers/bitExtract.h"

Indice* indice_init(unsigned int tamano, unsigned int identificador_relativo, unsigned int identificador_absoluto) {
    Indice* indice = malloc(sizeof(Indice));
    indice -> tamano = tamano;
    indice -> cantidad_bloques = tamano / 2048;
    indice -> lista_de_datos = malloc(sizeof (Data*) * indice -> cantidad_bloques );
    indice -> identificador_relativo = identificador_relativo;
    indice -> identificador_absoluto = identificador_absoluto;
    return indice;
};

void assing_data_list(Indice* indice, Data* data, int index) {
    // int cantidad_bloques = sizeof data_list_pointer / sizeof unsigned int;
    indice -> lista_de_datos[index] = data;
}

void indice_clean(Indice* indice) {
    for (int i = 0; i < indice -> cantidad_bloques; i++) {
        free(indice -> lista_de_datos[i]);
    };
    free(indice -> lista_de_datos);
    free(indice);
};

void write_indice(Indice* indice) {
    unsigned char bytes_array[2048];
    unsigned int size;
    char* response;
    // Primeros 5 bytes tamaño archivo
    int j = indice -> tamano;
    size = 1;
    while (j > 255) {
        size += 1;
        j /= 255;
    }
    response = calloc(1, size);
    get_bits(response, indice -> tamano, 0);
    while (size < 5) {
        bytes_array[0 + 4 - size] = 0;
        size += 1;
    }
    int arraySize = strlen(response);
    int i = 0;
    char subset[8];
    while (i * 8 < arraySize) {
        for (int j = 0; j < 8; j++) {
            subset[j] = response[arraySize - (i + 1) * 8 + j];
        }
        i += 1;
        bytes_array[0 + size - i] = binarioADecimal(subset, 8);
        for (int i = 0; i < 8; i++) {
            subset[i] = 0;
        }
    }
    printf("-- %d %d %d %d %d\n", bytes_array[0], bytes_array[1], bytes_array[2], bytes_array[3], bytes_array[4]);

    // PUNTEROS
    // Guardar punteros

    for (i = 0; i < indice -> cantidad_bloques; i++) {
        // indice -> lista_de_punteros[i]
        // Obtener 3 bytes para cada numero
        int numeros[3];
        bytes_array[5 + i * 3 + 0] = numeros[0];
        bytes_array[5 + i * 3 + 1] = numeros[1];
        bytes_array[5 + i * 3 + 2] = numeros[2];
    }

    writeBytes(indice -> identificador_absoluto, 0, bytes_array, 2048);
}