#include "jagruz.h"

JagRuz *jagruz_init()
{
    JagRuz *jagruz = malloc(sizeof(JagRuz));
    jagruz->initial_life = 10000;
    return jagruz;
}

void ruzgar_hability(JagRuz *jagruz, Jugador *player)
{
    // El JagRuz ocupa sus poderosas garRaz para atacar a un enemigo 1000 puntos de daño"""
    int damage = 1000;
    update_player_life(player, -damage); // Update -damage player life
}

void coletazo_hability(JagRuz *jagruz, Jugador **players, int players_amount)
{
    int damage = 500;
    // El JagRuz golpea a todos los objetivos con su cola 500 puntos de daño
    for (int jg = 0; jg < players_amount; jg++)
    {
        update_player_life(players[jg], -damage); // Update -damage player life
    }
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