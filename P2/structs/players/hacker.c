#include "hacker.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



Hacker *hacker_init()
{
    Hacker *hacker = malloc(sizeof(Hacker));
    hacker -> initial_life = 2500;
    hacker -> current_life = 2500;
    hacker -> duplicado = 0;
    hacker -> fuerza_bruta = 0;
    return hacker;
}


void inyeccion_sql_ability(Player *player){
// duplica el ataque de otro jugador por 2 turnos
// lo que hace ahora es agregar dos turnos en el cual se multiplica el daño que ejerce el jugador
// hay una issue sobre algo que no esta especificado, y le dicen que como no lo dice, se agregue al readme la explicacion
// podriamos hacerlo con este metodo, y asi no hay que cambiar esto tampoco
// si no, hay que cambiar la manera en la que se piensa el duplicado, ya que ahora no se acumulan duplicaciones
    player -> duplicado += 2;
}


void ataque_ddos_ability(Hacker *hacker, Server *server){
// inflinge 3000 de daño al monstruo
    if (hacker -> duplicado != 0){
        if (server -> monster -> current_life > 3000){
            server -> monster -> current_life -= 3000;   
        }
        else {
            server -> monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        }
    }
    else{
        if (server -> monster -> current_life > 1500){
            server -> monster -> current_life -= 1500;   
        }
        else {
            server -> monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        }
    }

}


void fuerza_bruta_ability(Hacker *hacker, Server *server){
    hacker -> fuerza_bruta += 1;
    if (hacker->fuerza_bruta == 3){
        if (hacker->duplicado != 0){
            if (server -> monster -> current_life > 20000){
                server -> monster -> current_life -= 20000;
            }
            else{
                server -> monster -> current_life = 0; // avisar muerte del monstruo ? 
            }
        }
        else{
            if (server -> monster -> current_life > 10000){
                server -> monster -> current_life -= 10000;
            }
            else{
                server -> monster -> current_life = 0; // avisar muerte del monstruo ? 
            }
        }
        hacker -> fuerza_bruta = 0;
    }
}


void hacker_clean(Hacker *hacker)
{
    free(hacker);
};