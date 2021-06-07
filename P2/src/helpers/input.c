#include "input.h"
#include <stdlib.h>
#include <stdio.h>

char * get_input(){
    char * response = malloc(20);
    int pos=0;
    while (1){
        char c = getchar();
        if (c == '\n') break;
        response[pos] = c;
        pos++;
    }
    response[pos] = '\0';
    return response;
}