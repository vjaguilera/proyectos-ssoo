#include "indice.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../helpers/writeBytes.h"
#include "../helpers/bitExtract.h"

Indice *indice_init(unsigned long int tamano, unsigned int identificador_relativo, unsigned int identificador_absoluto)
{
    Indice *indice = malloc(sizeof(Indice));
    indice->tamano = tamano;
    indice->cantidad_bloques = tamano / 2048;
    if (indice -> cantidad_bloques * 2048 < tamano) {
        indice -> cantidad_bloques += 1;
    }
    indice->lista_de_datos = malloc(sizeof(Data *) * indice->cantidad_bloques);
    indice->identificador_relativo = identificador_relativo;
    indice->identificador_absoluto = identificador_absoluto;
    indice->last_read_byte = 0;
    indice->lista_de_punteros = calloc(681, sizeof(int)); // relativos
    return indice;
};

void assing_data_list(Indice *indice, Data *data, int index)
{
    // int cantidad_bloques = sizeof data_list_pointer / sizeof unsigned int;
    indice->lista_de_datos[index] = data;
}

void assign_pointer(Indice *indice, int pointer, int index)
{
    indice->lista_de_punteros[index] = pointer;
}

void indice_clean(Indice *indice)
{
    for (int i = 0; i < indice->cantidad_bloques; i++)
    {
        free(indice->lista_de_datos[i]);
    };
    free(indice->lista_de_datos);
    free(indice);
};

void write_indice(Indice *indice)
{
    printf("[g] Guardar índice\n");
    unsigned char* bytes_array = calloc(2048, 1);
    unsigned int size;
    char *response;
    // Primeros 5 bytes tamaño archivo
    int j = indice->tamano;
    size = 1;
    while (j > 255)
    {
        size += 1;
        j /= 255;
    }
    int osize = size;
    response = calloc(1, size);
    get_bits(response, indice->tamano, 0);
    while (size < 5)
    {
        bytes_array[0 + 4 - size] = 0;
        size += 1;
    }
    int i = 0;
    char subset[8];
    while (i * 8 < osize * 8)
    {
        for (int j = 0; j < 8; j++)
        {
            subset[j] = response[osize * 8 - (i + 1) * 8 + j];
        }
        i += 1;
        bytes_array[0 + size - i] = binarioADecimal(subset, 8);
        for (int i = 0; i < 8; i++)
        {
            subset[i] = 0;
        }
    }
    // printf("-- %d %d %d %d %d\n", bytes_array[0], bytes_array[1], bytes_array[2], bytes_array[3], bytes_array[4]);

    // PUNTEROS
    // Guardar punteros

    int pointer;
    printf("[g] Guardar %d punteros\n", indice -> cantidad_bloques);
    for (int i = 0; i < 681; i++) {
        // indice -> lista_de_punteros[i]
        // Obtener 3 bytes para cada numero
        pointer = indice -> lista_de_punteros[i];
        int j = pointer;
        size = 1;
        while (j > 255)
        {
            size += 1;
            j /= 255;
        }
        response = calloc(1, size);
        get_bits(response, pointer, 0);
        while (size < 3)
        {
            bytes_array[5 + i * 3 + 2 - size] = 0;
            size += 1;
        }
        int arraySize = strlen(response);
        int k = 0;
        char subset[8];
        while (k * 8 < arraySize)
        {
            for (int j = 0; j < 8; j++)
            {
                subset[j] = response[arraySize - (k + 1) * 8 + j];
            }
            bytes_array[4 + i * 3 + size - k] = binarioADecimal(subset, 8);
            k += 1;
            for (int p = 0; p < 8; p++)
            {
                subset[p] = 0;
            }
        }
    }
    // printf("-- %d %d %d %d %d %d %d %d %d\n", bytes_array[5], bytes_array[6], bytes_array[7],
                                    //  bytes_array[8], bytes_array[9], bytes_array[10],
                                    //  bytes_array[11], bytes_array[12], bytes_array[13]);

    writeBytes(indice->identificador_absoluto, 0, bytes_array, 2048);

}