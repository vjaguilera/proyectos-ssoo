#pragma once
#include "indice.h"

typedef struct osfile
{
    char mode; // 0: Write, 1: Read
    char *nombre_archivo;
    Indice *indice;
} osFile;

osFile *osfile_init(char mode, char *nombre_archivo);

void osfile_print(osFile *osfile);

void assign_osfile_indice(osFile *osFile, Indice *indice);

void osfile_clean(osFile *osfile);
