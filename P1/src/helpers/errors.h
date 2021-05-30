#include <stdio.h>


typedef enum errors {
    EPERM, // OPERACION NO PERMITIDA: os_mount, os_bitmap
    ENOENT, // NO SUCH FILE OR DIRECTORY: os_exists
    ENODATA, // NO DATA AVAILABLE
    ENOMEM, // OUT OF MEMORY: os_create_partition, os_write   
    EMFILE, // TOO MANY OPEN FILES: os_open
    EROFS, // READ ONLY FILE: os_open
    ENAMETOOLONG, // FILENAME TOO LONG: os_open
    ENOTUNIQ, // NAME NOT UNIQUE: os_open

} Error;

Error OS_ERROR;

void os_strerror(Error error, char* description);