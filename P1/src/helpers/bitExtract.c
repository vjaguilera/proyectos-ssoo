#include <string.h>
#include <stdio.h>

// https://www.geeksforgeeks.org/extract-k-bits-given-position-number/
int bitExtracted(int number, int k, int p)
{
    return (((1 << k) - 1) & (number >> (p - 1)));
}

// https://www.sanfoundry.com/c-program-convert-binary-hex/
// https://stackoverflow.com/questions/5307656/how-do-i-convert-a-binary-string-to-hex-using-c/5307868
void binaryToHex(char* hexadecimal, char* binary) {

    char *a = binary;
    int num = 0;
    do {
        int b = *a=='1'?1:0;
        num = (num<<1)|b;
        a++;
    } while (*a);
    printf("%X\n", num);
    sprintf(hexadecimal, "0x%X", num);
}

void get_bits(char* response, int num, int initial) {
    int i = 0;
    int j = num;
    unsigned int size = 1;
    while (j > 255) {
        size += 1;
        j /= 255;
    }
    unsigned int maxPow = 1<<(size*8-1);
    for ( ; i < size * 8; ++i) {
        // print last bit and shift left.
        // printf("%d %d %u\n", i, i - 1, num & maxPow ? 1 : 0);
        response[initial + i] = '0' + (num & maxPow ? 1 : 0);
        // int pnum = num & maxPow ? 1 : 0;
        // sprintf(response[size * 8 - i - 1], "%u", pnum);
        num = num << 1;
    }
    // printf("%s\n", response);
    // printf("\n%c %c %c %c %c %c %c %c\n", response[0], response[1], response[2], response[3], response[4], response[5], response[6], response[7]);
}

// https://parzibyte.me/blog/2018/11/19/convertir-binario-decimal-c/
int binarioADecimal(char *cadenaBinaria, int longitud) {
  int decimal = 0;
  int multiplicador = 1;
  char caracterActual;
  for (int i = longitud - 1; i >= 0; i--) {
    caracterActual = cadenaBinaria[i];
    if (caracterActual == '1') {
      decimal += multiplicador;
    }
    multiplicador = multiplicador * 2;
  }
  return decimal;
}