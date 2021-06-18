#include "character.h"

void set_class(Jugador* jugador, int num_clase) {
    jugador -> num_clase = num_clase;
    switch (num_clase)
    {
    case 1:
        printf("Usuario es Cazador\n");
        jugador -> initial_life = cazador_init() -> initial_life;
        jugador -> current_life = cazador_init() -> initial_life;
        jugador -> clase_str = cazador_init() -> clase_str;
        jugador -> is_cazador = 1;
        jugador -> is_medico = 0;
        jugador -> is_hacker = 0;
        break;
    case 2:
        printf("Usuario es Hacker\n");
        jugador -> initial_life = hacker_init() -> initial_life;
        jugador -> current_life = hacker_init() -> initial_life;
        jugador -> clase_str = hacker_init() -> clase_str;
        jugador -> is_cazador = 0;
        jugador -> is_medico = 0;
        jugador -> is_hacker = 1;
        break;
    case 3:
        printf("Usuario es Médico\n");
        jugador -> initial_life = medico_init() -> initial_life;
        jugador -> current_life = medico_init() -> initial_life;
        jugador -> clase_str = medico_init() -> clase_str;
        jugador -> is_cazador = 0;
        jugador -> is_medico = 1;
        jugador -> is_hacker = 0;
        break;
    
    default:
        break;
    }
    jugador -> mi_turno = 0;
    jugador -> rendido = 0;
    jugador -> duplicado = 0;
    jugador -> fuerza_bruta = 0;
    jugador -> intoxicated = 0;
    jugador -> demoralized = false;
    jugador -> estocadas = 0;
    jugador -> deleted = 0;
}