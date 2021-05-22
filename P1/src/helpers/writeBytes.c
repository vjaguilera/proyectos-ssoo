#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* DISKNAME = "file1.txt";


void set_disk_name(char* diskname) {
    DISKNAME = diskname;
};


void writeBytes(int bloque_inicial, int numero_byte_inicial, char* bytes_array, int cantidad) {
    FILE* file_write = fopen(DISKNAME, "r+");  
    int position = 1024 + bloque_inicial * 2048 + numero_byte_inicial;
    printf("Set position %d %d\n", position, cantidad);
    fseek(file_write, position, SEEK_SET);
    fwrite(bytes_array, cantidad, 1, file_write);
    fclose(file_write);  
}

void writeBytesMBT(int numero_byte_inicial, char* bytes_array, int cantidad) {
    FILE* file_write = fopen(DISKNAME, "r+");  
    int position = numero_byte_inicial;
    printf("Set position MBT %d %d %s\n", position, cantidad, DISKNAME);
    printf("%x\n", bytes_array[0]);
    if (file_write != NULL) {
        // fseek(file_write, position, SEEK_SET);
        // fwrite(bytes_array, cantidad, 1, file_write);
        fclose(file_write);  
    } else {
        perror("fopen");
    }
}

// FOR TESTING
// https://stackoverflow.com/questions/37451319/how-to-write-file-at-specific-position
// static void createFile(void)
// {
//     FILE * fPtr;
//     fPtr = fopen("file1.txt", "w");
//     if(fPtr == NULL)
//     {
//         exit(EXIT_FAILURE);
//     }
//     char data[16] = "hola como estas!";
//     int i = 0;
//     while (i < 41984) { // 20 bloques de 2kb + MBT
//         fputs(data, fPtr);
//         i+= 16;
//     }
//     fclose(fPtr);
// }

// int main() {
    // createFile();
    // writeBytes(1, 2, "funciona"); // deberia quedar un hofuncionaestas! en el byte 1024 + 2048 * 1 + 2 = 3074
    // writeBytesMBT(10, "pane"); // deberia quedar un hola como panes! en el byte 10
// }