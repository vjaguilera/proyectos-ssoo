#include "jugador.h"
#include "../structs_client/comunication.h"
#include "../structs_client/conection.h"
#include "../../helpers/input.h"

Jugador* init_jugador() {
    Jugador* jugador = malloc(sizeof(Jugador));
    jugador -> socket = socket;
    jugador -> mi_turno = 0;
    jugador -> rendido = 0;
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

