#pragma once
#include "indice.h"


typedef struct ent_ar
{
    int validez; // se castea desde un char
    unsigned int identificador_relativo;
    unsigned int identificador_absoluto;
    Indice* indice;
    char* nombre_archivo;
} EntAr;

// FUNCTIONS

EntAr* entar_init(char validez, unsigned int identificador_relativo, unsigned int identificador_absoluto, char* nombre_archivo);

void assign_indice(EntAr* entar, Indice* indice);

void entar_clean(EntAr* entar);