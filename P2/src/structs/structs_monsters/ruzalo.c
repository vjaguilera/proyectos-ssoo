#include "ruzalo.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../structs_shared/jugador.h"

Ruzalo *ruzalo_init()
{
    Ruzalo *ruzalo = malloc(sizeof(Ruzalo));
    ruzalo->initial_life = 20000;
    ruzalo->salto_active = false;
    return ruzalo;
}

int choose_hability_ruzalo()
{
    // 1: salto
    // 0: espinavenenosa
    float random_num;
    
    random_num = rand() % 10;
    printf("%f\n", random_num);
    return random_num > 4;
}
void ruzalo_clean(Ruzalo *ruzalo)
{
    free(ruzalo);
};