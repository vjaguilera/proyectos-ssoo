#include "osFile.h"
#include <stdlib.h>

osFile *osfile_init(int mode, char *nombre_archivo)
{
    osFile *osfile = malloc(sizeof(osFile));
    osfile->mode = mode;
    osfile->nombre_archivo = nombre_archivo;
    return osfile;
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
