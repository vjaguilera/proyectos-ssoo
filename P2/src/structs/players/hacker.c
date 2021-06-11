#include "hacker.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



Hacker *hacker_init()
{
    Hacker *hacker = malloc(sizeof(Hacker));
    hacker -> initial_life = 2500;
    hacker -> clase_str = "hacker";
    return hacker;
}


void inyeccion_sql_ability(Jugador *jugador_origen, Jugador *jugador_destino){
// duplica el ataque de otro jugador por 2 turnos
// lo que hace ahora es agregar dos turnos en el cual se multiplica el daño que ejerce el jugador
// hay una issue sobre algo que no esta especificado, y le dicen que como no lo dice, se agregue al readme la explicacion
// podriamos hacerlo con este metodo, y asi no hay que cambiar esto tampoco
// si no, hay que cambiar la manera en la que se piensa el duplicado, ya que ahora no se acumulan duplicaciones
    jugador_destino -> duplicado += 2;
}


void ataque_ddos_ability(Jugador *jugador, Monster *monster){
// inflinge 3000 de daño al monstruo
    if (jugador -> duplicado != 0){
        if (monster -> current_life > 3000){
            monster -> current_life -= 3000;   
        }
        else {
            monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        }
    }
    else{
        if (monster -> current_life > 1500){
            monster -> current_life -= 1500;   
        }
        else {
            monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        }
    }

}


void fuerza_bruta_ability(Jugador *jugador, Monster *monster){
    jugador-> fuerza_bruta += 1;
    if (jugador->fuerza_bruta == 3){
        if (jugador->duplicado != 0){
            if (monster -> current_life > 20000){
                monster -> current_life -= 20000;
            }
            else{
                monster -> current_life = 0; // avisar muerte del monstruo ? 
            }
        }
        else{
            if (monster -> current_life > 10000){
                monster -> current_life -= 10000;
            }
            else{
                monster -> current_life = 0; // avisar muerte del monstruo ? 
            }
        }
        jugador -> fuerza_bruta = 0;
    }
}


void hacker_clean(Hacker *hacker)
{
    free(hacker);
};