#include "jagruz.h"
#include "../structs_shared/jugador.h"

JagRuz *jagruz_init()
{
    JagRuz *jagruz = malloc(sizeof(JagRuz));
    jagruz->initial_life = 10000;
    return jagruz;
}

int choose_hability_jagruz()
{
    // 1: ruzgar
    // 0: coletazo
    float random_num;

    random_num = rand() % 10;
    printf("%f\n", random_num);
    return random_num > 5;
}

void jagruz_clean(JagRuz *jagruz)
{
    free(jagruz);
};