#pragma once
#include "indice.h"


typedef struct ent_ar
{
    int validez; // se castea desde un char
    unsigned long int identificador_relativo;
    unsigned long int identificador_absoluto;
    Indice* indice;
    char* nombre_archivo;
} EntAr;

// FUNCTIONS

EntAr* entar_init(char validez, unsigned long int identificador_relativo, unsigned long int identificador_absoluto, char* nombre_archivo);

void assign_indice(EntAr* entar, Indice* indice);

void entar_clean(EntAr* entar);