#include "jugador.h"
#include "../structs_client/comunication.h"
#include "../structs_client/conection.h"
#include "../../helpers/input.h"

Jugador* init_jugador(int identificador) {
    Jugador* jugador = malloc(sizeof(Jugador));
    jugador -> socket = socket;
    jugador -> mi_turno = 0;
    jugador -> rendido = 0;

    switch (identificador)
    {
    case 0:
        jugador -> is_cazador = 1;
        jugador -> is_medico = 0;
        jugador -> is_ruzalo = 0;
        jugador -> cazador = cazador_init();
        jugador -> current_life = jugador -> cazador-> initial_life;
        break;
    case 1:
        jugador -> is_cazador = 0;
        jugador -> is_medico = 1;
        jugador -> is_hacker = 0;
        jugador -> medico = medico_init();
        jugador -> current_life = jugador -> cazador-> initial_life;~
        break;
    case 2:
        jugador -> is_cazador = 0;
        jugador -> is_medico = 0;
        jugador -> is_hacker = 1;
        jugador -> hacker = hacker_init();
        jugador -> current_life = jugador -> cazador-> initial_life;
        break;
    
    default:
        printf("no deberia llegar aca!");
        break;
    }
    return jugador;
};

void set_socket(Jugador* jugador, int socket) {
    jugador -> socket = socket;
}

void set_name(Jugador* jugador, char* name) {
    jugador -> nombre = name;
}; 

void set_class(Jugador* jugador, int num_clase) {
    jugador -> num_clase = num_clase;
    jugador -> clase = 0; // deberia ser de enum
}

void listen_client(Jugador* jugador, int socket) {
    char * message;
    while (1){
        int msg_code = client_receive_id(socket);
        printf("Msg receive client: %d\n", msg_code);

        if (msg_code != 0)
        {
            message = client_receive_payload(socket);
            // printf("El servidor dice: %s\n", message);
        }

        if (msg_code == 1) {
            show_menu(message, 0);
            char * response = get_input();
            send_msg(jugador, 1, response);
        } else if (msg_code == 2) {
            show_menu(message, 0);
            show_menu("Cazador", 1);
            int option = pick_option();
            char response[2];
            sprintf(response, "%d", option);
            send_msg(jugador, 1, response);
        } else if (msg_code == 3) {
            // mensage general
            printf("%s\n", message);

        } else if (msg_code == 4) {
            show_menu(message, 0);
            show_menu("Si", 1);
            show_menu("No", 2);
            int option = pick_option();
            char response[2];
            sprintf(response, "%d", option);
            send_msg(jugador, 1, response);
        }



        else if (msg_code == 5) {
            show_menu(message, 0);
            show_menu("Ataque 1", 1);
            show_menu("Ataque 2", 2);
            int option = pick_option();
            char msg[2];
            sprintf(msg, "%d", option);
            send_msg(jugador, option, msg);
        }

        else if (msg_code == 7) {
            show_menu(message, 0);
            int option = pick_option();
            char msg[2];
            sprintf(msg, "%d", option);
            send_msg(jugador, option, msg);
        }
        
        
        else if (msg_code == 10 || msg_code == 20) 
        {   
            show_menu("¿Qué desea hacer?", 0);
            show_menu("Enviar mensaje al servidor", 1);
            show_menu("Enviar mensaje al otro cliente", 2);
            int option = pick_option();

            printf("Ingrese su mensaje: ");
            char * response = get_input();
            send_msg(jugador, option, response);
        } else {
            printf("Msg code %d no procesado\n", msg_code);
            if (msg_code != 0) {
                free(message);
            }
            break;
        }

        printf("------------------\n");
    }
    close(jugador -> socket);
}

void send_msg(Jugador* jugador, int option, char* msg) {
    client_send_message(jugador -> socket, option, msg);
}

void show_menu(char* text, int option) {
    if (option == 0) {
        printf("%s\n", text);
    } else {
        printf("%d: %s\n", option, text);
    }
}

int pick_option() {
    int option = getchar() - '0';
    getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin
    return option;
}

void clean_jugador(Jugador* jugador) {
    free(jugador);
}

void update_player_life(Jugador* jugador, int damage) {
    printf("Quitar %d de vida a jugador %s\n", damage, jugador -> nombre);
    switch (jugador -> num_clase)
    {
    case 0:
        // jugador -> cazador -> life -= damage;
        break;
    case 1:
        // jugador -> medico -> life -= damage;
        break;
    case 2:
        // jugador -> hacker -> life -= damage;
        break;
    
    default:
        break;
    }   
}

void intoxicate_player(Jugador* jugador, int intoxication) {
        printf("Quitar %d de vida a jugador %s\n", intoxication, jugador -> nombre);
    switch (jugador -> num_clase)
    {
    case 0:
        // jugador -> cazador -> intoxicated -= damage;
        break;
    case 1:
        // jugador -> medico -> intoxicated -= damage;
        break;
    case 2:
        // jugador -> hacker -> intoxicated -= damage;
        break;
    
    default:
        break;
    }  
};