#include "server.h"
// #include "../structs/monster.h"
#include "../structs/structs_server/conection.h"

Server* init_server(int socket) {
    Server* server = malloc(sizeof(Server));
    server -> clientes = malloc(sizeof(Jugador) * 5);
    server -> cantidad_clientes = 0;
    server -> socket = socket;
    server -> monster = malloc(sizeof(Monster));
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
  pthread_t tid[1];
  ArgumentsCreateThread* thrgs = malloc(sizeof(ArgumentsCreateThread));
  thrgs -> not_start = 1;
  
  char * welcome = "¡Bienvenido Cliente!\nEscriba su nombre:";
  Jugador* actual;
  while (thrgs -> not_start) {
    // Se definen las estructuras para almacenar info sobre los sockets de los clientes
    struct sockaddr_in client1_addr;
    socklen_t addr_size = sizeof(client1_addr);

    fd_set set;
    struct timeval timeout;
    int rv;
    FD_ZERO(&set); /* clear the set */
    FD_SET(server -> socket, &set); /* add our file descriptor to the set */

    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    rv = select(server -> socket + 1, &set, NULL, NULL, &timeout);
    if (rv == -1)
    {
        perror("select"); /* an error occurred */
    }
    else if (rv == 0)
    {
      // printf("timeout occurred (20 second) \n"); /* a timeout occurred */
    }
    else
    {
      new_socket = accept(server -> socket, (struct sockaddr *)&client1_addr, &addr_size);
    

      thrgs -> espera_cliente = 1;
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
      thrgs -> espera_cliente = 0;

      server -> cantidad_clientes += 1;

      // quiero que esto sea en paralelo
      if (pos == 0) {
        server -> lider = actual;
        thrgs -> socket = new_socket;
        if ( pthread_create(&tid[0], NULL, leader_start, thrgs) != 0 ) {
          printf("Failed to create thread\n");
        }
      }

      sprintf(response, "%s ha ingresado a la partida con la clase %d\n", actual -> nombre, actual -> num_clase);
      notify_all_clients(server, response);
    }

    if (thrgs -> not_start == 0) {
      printf("Termina la elección de clientes\n");
      break;
    }
  }
  printf("Empezará la partida\n");
  notify_all_clients(server, "¡Comenzará la partida!");
}

void * leader_start(void *args) {
  int msg_code;
  ArgumentsCreateThread * info = args;

  int op = 1;
  while (op ) {
    server_send_message(info -> socket, 4, "¿Quiere comenzar?");
    msg_code = server_receive_id(info -> socket);
    if (msg_code == 1) {
      char * client_message = server_receive_payload(info -> socket);
      printf("El cliente lider dice: %s\n", client_message);
      if (info -> espera_cliente == 1) {
          server_send_message(info -> socket, 4, "Hay un cliente registrándose.\n¿Quiere comenzar?");
      } else if (atoi(client_message) == 1) {
        op = 0;
        info -> not_start = 0;
        break;
      } else {
      }
    }
  }
  printf("Termine la función del lider\n");
  pthread_exit(NULL);
}

void start_playing(Server* server, Jugador* jugadores){
  // Se asume que recibe una lista de Jugadores 

  // Se instancia el mounstro
  server -> monster = Monster_init()

  // Le enviamos al primer cliente un mensaje de bienvenida 
  // char * welcome = "Bienvenido Cliente 1!!";
  // server_send_message(players_info->socket_c1, 1, welcome);


  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[2] = {players_info->sockets_array[0], players_info->socket_c2};

  int rounds = 1;
  while (1){
    for (int turn = 0; turn < server->cantidad_clientes; turn++)
    {
      // Juega cada jugador en orden de llegada
      // El jugadador 0 es el lider
      char * welcome = "Bienvenido Cliente !!";
      char * currentTurn = "Es el turno del jugador XX";
      // server_send_message(players_info->socket_c1, 1, welcome);
      notify_players(currentTurn);
      char * options = "Escoge una opcion\n1) Realizar una habilidad\n2) Rendirse\n";
      server_send_message(server -> clientes[turn] -> socket , turn, options);

      int msg_code = server_receive_id(sockets_array[turn]);
    }
    
    // Se obtiene el paquete del cliente 1
    int msg_code = server_receive_id(sockets_array[turn]);

    if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
    {
      char * client_message = server_receive_payload(sockets_array[turn]);
      printf("El cliente %d dice: %s\n", turn+1, client_message);

      // Le enviaremos el mismo mensaje invertido jeje
      char * response = revert(client_message);

      // Le enviamos la respuesta
      server_send_message(sockets_array[turn], 1, response);
    }
    else if (msg_code == 2){ //El cliente le envía un mensaje al otro cliente
      char * client_message = server_receive_payload(sockets_array[turn]);
      printf("Servidor traspasando desde %d a %d el mensaje: %s\n", turn+1, ((turn+1)%2)+1, client_message);

      // Mi atención cambia al otro socket
      turn = (turn + 1) % 2;

      server_send_message(sockets_array[turn], 2, client_message);
    }

    else if (msg_code == 0) {
      return 0;
    }
    printf("------------------\n");
    
  }
}

void notify_players(char* message){
  // Funcion que recibe un mensaje y lo envia a todos los jugadores
  for (int jugador = 0; jugador < server->cantidad_clientes; jugador++){
    server_send_message(server -> clientes[jugador] -> socket , jugador, message);
  }
}