#include "ruiz.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Ruiz *Ruiz_init()
{
    Ruiz *ruiz = malloc(sizeof(Ruiz));
    ruiz->initial_life = 25000;
    return ruiz;
}

void copycase_hability(Ruiz *ruiz, Player *copy_player, Player *obj_player)
{
    // Ruiz rompe el Código de Honor y copia de forma aleatoria una de las habilidades de
    // algún jugador y la usa contra alguno de los jugadores causando los efectos asociados. Si la habilidad normal-
    // mente involucra ayudar a un jugador aliado, Ruiz recibe el efecto sobre si mismo.
}

void reprobatron_hability(Ruiz *ruiz, Player *player)
{
    // Ruiz reprueba instantáneamente a un jugador. El jugador queda con estado repro-
    // bado hasta el fin del siguiente turno de Ruiz. Un jugador con estado reprobado está desmoralizado y recibe
    // 50 % de daño extra de todas las fuentes e inflige 50 % menos de puntos de vida con sus habilidades, ya sea al
    // curar o dañar

    // Champion desmoralization
    player->champion->demoralized = true;

    // Add demoralized player to a list in Server
}

void sudormrf_hability(Ruiz *ruiz, Server *server, Player **players, int players_amount)
{
    // Ruiz borra todas las rondas ocurridas hasta ahora para infligir daño a todos los jugadores.
    // Hace 100·(número de rondas desde el inicio del combate hasta ahora, sin considerar usos anteriores de esta
    // habilidad) de daño a todos los jugadores.

    // Get rounds until now and get damage
    int rounds = server->rouds_without_sudo; // Rounds from the beginning til now without usage of sudo rmrf

    int damage = 100 * rounds;

    // Update players life
    for (int jg = 0; jg < players_amount; jg++)
    {
        update_player_life(players[jg], -damage); // Update -damage player life
    }

    // Set server rounds to 0
    server->current_round = 0;
}

int choose_hability()
{
    // 2: copycase
    // 1: reprobatron
    // 0: sudo rmrf
    float random_num;

    random_num = ((float)rand()) / RAND_MAX;
    printf("%f\n", random_num);

    if (random_num < 0.4)
    {
        // Copycase
        return 2;
    }
    else if (random_num >= 0.4 && random_num < 0.6)
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