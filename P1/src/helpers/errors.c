#include "errors.h"

void os_strerror(Error error, char* description) {
    switch (error)
    {
    case EPERM:
        printf("[E] Error: Operación no permitida '%s'\n", description);
        break;
    case ENOENT:
        printf("[E] Error: No existe archivo o directorio '%s'\n", description);
        break;
    case ENOMEM:
        printf("[E] Error: No hay memoria disponible\n");
        break;
    case ENODATA:
        printf("[E] Error: No hay data disponible\n");
        break;
    case EMFILE:
        printf("[E] Error: Muchos archivos abiertos\n");
        break;
    case EROFS:
        printf("[E] Error: Archivo solo de lectura\n");
        break;
    case ENAMETOOLONG:
        printf("[E] Error: Nombre archivo '%s' muy largo\n", description);
        break;
    case ENOTUNIQ:
        printf("[E] Error: Archivo '%s' no es único\n", description);
        break;
    default:
        printf("[E] Error code not supported: %d\n", error);
        break;
    }
}