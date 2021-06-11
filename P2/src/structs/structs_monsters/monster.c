#include "monster.h"

Monster* monster_init(int identificador) {
    Monster* monster = malloc(sizeof(Monster));
    monster -> current_life = 10; // harcodeada
    monster -> estocadas = 0;
    monster -> me_distrajo = malloc(sizeof(Jugador));
    switch (identificador)
    {
    case 0:
        monster -> is_jagruz = 1;
        monster -> is_ruiz = 0;
        monster -> is_ruzalo = 0;
        monster -> jagruz = jagruz_init();
        monster -> initial_life = monster -> jagruz -> initial_life;
        break;
    case 1:
        monster -> is_jagruz = 0;
        monster -> is_ruiz = 1;
        monster -> is_ruzalo = 0;
        monster -> ruiz = ruiz_init();
        monster -> initial_life = monster -> ruiz -> initial_life;
        break;
    case 2:
        monster -> is_jagruz = 0;
        monster -> is_ruiz = 0;
        monster -> is_ruzalo = 1;
        monster -> ruzalo = ruzalo_init();
        monster -> initial_life = monster -> ruzalo -> initial_life;
        break;
    
    default:
        printf("no deberia llegar aca!");
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