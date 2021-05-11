#include "osFile.h"
#include <stdlib.h>

osFile* osfile_init(int mode, char* nombre_archivo, Indice* indice) {
    osFile* osfile = malloc(sizeof(osFile));
    osfile ->mode = mode;
    osfile -> nombre_archivo = nombre_archivo;
    osfile -> indice = indice;
    return osfile;
}

void osfile_clean(osFile* osfile) {
    indice_clean(osfile -> indice);
    free(osfile);
};
