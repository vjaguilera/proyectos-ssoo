#include "osFile.h"
#include <stdlib.h>
#include <stdio.h>

osFile *osfile_init(char mode, char *nombre_archivo)
{
    osFile *osfile = malloc(sizeof(osFile));
    osfile->mode = mode;
    osfile->nombre_archivo = nombre_archivo;
    return osfile;
}

void osfile_print(osFile *osfile)
{
    printf("[OSFILE] MODE: %c | NOMBRE: %s | TAMANO: %d | # BLOQUES: %d | IA: %d | LRB: %d\n", osfile->mode, osfile->nombre_archivo, osfile->indice->tamano, osfile->indice->cantidad_bloques, osfile->indice->identificador_absoluto, osfile->indice->last_read_byte);
}

void assign_osfile_indice(osFile *osFile, Indice *indice)
{
    osFile->indice = indice;
}

void osfile_clean(osFile *osfile)
{
    indice_clean(osfile->indice);
    free(osfile);
};
