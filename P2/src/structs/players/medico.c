#include "medico.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



Medico *medico_init(){
    Medico *medico = malloc(sizeof(Medico));
    medico -> initial_life = 3000;
    medico -> clase_str = "medico";
    return medico;
}


void curar_ability(Jugador *player_medico, Jugador *receptor){
// da 2000 de vida al jugador elegido
    if (receptor -> current_life > receptor -> initial_life - 2000){
        receptor -> current_life = receptor -> initial_life;
    }
    else {
        receptor -> current_life += 2000;
    }
}


void destello_regenerador_ability(Jugador *jugador_actual, Monster *monster, Jugador *jugador_recuperar){
// inflinge entre 750 y 2000 de daño al monstruo.
// elige un jugador al azar para darle la mitad del numero anterior como vida
    srand(time(0));
    int cantidad_ataque = (rand() % (2500 - 750 + 1)) + 750;
    if (jugador_actual-> duplicado != 0){
        if (monster -> current_life < 2 * cantidad_ataque){
            monster -> current_life = 0;
            // enviar señal de que el monstruo murio ? 
        }
        else {
            monster -> current_life -= 2 * cantidad_ataque;
        }
    }
    else{
        if (monster -> current_life < cantidad_ataque){
            monster -> current_life = 0;
            // enviar señal de que el monstruo murio ? 
        }
        else {
            monster -> current_life -= cantidad_ataque;    
        }
    }

    int vida_recuperada = (int)cantidad_ataque/2;

    if (jugador_recuperar -> current_life + vida_recuperada > jugador_recuperar -> initial_life){
        jugador_recuperar -> current_life = jugador_recuperar -> initial_life;
    }
    else{
        jugador_recuperar -> current_life += vida_recuperada;
    }
}



void descarga_vital_ability(Jugador *jugador, Monster *monster){
    int dano = 2 * (jugador -> initial_life - jugador -> current_life);
    if (jugador -> duplicado != 0){
        if (monster -> current_life <= 2 * dano){
            monster -> current_life = 0; 
        }
        else{
            monster -> current_life -= 2 * dano;
        }
    }
    else{
        if (monster -> current_life <= dano){
            monster -> current_life = 0; 
        }
        else{
            monster -> current_life -= dano;
        }
    }
}


void medico_clean(Medico *medico){
    free(medico);
};
