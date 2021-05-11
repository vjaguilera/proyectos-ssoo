#pragma once
#include "indice.h"

typedef struct osfile
{
    int mode; // 0: Write, 1: Read
    char* nombre_archivo;
    Indice* indice;
} osFile;

osFile* osfile_init(int mode, char* nombre_archivo, Indice* indice);

void osfile_clean(osFile* osfile);

