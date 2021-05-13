#include "indice.h"
#include <stdlib.h>

Indice* indice_init(signed int tamano, signed int identificador_relativo, signed int identificador_absoluto) {
    Indice* indice = malloc(sizeof(Indice));
    indice -> tamano = tamano;
    indice -> cantidad_bloques = tamano / 2048;
    indice -> lista_de_datos = malloc(sizeof (Data*) * indice -> cantidad_bloques );
    indice -> identificador_relativo = identificador_relativo;
    indice -> identificador_absoluto = identificador_absoluto;
    return indice;
};

void assing_data_list(Indice* indice, Data* data, int index) {
    // int cantidad_bloques = sizeof data_list_pointer / sizeof signed int;
    indice -> lista_de_datos[index] = data;
}

void indice_clean(Indice* indice) {
    for (int i = 0; i < indice -> cantidad_bloques; i++) {
        free(indice -> lista_de_datos[i]);
    };
    free(indice -> lista_de_datos);
    free(indice);
};