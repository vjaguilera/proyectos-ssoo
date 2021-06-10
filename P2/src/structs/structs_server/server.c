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

// [?] jugadores seria server -> clients
void start_playing(Server* server, Jugador* jugadores){
  // [?] Se asume que recibe una lista de Jugadores 

  // [?] Se instancia el mounstro
  server -> monster = Monster_init();
  int cantidad_activos = server->cantidad_clientes;
  int rounds = 1;

  // WHILE 
  while (server->monster->vida > 0 && cantidad_activos > 0){

    // FOR
    for (int turn = 0; turn < server->cantidad_clientes; turn++){

      if (server -> clientes[turn] -> rendido){
        continue;
      }

      // Juega cada jugador en orden de llegada
      // El jugadador 0 es el lider
      char * currentTurnMessage;
      sprintf(currentTurnMessage, "Es el turno del jugador %i\n", turn);
      notify_players(currentTurnMessage);

      char * options = "Escoge una opcion\n1) Realizar una habilidad\n2) Rendirse\n";

      // [?] Que es el id que se entrega, el segundo parametro
      server_send_message(server -> clientes[turn] -> socket , turn, options);

      // [?] El servidor espera respuesta del cliente actual
      int opc_code = server_receive_id(sockets_array[turn]);
      // El cliente responde esto
      char * client_response = server_receive_payload(sockets_array[turn]);
      // Manejo de ifs
      if (client_response == "1"){
        char * optionsPlay;
        // [?] Como acceder a las habilidades de la clase del jugador actual
        sprintf(optionsPlay, "Escoge tu habilidad\n1)%s\n2)%s\n3)%s\n",
          server -> clientes[turn] -> clase -> habilidades[0],
          server -> clientes[turn] -> clase -> habilidades[1],
          server -> clientes[turn] -> clase -> habilidades[2]);
        // [?] Que es el id que se entrega, el segundo parametro
        server_send_message(server -> clientes[turn] -> socket , turn, optionsPlay);
        char * client_response_ability = server_receive_payload(sockets_array[turn]);
        // ver como se comporta la habilidad
        // [ToDo] notify_players de la habilidad realizada (jugador, habilidad, consecuencia, target, turno)
        if (server -> clientes[turn] -> clase -> clase_str == "hacker"){
          if (client_response_ability == 1){
            char * optionsSQL = "Escoge a quien quieres duplicarle el ataque\n";
            int opcion = 0;
            // [?] Como acceder a las habilidades de la clase del jugador actual
            for (int i = 0; i < server -> cantidad_clientes; i++){
              if (server ->clientes[i] -> rendido){
                opcion += 1;
                continue;
              }
              else{
                sprintf(optionsSQL, "%i) %s\n", opcion, server->clientes[i]->nombre);
                opcion += 1;
              }
            }
            // [?] Que es el id que se entrega, el segundo parametro
            server_send_message(server -> clientes[turn] -> socket , turn, optionsSQL);
            char * client_response_sql = server_receive_payload(sockets_array[turn]);
            // [?] Se castea asi a int? o con atoi
            int cliente_a_duplicar = (int)client_response_sql;

            inyeccion_sql_ability(server -> clientes[turn], server->clientes[cliente_a_duplicar]);
          } 
          else if (client_response_ability == 2){
            ataque_ddos_ability(server -> clientes[turn], server -> monster);
          }
          else if (client_response_ability == 3){
            fuerza_bruta_ability(server -> clientes[turn], server -> monster);
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }
        else if (server -> clientes[turn] -> clase -> clase_str == "medico"){
          if (client_response_ability == 1){
            // A quien quieres curar
          char * optionsCure = "Escoge a quien quieres curar\n";
          int opcion = 0;
          // [?] Como acceder a las habilidades de la clase del jugador actual
          for (int i = 0; i < server -> cantidad_clientes; i++){
            if (server ->clientes[i] -> rendido){
              opcion += 1;
              continue;
            }
            else{
              sprintf(optionsCure, "%i) %s\n", opcion, server->clientes[i]->nombre);
              opcion += 1;
            }
          }
          // [?] Que es el id que se entrega, el segundo parametro
          server_send_message(server -> clientes[turn] -> socket , turn, optionsCure);
          char * client_response_cure = server_receive_payload(sockets_array[turn]);
          // [?] Se castea asi a int? o con atoi
          int cliente_a_curar = (int)client_response_cure;
          curar_ability(server -> clientes[turn], server -> clientes[cliente_a_curar]);
          } 
          else if (client_response_ability == 2){
            // Asignar target_recuperacion al azar
            int jugador_valido = 1;
            int jugador_recuperado;
            while(jugador_valido){
              jugador_recuperado = (rand() % cantidad_jugadores + 1);
              if (server -> clientes[jugador_recuperado] -> rendido){
                continue;
              }
              else{
                jugador_valido = 0;
              }
            }
            destello_regenerador_ability(server -> clientes[turn], server -> monster, server -> clientes[jugador_recuperado]);
          }
          else if (client_response_ability == 3){
            descarga_vital_ability(server -> clientes[turn], server -> monster);
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }
        else if (server -> clientes[turn] -> clase -> clase_str == "cazador"){
          if (client_response_ability == 1){
            estocada_ability(server -> clientes[turn], server -> monster);
          } 
          else if (client_response_ability == 2){
            corte_cruzado_ability(server -> clientes[turn], server -> monster);
          }
          else if (client_response_ability == 3){
            distraer_ability(server -> clientes[turn], server -> monster);
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }

      }
      else if (client_response == "2"){
        // server -> cantidad_clientes -= 1;
        server -> clientes[turn] -> rendido = 1;
        char * playerSurrender;
        sprintf(playerSurrender, "El jugador %i se rindio\n", turn);
        notify_players(playerSurrender);
      }
    }
    // FIN DEL FOR DE JUGADORES


    // [vicho] Aqui juega el moster

    // FIN DEL MONSTER

    // Calculamos nueva cantidad de activos
    int cantidad_activos_aux = 0;
    for (int i = 0; i < server->cantidad_clientes; i++){
      if (server->clientes[i]->rendido){
        continue;
      }
      else{
        cantidad_activos_aux += 1;
      }
    }
    int cantidad_activos = cantidad_activos_aux;
    rounds += 1;
  }
  // FIN DEL WHILE

  // GANAMOS 
  if (server->monster->vida <= 0){
    char * winMessage = "El monstruo ha sido derrotado, FELIICITACIONES A LOS JUGADORES\n";  
    notify_players(winMessage);
  }
  // PERDIMOS :(
  else{
    // [tomas] si pierden la vida, que se dejen como rendido = 1
    char * lossMessage = "El monstruo nos ha derrotado\n";
    notify_players(lossMessage);
  }
}

void notify_players(char* message){
  // Funcion que recibe un mensaje y lo envia a todos los jugadores
  for (int jugador = 0; jugador < server->cantidad_clientes; jugador++){
    server_send_message(server -> clientes[jugador] -> socket , jugador, message);
  }
}

void turnos_listen(Server* server) {
  int my_attention = 0;
  server -> turno_actual = my_attention;
  server -> active_match = 1;
  char message[70];
  while (server -> active_match)
  {

    printf("Turno de %d\n", server -> turno_actual);
    server -> cliente_actual = server -> clientes[server -> turno_actual];
    printf("Nombre: %s\n", server -> cliente_actual -> nombre);
    sprintf(message, "Es el turno de %s", server -> cliente_actual -> nombre);
    notify_all_clients(server, message);
    server_send_message(server -> cliente_actual -> socket, 5, "¿Qué desea hacer?");
    // Se obtiene el paquete del cliente 1
    int msg_code = server_receive_id(server -> cliente_actual -> socket);
    char * client_message = server_receive_payload(server -> cliente_actual -> socket);
    printf("Se recibe %d: %s\n", msg_code, client_message);
    printf("------------------\n");
    change_turn(server);
  }
}

void change_turn(Server* server) {
  printf("-Turno de %d\n", server -> turno_actual);
  server -> turno_actual += 1;
  if (server -> turno_actual > 4) {
    server -> turno_actual = 0;
  }
  printf("--Turno de %d\n", server -> turno_actual);
  // otras cosas de combates etc
}