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

void get_bits(char* response, int num) {
    int i=0;
    unsigned int size = sizeof(num) / sizeof(int);
    unsigned int maxPow = 1<<(size*8-1);
    for ( ; i < size * 8; ++i) {
        // print last bit and shift left.
        // printf("%d %d %u\n", i, size * 8 - i - 1, num & maxPow ? 1 : 0);
        response[i - 1] = '0' + (num & maxPow ? 1 : 0);
        // int pnum = num & maxPow ? 1 : 0;
        // sprintf(response[size * 8 - i - 1], "%u", pnum);
        num = num << 1;
    }
    // printf("%s\n", response);
    // printf("\n%c %c %c %c %c %c %c %c\n", response[0], response[1], response[2], response[3], response[4], response[5], response[6], response[7]);
}