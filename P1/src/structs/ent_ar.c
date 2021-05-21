#include "ent_ar.h"
#include <stdlib.h>
#include <stdio.h>

EntAr* entar_init(char validez, unsigned int identificador_relativo, unsigned int identificador_absoluto, char* nombre_archivo) {
    EntAr* entar = malloc(sizeof(EntAr));
    entar -> validez = (int) validez;
    entar -> identificador_relativo = identificador_relativo;
    entar -> identificador_absoluto = identificador_absoluto;
    entar -> indice = malloc(sizeof(Indice*));
    entar -> nombre_archivo = nombre_archivo;
    return entar;
};

void assign_indice(EntAr* entar, Indice* indice){
    entar -> indice = indice;
}

void entar_clean(EntAr* entar) {
    free(entar -> indice);
    free(entar);
}