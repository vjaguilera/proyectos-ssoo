#pragma once
#include "data.h"

typedef struct indice
{
    unsigned int tamano;   // tamano archivo
    int cantidad_bloques;  // tamaño dividido por 2kb
    Data **lista_de_datos; // lista con numeros de bloques
    unsigned int identificador_relativo;
    unsigned int identificador_absoluto;
    unsigned int last_read_byte;
    int* lista_de_punteros;
} Indice;

// FUNCTIONS

Indice *indice_init(unsigned int tamano, unsigned int identificador_relativo, unsigned int identificador_absoluto);

void assing_data_list(Indice *indice, Data *data, int index);

void indice_clean(Indice* indice);

void write_indice(Indice* indice);

