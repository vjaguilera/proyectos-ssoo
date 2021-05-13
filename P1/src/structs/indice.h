#pragma once
#include "data.h"

typedef struct indice {
    signed int tamano; // tamano archivo
    int cantidad_bloques; // tamaño dividido por 2kb
    Data** lista_de_datos; // lista con numeros de bloques
    signed int identificador_relativo;
    signed int identificador_absoluto;
} Indice;


// FUNCTIONS

Indice* indice_init(signed int tamano, signed int identificador_relativo, signed int identificador_absoluto);

void assing_data_list(Indice* indice, Data* data, int index);

void indice_clean(Indice* indice);
