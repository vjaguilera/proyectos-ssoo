#include "server.h"

Server* init_server(int socket) {
    Server* server = malloc(sizeof(Server));
    server -> clientes = malloc(sizeof(Jugador) * 5);
    server -> cantidad_clientes = 0;
    server -> socket = socket;
    return server;
}

void notify_all_clients(Server* server, char* msg) {
    for (int i = 0; i < server -> cantidad_clientes; i++) {
        server_send_message(server -> clientes[i] -> socket, 3, msg);
    }
}

void initial_listen(Server* server) {
    int new_socket;
  int pos;
  int msg_code;
  char response[50];
  int espera_cliente = 0;
  int not_start = 1;

  char * welcome = "¡Bienvenido Cliente!\nEscriba su nombre:";
  Jugador* actual;
  while (1) {
    // Se definen las estructuras para almacenar info sobre los sockets de los clientes
    struct sockaddr_in client1_addr;
    socklen_t addr_size = sizeof(client1_addr);
    new_socket = accept(server -> socket, (struct sockaddr *)&client1_addr, &addr_size);
    espera_cliente = 1;
    printf("Enviamos mensaje bienvenida a %d\n", new_socket);
    pos = server -> cantidad_clientes;
    server -> clientes[server -> cantidad_clientes] = init_jugador();
    actual = server -> clientes[server -> cantidad_clientes];
    set_socket(actual, new_socket);
    
    // ask for name
    printf("Mandar a %d\n", actual -> socket);
    server_send_message(actual -> socket, 1, welcome);
    msg_code = server_receive_id(new_socket);
    if (msg_code == 1) {
      char * client_message = server_receive_payload(new_socket);
      printf("El cliente %d dice: %s\n", server -> cantidad_clientes, client_message);
      actual -> nombre = client_message;

    }
    server_send_message(new_socket, 2, "Seleccione su clase");
    msg_code = server_receive_id(new_socket);
    if (msg_code == 1) {
      char * client_message = server_receive_payload(new_socket);
      printf("El cliente %d dice: %s\n", server -> cantidad_clientes, client_message);
      actual -> num_clase = atoi(client_message);
    }
    espera_cliente = 0;

    server -> cantidad_clientes += 1;

    // quiero que esto sea en paralelo
    if (pos == 0) {
      server -> lider = actual;
      while (not_start) {
        server_send_message(new_socket, 4, "¿Quiere comenzar?");
        msg_code = server_receive_id(new_socket);
        if (msg_code == 1) {
          char * client_message = server_receive_payload(new_socket);
          printf("El cliente %d dice: %s\n", server -> cantidad_clientes, client_message);
          if (espera_cliente == 1) {
              server_send_message(new_socket, 4, "Hay un cliente registrándose.\n¿Quiere comenzar?");
          } else if (atoi(client_message) == 1) {
            not_start = 0;
            printf("Empezará la partida\n");
            notify_all_clients(server, "¡Comenzará la partida!");
          }
        }
      }
    }

    sprintf(response, "%s ha ingresado a la partida con la clase %d\n", actual -> nombre, actual -> num_clase);
    notify_all_clients(server, response);

    if (server -> cantidad_clientes == 3) {
      printf("Termina la elección de clientes\n");
      break;
    }
  }
}