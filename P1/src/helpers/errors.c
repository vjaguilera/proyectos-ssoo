#include "errors.h"

void os_strerror(Error error) {
    switch (error)
    {
    case EPERM:
        printf("[E] Error: Operación no permitida\n");
        break;
    case ENOENT:
        printf("[E] Error: No existe directorio\n");
        break;
    case ENOMEM:
        printf("[E] Error: Sin memoria\n");
        break;
    case ENODATA:
        printf("[E] Error: No hay data disponible\n");
        break;
    case EEXIST:
        printf("[E] Error: Archivo existe\n");
        break;
    case EMFILE:
        printf("[E] Error: Muchos archivos abiertos\n");
        break;
    case EROFS:
        printf("[E] Error: Archivo solo de lectura\n");
        break;
    case ENAMETOOLONG:
        printf("[E] Error: Nombre archivo muy largo\n");
        break;
    case ENOTUNIQ:
        printf("[E] Error: Nombre archivo no es único\n");
        break;
    default:
        printf("[E] Error code not supported: %d\n", error);
        break;
    }
}