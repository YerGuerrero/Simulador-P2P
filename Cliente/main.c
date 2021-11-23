#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

int socket_fd;
char requestText[1024]="Hola";
int valread;
struct sockaddr_in sock_addr;
char txt[1024]="Hola Servidor";
int sizeAddr = sizeof(sock_addr);
pthread_t threads;


void armarArchivo(){

}

void find(){

}

void compartirInfo(){

}

void request(){

}

void *cicloRequest(void *args){
    printf("Holi");
    char buffer[1024] = {0};
    send(socket_fd , txt, sizeof(txt), 0 );
    valread = read( socket_fd , buffer, 1024);
    printf(" %s\n\n",buffer);

}

void crearHilosCliente(){
    pthread_create(&threads, NULL, cicloRequest,NULL);
}

int main(int argc, char *argv[]){

    char *dirIP = argv[1];
    int puerto = atoi(argv[2]);
    //int puerto_trans = atoi(argv[3]);
    //char *carpeta= argv[4];

    sock_addr.sin_family= AF_INET;
    sock_addr.sin_port = htons(8080);

    socket_fd = socket(AF_INET,SOCK_STREAM,0); //Si el retorno es -1 ocurrió un error
    if( socket_fd == -1 ){
        printf("No se creó el socket :c");
        return -1;
    }

    if(inet_pton(AF_INET, "127.0.0.1", &sock_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if( connect(socket_fd, (struct sockaddr *) &sock_addr, sizeAddr) < 0 ){
        printf("Falla conexión\n");
    }
    crearHilosCliente();

    pthread_join(threads,NULL);
    return 0;
}