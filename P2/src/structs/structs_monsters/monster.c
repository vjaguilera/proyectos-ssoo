#include "monster.h"

Monster* monster_init(int identificador) {
    Monster* monster = malloc(sizeof(Monster));
    monster -> current_life = 10; // harcodeada
    monster -> estocadas = 0;
    monster -> me_distrajo = malloc(sizeof(Jugador));
    int num = identificador;
    monster -> salto_active = false;
    if (identificador == 4) {
        num = rand() % 3;
    }
    switch (num)
    {
    case 1:
        printf("Monstruo escogido: JagRuz\n");
        monster -> is_jagruz = 1;
        monster -> is_ruiz = 0;
        monster -> is_ruzalo = 0;
        monster -> jagruz = jagruz_init();
        monster -> initial_life = monster -> jagruz -> initial_life;
        monster -> class_str = "JagRuz";
        break;
    case 2:
        printf("Monstruo escogido: Ruiz\n");
        monster -> is_jagruz = 0;
        monster -> is_ruiz = 1;
        monster -> is_ruzalo = 0;
        monster -> ruiz = ruiz_init();
        monster -> initial_life = monster -> ruiz -> initial_life;
        monster -> class_str = "Ruiz";
        break;
    case 3:
        printf("Monstruo escogido: Ruzalo\n");
        monster -> is_jagruz = 0;
        monster -> is_ruiz = 0;
        monster -> is_ruzalo = 1;
        monster -> ruzalo = ruzalo_init();
        monster -> initial_life = monster -> ruzalo -> initial_life;
        monster -> class_str = "Ruzalo";
        break;
    
    default:
        break;
    }
    return monster;
}

void monster_clean(Monster* monster) {
    free(monster -> me_distrajo);
    free(monster -> jagruz);
    free(monster -> ruiz);
    free(monster -> ruzalo);
    free(monster);
}

void ruzgar_hability(Monster *jagruz, Jugador *player)
{
    // El JagRuz ocupa sus poderosas garRaz para atacar a un enemigo 1000 puntos de daño"""
    int damage = 1000;
    update_player_life(player, -damage); // Update -damage player life
}

void coletazo_hability(Monster *jagruz, Jugador **players, int players_amount)
{
    int damage = 500;
    // El JagRuz golpea a todos los objetivos con su cola 500 puntos de daño
    for (int jg = 0; jg < players_amount; jg++)
    {
        update_player_life(players[jg], -damage); // Update -damage player life
    }
}

void copycase_hability(Monster *ruiz, Jugador *copy_player, Jugador *obj_player)
{
    // Ruiz rompe el Código de Honor y copia de forma aleatoria una de las habilidades de
    // algún jugador y la usa contra alguno de los jugadores causando los efectos asociados. Si la habilidad normal-
    // mente involucra ayudar a un jugador aliado, Ruiz recibe el efecto sobre si mismo.
}

void reprobatron_hability(Monster *ruiz, Jugador *player)
{
    // Ruiz reprueba instantáneamente a un jugador. El jugador queda con estado repro-
    // bado hasta el fin del siguiente turno de Ruiz. Un jugador con estado reprobado está desmoralizado y recibe
    // 50 % de daño extra de todas las fuentes e inflige 50 % menos de puntos de vida con sus habilidades, ya sea al
    // curar o dañar

    // Champion desmoralization
    if (player -> is_cazador) {
        player -> demoralized = true;
    }

    // Add demoralized player to a list in Server
}

void salto_hability(Monster *ruzalo, Jugador *player)
{
    // El Ruzalos salta sobre su objetivo, cayendo con gran fuerza sobre el, este recibe 1500 puntos de
    // daño. Ruzalos no puede usar esta habilidad 2 o más veces seguidas, es decir, si usa esta habilidad, en su próximo
    // turno no puede usarla
    int damage = 1500;
    update_player_life(player, -damage); // Update -damage player life
    ruzalo->salto_active = true;
}

void espinavenenosa_hability(Monster *ruzalo, Jugador *player)
{
    // El Ruzalos golpea al objetivo con la espina en su cola intoxicándolo, recibiendo 400
    // puntos de daño cada turno (dura 3 turnos este efecto). Si el objetivo ya está intoxicado, inflije 500 puntos de
    // daño
    int damage = 500;
    // PENDIENTE
    if (player -> intoxicated)
    {
        // Si el objetivo se encontraba intoxicado, inflingir 500 de dano
        update_player_life(player, -damage); // Update -damage player life
    }
    // Intoxicar jugador
    int intoxication = 400;
    intoxicate_player(player, intoxication);
    ruzalo->salto_active = false;
}