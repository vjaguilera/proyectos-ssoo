# Proyecto 1

**Integrantes:**

- 16637860 | Vicente Aguilera
- 15624439 | Sebastian Calderon
- 16666666 | Tomas Castro
- 16635264 | Ignacio Contreras
- 16666666 | Jan Siegel

## Descripcion del proyecto

El objetivo de este proyecto es crear una API que permita interactura con un disco. Para poder realizar esto, se definieron las siguientes structs:

### Bitmap

Representa al bloque _bitmap_ dentro de la particion, contiene la cantidad de bloques dentro de la particion y los bits que representan si el bloque esta libre o no (en forma de array de ints).

### Data

Representa al bloque de datos dentro de la particion, contiene su identificador absoluto dentro de la particion, y un array de bytes que contiene los 2KB de informacion del archivo.

### Directory

Representa al bloque directorio dentro de la particion, contiene un identificador de bloque, que es unico para cada bloque, tambien contiene un numero que representa la cantidad de archivos en el directorio, un numero que representa la cantidad de bloques bitmap y un arreglo de **Entradas de Archivos**, que corresponden a las entradas del bloque en si (son otra struct)

### EntAr

Representa las entradas del bloque directorio, cada una contiene un caracter de validez, un identificador absoluto y uno relativo a la particion, una referencia al bloque indice, el nombre del archivo en la entrada, y un numero que representa la entrada.

### EntDir

Representa las entradas del MBT, cada una contiene un bit de validez, un identificador de la particion, un identificador de directorio, el numero de la cantidad de bloques en la particion, la cantidad de bitmas necesarios, un numero identificador de la entrada y un arreglo de Bitmaps con la referencia a los structs Bitmap.

### Indice

Representa un bloque Indice, contiene el tamano del archivo, la cantidad de bloques referenciados, un array con punteros a los bloques de datos, un identificador relativo, un identificador absoluto, una referencia al ultimo byte leido entre todos los bloques de datos y un array con lista de punteros

### MBT

Representa el bloque MBT, contiene un arreglo de punteros a las entradas del MBT (struc EntDir) y un arreglo con las particiones validas en el disco.

### OS FILE

Representa el archivo que es abierto, tiene un modo (que puede ser lectura o escritura, r/w por sus siglas en ingles), el nombre del archivo y el la referencia al bloque indice que lo contiene.
