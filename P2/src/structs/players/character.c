#include "character.h"

void set_class(Jugador* jugador, int num_clase) {
    jugador -> num_clase = num_clase;
    switch (num_clase)
    {
    case 1:
        printf("Usuario es Cazador\n");
        Cazador* cazador = cazador_init();
        jugador -> initial_life =  cazador -> initial_life;
        jugador -> current_life = cazador -> initial_life;
        jugador -> clase_str = cazador -> clase_str;
        jugador -> is_cazador = 1;
        jugador -> is_medico = 0;
        jugador -> is_hacker = 0;
        cazador_clean(cazador);
        break;
    case 2:
        printf("Usuario es Hacker\n");
        Hacker* hacker = hacker_init();
        jugador -> initial_life = hacker -> initial_life;
        jugador -> current_life = hacker -> initial_life;
        jugador -> clase_str = hacker -> clase_str;
        jugador -> is_cazador = 0;
        jugador -> is_medico = 0;
        jugador -> is_hacker = 1;
        hacker_clean(hacker);
        break;
    case 3:
        printf("Usuario es Médico\n");
        Medico* medico = medico_init();
        jugador -> initial_life = medico -> initial_life;
        jugador -> current_life = medico -> initial_life;
        jugador -> clase_str = medico -> clase_str;
        jugador -> is_cazador = 0;
        jugador -> is_medico = 1;
        jugador -> is_hacker = 0;
        medico_clean(medico);
        break;
    
    default:
        break;
    }
}