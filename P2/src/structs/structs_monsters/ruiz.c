#include "ruiz.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../structs_shared/jugador.h"
#include "../structs_server/server.h"

Ruiz *ruiz_init()
{
    Ruiz *ruiz = malloc(sizeof(Ruiz));
    ruiz->initial_life = 25000;
    ruiz->duplicado = 0;
    return ruiz;
}

int choose_hability_ruiz()
{
    // 2: copycase
    // 1: reprobatron
    // 0: sudo rmrf
    float random_num;

    random_num = rand() % 10;
    printf("%f\n", random_num);

    if (random_num < 4)
    {
        // Copycase
        return 2;
    }
    else if (random_num >= 4 && random_num < 6)
    {
        // Reprobatron
        return 1;
    }
    else
    {
        // Sudo RM RF
        return 0;
    }
}

void ruiz_clean(Ruiz *ruiz)
{
    free(ruiz);
};