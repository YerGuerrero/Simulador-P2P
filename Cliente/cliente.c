#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>


char* nombreRecurso;
char* dirIP;
char* puerto;
char* puertoTrans;
char* carpeta;
char* request;

#define MAX 10000
pthread_mutex_t mutex;

int numeroHilos;

int bytesRecibidos = 0;
int indice = 0;


char* cantidadBytes(char *response){
    char copiaResponse[1024];
    strcpy(copiaResponse,response);
    char *ptr = strtok(copiaResponse, " ");
    ptr = strtok(NULL, " ");
    ptr = strtok(NULL, " ");
    ptr = strtok(NULL, " ");
    return ptr;
}

void solicitarInstruccion(int clienteSocket) {
    //Recibir si es un find o request
    // Si es find {
    // crea un request FIND nombre archivo
    // }
    // Si es request{
    // crea un request y lo envia a los clientes
    // REQUEST hash Tam
    // }
    /*char buffer[1024] = {0};
    char request[MAX];
    double bytes = 0;
    snprintf(request, MAX, "%s%s%s","GET /",nombreRecurso," HTTP/1.0 ");
    send(clienteSocket, request, sizeof(request), 0);
    read(clienteSocket , buffer, 1024);
    bytes = atoi(cantidadBytes(buffer));
    return bytes;*/

}

long calcularHash(file file){
    //Calcular el hash de un archivo
    //ver video del profe
    long n;
    return n
}

void enviarInformacion(int clienteSocket){
    snprintf(request, MAX, "%s%s%s","SEND ",dirIP, puerto);
    printf("%s",request);
    //for i in carpeta{
        //Llamar a calcular Hash con i
        // Meterle al request los datos de los archivos
    // }
    //write(clienteSocket, request, strlen(request));
    return;
}

int connectarServidor(){
    int socket_fd;
    struct sockaddr_in sock_addr;
    int sizeAddr = sizeof(sock_addr);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(dirIP);
    sock_addr.sin_port = htons(atoi(puerto));
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if( socket_fd < 0 ){
        printf("No se creó el socket :c\n");
        return -1;
    }
    if(inet_pton(AF_INET, dirIP, &sock_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if( connect(socket_fd, (struct sockaddr *) &sock_addr, sizeAddr) < 0 ){
        printf("Falla conexión\n");
    }
    enviarInformacion(socket_fd);
    solicitarInstruccion(socket_fd);
    return 0;
}

void *pthreadFunc(void *args){
    long thread_id = (long) args;
    pthread_mutex_lock(&mutex);
    printf("Se solicita recurso", thread_id, i);
    connectarServidor();
    pthread_mutex_unlock(&mutex);

}

void hiloCliente(){
    pthread_t threadID;

    if (pthread_create(&threadID, NULL, pthreadFunc, NULL){
        printf("Falla creación del hilo");
    }
    printf("Inicia el hilo\n", i);

    if (pthread_join(threadID, NULL) != 0) {
        printf("No se pudo hacer el join");
    }
    printf("El hilo ha terminado\n");
    pthread_mutex_destroy(&mutex);
}


int main(int argc, char *argv[]) {
    dirIP = argv[1];
    puerto = argv[2];
    puertoTrans = argv[3];
    carpeta = argv[4];
    hiloCliente();
    conectarServidor();

}

