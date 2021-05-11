#pragma once
#include "data.h"

typedef struct indice {
    unsigned long int tamano; // tamano archivo
    int cantidad_bloques; // tamaño dividido por 2kb
    Data** lista_de_datos; // lista con numeros de bloques
    unsigned long int identificador_relativo;
    unsigned long int identificador_absoluto;
} Indice;


// FUNCTIONS

Indice* indice_init(unsigned long int tamano, unsigned long int identificador_relativo, unsigned long int identificador_absoluto);

void assing_data_list(Indice* indice, Data* data, int index);

void indice_clean(Indice* indice);
