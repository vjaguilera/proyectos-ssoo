# Errores

Se definieron los siguientes errores:

EPERM, // OPERACION NO PERMITIDA: os_mount, os_bitmap
ENOENT, // NO SUCH FILE OR DIRECTORY: os_exists
ENODATA, // NO DATA AVAILABLE
ENOMEM, // OUT OF MEMORY: os_create_partition, os_write   
EEXIST, // FILE EXISTS: os_exists
EMFILE, // TOO MANY OPEN FILES: os_open
EROFS, // READ ONLY FILE: os_open
ENAMETOOLONG, // FILENAME TOO LONG: os_open
ENOTUNIQ, // NAME NOT UNIQUE: os_open

| Error  | Explicación  | Mensaje | Casos  |
|---|---|---|---|
| EPERM  | Operación no permitida: Se utilizará cuando el llamado a una función con un identificador podría causar la falla directamente.  | Operación no permitida |  |
| ENOENT  |   | No existe directorio  |   |
| ENODATA  |   | No hay data disponible  |   |
| ENOMEM  |   |  No hay memoria disponible |   |
| EEXIST  |   |  Archivo ya existe |   |
| EMFILE  |   |  Muchos archivos abiertos |   |
| EROFS  |   | Archivo solo de lectura  |   |
| ENAMETOOLONG  |   | Nombre archivo muy largo  |   |
| ENOTUNIQ  |   | Nombre archivo no es único  |   |