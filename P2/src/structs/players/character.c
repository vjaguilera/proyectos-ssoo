#include "character.h"

void set_class(Jugador* jugador, int num_clase) {
    jugador -> num_clase = num_clase;
    switch (num_clase)
    {
    case 0:

        jugador -> initial_life = cazador_init() -> initial_life;
        jugador -> current_life = cazador_init() -> initial_life;
        jugador -> clase_str = cazador_init() -> clase_str;
        break;
    case 1:
        jugador -> initial_life = hacker_init() -> initial_life;
        jugador -> current_life = hacker_init() -> initial_life;
        jugador -> clase_str = hacker_init() -> clase_str;
        break;
    case 2:
        jugador -> initial_life = medico_init() -> initial_life;
        jugador -> current_life = medico_init() -> initial_life;
        jugador -> clase_str = medico_init() -> clase_str;
        break;
    
    default:
        break;
    }
}