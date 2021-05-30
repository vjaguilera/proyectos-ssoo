# Errores

Todas las funciones de la API_OS tienen algún uso de estos errores. Se definieron los siguientes errores:

| Error  | Explicación  | Mensaje | Casos  |
|---|---|---|---|
| EPERM  | Se utilizará cuando el llamado a una función con un identificador podría causar la falla directamente.  | Operación no permitida %s |  |
| ENOENT  | Cuando no existe archivo o un directorio se entrega este error. | No existe archivo o directorio %s |   |
| ENODATA  | Cuando no se puede leer información del disco.   | No hay data disponible  |   |
| ENOMEM  | Cuando no hay memoria disponible para crear particiones o archivos.  |  No hay memoria disponible |   |
| EEXIST  | Cuando el archivo ya existe y no se puede reemplazar.  |  Archivo ya existe |   |
| EROFS  | Cuando el archivo es solo de lectura y se quiere escribir.  | Archivo solo de lectura  |   |
| ENAMETOOLONG  | Cuando el nombre de archivo supera los 28 caracteres.  | Nombre archivo muy largo %s  |   |
| ENOTUNIQ  | Cuando ya existe un archivo con el mismo nombre.  | Nombre archivo no es único %s  |   |