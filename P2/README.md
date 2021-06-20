# Proyecto 2

**Integrantes:**

- 16637860 | Vicente Aguilera
- 15624439 | Sebastian Calderon
- 16639707 | Tomas Castro
- 16635264 | Ignacio Contreras
- 15621278 | Jan Siegel

## Ejecución

```
./server -i 0.0.0.0 -p 8080
./client -i 0.0.0.0 -p 8080
```

## Comunicación

Para la conunicación se usó principalmente <sys/socket.h>, <netinet/in.h>, <sys/un.h>, <arpa/inet.h> y <netdb.h>. Estos paquetes en conjunto se utilizan para realizar la comunicación entre el servidor y los clientes. Se abre la conexión del servidor con un socket, se hace bind y listen. Luego para cada cliente se tiene un select que espera a que llegue un mensaje de conexión y si hay uno se hace recv para añadir el socket del cliente al servidor y comenzar a interactuar con él.

## Funciones

Principales funciones del servidor:
- start_playing: tiene todo el flujo de acciones entre los clientes y el monstruo
- initial_listen: es donde se realiza la conexión de los clientes con el servidor
- end_listen: es donde se pregunta a cada cliente si quiere continuar participando del juego

Principales funciones del cliente:
- listen_client: es donde se reciben y envian los mensajes del jugador, no hay lógica en el cliente.

## Diseño

Se definió una clase Jugador, Server y Monstruo. Además, se crearon las clases de cada tipo de JUgador y cada tipo de Monstruo. Luego de que cada cliente elige su clase se inicializa el tipo de jugador y se comparten la salud, el tipo de clase y las habilidades. Esto ocurre de forma similar para el monstruo, ya que una vez seleccionado se instancia y se comparten los atributos del tipo de monstruo a la instancia Monstruo del Server. El server es quien realiza toda la lógica de las habilidades y turnos y revisa cuando los jugadores están muertos o si el monstruo está muerto. Una vez que finaliza la batalla se llama a cada cliente para ver si quiere continuar jugando. En caso de que el líder no quiera jugar el siguiente usuario que llegó se define como líder. Si ninguno quiere jugar se cierran todas las conexiones. Un usuario que está muerto o rendido es notificado de las acciones que ocurran en el juego, pero si ya decidió salir del juego su conexión es eliminada.

## Bonus

Se hizo el bonus del loot conseguido. Una vez ganada la batalla se envian a través de paquetes las imágenes al cliente con la función send_loot.