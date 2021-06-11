#include "ruzalo.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Ruzalo *ruzalo_init()
{
    Ruzalo *ruzalo = malloc(sizeof(Ruzalo));
    ruzalo->initial_life = 20000;
    ruzalo->salto_active = false;
    return ruzalo;
}

void salto_hability(Ruzalo *ruzalo, Jugador *player)
{
    // El Ruzalos salta sobre su objetivo, cayendo con gran fuerza sobre el, este recibe 1500 puntos de
    // daño. Ruzalos no puede usar esta habilidad 2 o más veces seguidas, es decir, si usa esta habilidad, en su próximo
    // turno no puede usarla
    int damage = 1500;
    update_player_life(player, -damage); // Update -damage player life
    ruzalo->salto_active = true;
}

void espinavenenosa_hability(Ruzalo *ruzalo, Jugador *player)
{
    // El Ruzalos golpea al objetivo con la espina en su cola intoxicándolo, recibiendo 400
    // puntos de daño cada turno (dura 3 turnos este efecto). Si el objetivo ya está intoxicado, inflije 500 puntos de
    // daño
    int damage = 500;
    // PENDIENTE
    // if (player->champion->intoxicated)
    // {
        // // Si el objetivo se encontraba intoxicado, inflingir 500 de dano
        // update_player_life(player, -damage); // Update -damage player life
    // }
    // Intoxicar jugador
    int intoxication = 400;
    intoxicate_player(player, intoxication);
    ruzalo->salto_active = false;
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