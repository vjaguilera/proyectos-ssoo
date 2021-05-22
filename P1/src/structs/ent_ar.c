#include "ent_ar.h"
#include <stdlib.h>
#include <stdio.h>

EntAr* entar_init(char validez, unsigned int identificador_relativo, unsigned int identificador_absoluto, char* nombre_archivo, int entrada) {
    EntAr* entar = malloc(sizeof(EntAr));
    entar -> validez = (int) validez;
    entar -> identificador_relativo = identificador_relativo;
    entar -> identificador_absoluto = identificador_absoluto;
    entar -> indice = malloc(sizeof(Indice*));
    entar -> nombre_archivo = malloc(sizeof(char) * 28);
    for (int i = 0; i < 28; i++) {
        entar -> nombre_archivo[i] = nombre_archivo[i];
    }
    entar -> entrada = entrada;
    return entar;
};

void assign_indice(EntAr* entar, Indice* indice){
    entar -> indice = indice;
}

void entar_clean(EntAr* entar) {
    free(entar -> indice);
    free(entar);
}