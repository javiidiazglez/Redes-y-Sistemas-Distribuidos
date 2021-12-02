//****************************************************************************
//                         REDES Y SISTEMAS DISTRIBUIDOS
//
//                     2º de grado de Ingeniería Informática
//
//                        Main class of the FTP server
//
//****************************************************************************

#include <cerrno>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

 #include <unistd.h>


#include <pthread.h>

#include <list>

#include "common.h"
#include "FTPServer.h"
#include "ClientConnection.h"


int define_socket_TCP(int port) {
  // ---- Include the code for defining the socket. ---------

    struct sockaddr_in sin; //Estructura de memoria
    int s;

    s = socket(AF_INET, SOCK_STREAM, 0); //Creo un int socket, con la familia de protocolos AF_INET, de tipo SOCK_STREAM, y el protocolo que se utiliza

    if(s < 0){
        errexit("No puedo crear el socket: %s\n", strerror(errno));
    }

    memset(&sin, 0, sizeof(sin)); //memset(char *buffer, char v, size.t tam)
    // Lo que hace el memset es rellenar toda la estructura con el valor que le pasas en nuestro caso 0. Con esto limpias la estructura 
    // y le puedes dar los parametros que tu quieras.
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        errexit("No puedo hacer el bind con el puerto: %s\n", strerror(errno));
    }

    /*
    A la función bind le paso s que es el descriptor del socket, sin el puntero a la estructura con la dirección y sizeof(sin) el tamaño de la estructura.
    El bind lo unico que hace es asociar la dirección que en este caso es cualquier y el puerto al socket. Le asigna una dirección
    */
    if (listen(s, 5) < 0) {
    errexit("Fallo en el listen: %s\n", strerror(errno));
    }

   return s;
}


// This function is executed when the thread is executed.
void* run_client_connection(void *c) {
    ClientConnection *connection = (ClientConnection *)c;
    connection->WaitForRequests();

    return NULL;
}

FTPServer::FTPServer(int port) {
    this->port = port;

}

// Parada del servidor.
void FTPServer::stop() {
    close(msock);
    shutdown(msock, SHUT_RDWR);

}

// Starting of the server
void FTPServer::run() {

    struct sockaddr_in fsin;
    int ssock;
    socklen_t alen = sizeof(fsin);
    msock = define_socket_TCP(port);  // This function must be implemented by you.
    while (1) {
	pthread_t thread;
        ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
        if(ssock < 0)
            errexit("Fallo en el accept: %s\n", strerror(errno));

	ClientConnection *connection = new ClientConnection(ssock);

	// Here a thread is created in order to process multiple
	// requests simultaneously
	pthread_create(&thread, NULL, run_client_connection, (void*)connection);

    }

}
