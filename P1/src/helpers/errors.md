# Errores

Todas las funciones de la API_OS tienen algún uso de estos errores. Se definieron los siguientes errores:

| Error  | Explicación  | Mensaje | Casos  |
|---|---|---|---|
| EPERM  | Se utilizará cuando el llamado a una función con un identificador podría causar la falla directamente.  | Operación no permitida %s |<ul><li>Acceso a partición inválida</li><li>Acceso a bitmap inválido</li><li>Operaciones sobre directorio nulo</li><li>Creación partición número incorrecto o siendo utilizado</li><li>Modo lectura incorrecto</li></ul> |
| ENOENT  | Cuando no existe archivo o un directorio se entrega este error. | No existe archivo o directorio %s | <ul><li>Archivo no existe</li><li>No hay archivos en el directorio</li></ul>  |
| ENODATA  | Cuando no se puede leer información del disco.   | No hay data disponible  |  <ul><li>No hay particiones válidas</li><li>No hay lectura de disco</li></ul> |
| ENOMEM  | Cuando no hay memoria disponible para crear particiones o archivos.  |  No hay memoria disponible |  <ul><li>Nueva partición no tiene espacio suficente en disco</li><li>Nuevo archivo no tiene espacio suficiente en disco</li></ul> |
| EROFS  | Cuando el archivo es solo de lectura y se quiere escribir.  | Archivo solo de lectura  | <ul><li>Abrir archivo en lectura e intentar escribir en él</li></ul>  |
| ENAMETOOLONG  | Cuando el nombre de archivo supera los 28 caracteres.  | Nombre archivo muy largo %s  | <ul><li>Crear archivo con nombre mayor a 28 caracteres</li</ul>  |
| ENOTUNIQ  | Cuando ya existe un archivo con el mismo nombre.  | Nombre archivo no es único %s  | <ul><li>Escribir archivo con nombre existente en directorio</li></ul>  |