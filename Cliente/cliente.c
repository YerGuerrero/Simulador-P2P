#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX 10000

char* nombreRecurso;
char* dirIP;
char* puerto;
char* puertoTrans;
char* carpeta;

long long int primos[] = {1000000007, (1LL<<31)-1}; // (10^9)+7, primo mas grande que cabe en entero
//unsigned int le cabe (2^31)-1
long long int base = 257; //porque es mayor a 256


pthread_mutex_t mutex;
char request[MAX];
int numeroHilos;

int bytesRecibidos = 0;
int indice = 0;

long long expLogMod(int base, int exp, int p){
    if(exp==0){
        return 1LL;
    }else{
        long long int r = expLogMod(base, exp/2, p);
        r*= r; //elevo cuadrado
        r%= primos[p]; //primos en p
        if(exp%2){
            r*= base;//si el exponente era impar entonces multiplico por la base
            return r%primos[p]; //y vuelvo a modular
        }
    }
}

char* cantidadBytes(char *response){
    char copiaResponse[1024];
    strcpy(copiaResponse,response);
    char *ptr = strtok(copiaResponse, " ");
    ptr = strtok(NULL, " ");
    ptr = strtok(NULL, " ");
    ptr = strtok(NULL, " ");
    return ptr;
}

/*void solicitarInstruccion(int clienteSocket) {
    // COn scanf se pide la entrada por consola
    if(clienteSocket=="FIND"){
        snprintf(request, MAX, "%s%s%s","FIND /",nombreRecurso," HTTP/1.0 ");
    } else if(clienteSocket=="REQUEST"){
        snprintf(request, MAX, "%s%s%s","REQUEST /",nombreRecurso," HTTP/1.0 ");
    }
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
    return bytes;

}*/

//CON FUNC VIDEO PROFE
long long calcHash(char *s, int p){
    long long int r = 0;
    for(int i=0; s[i]; i++, r%=primos[p]){
        r+= s[i] * expLogMod(base,i,p);
    }
    return r;
}


int enviarInformacion(int clienteSocket){
    snprintf(request, MAX, "%s%s%s\n","SEND ",dirIP, puerto);
    char temp[MAX];
    DIR *folder;
    struct dirent *entry;
    int files = 0;
    char* nombreArchivo;
    nombreArchivo= carpeta;
    folder = opendir(carpeta);
    if(folder == NULL){
        perror("No se puede leer la carpeta");
        return 1;
    }
    while((entry=readdir(folder))){
        if(strcmp(entry->d_name,"..")!= 0 && strcmp(entry->d_name,".")!= 0){
            files++;
            strcat(nombreArchivo,"/");
            strcat(nombreArchivo,entry->d_name);
            struct stat estadoBuffer;
            int fd = open(nombreArchivo, O_RDONLY);
            fstat(fd, &estadoBuffer);
            snprintf(temp, MAX, "%s %ld %s %u\n",entry->d_name,estadoBuffer.st_size,"Hash",estadoBuffer.st_uid);
            strcat(request,temp);

        }
    }
    printf("\n%s\n",request);
    closedir(folder);
    //Mandarselo al servidor
    //write(clienteSocket, request, strlen(request));
    return 0;
}

int conectarServidor(){
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
    //solicitarInstruccion(socket_fd);
    return 0;
}

void *pthreadFunc(void *args){
    long thread_id = (long) args;
    pthread_mutex_lock(&mutex);
    printf("Se solicita recurso %li\n", thread_id);
    conectarServidor();
    pthread_mutex_unlock(&mutex);

}

void hiloCliente(){
    pthread_t threadID;

    if (pthread_create(&threadID, NULL, pthreadFunc,  (void *) threadID)){
        printf("Falla creación del hilo");
    }
    printf("Inicia el hilo\n");

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
    printf("%s\n",dirIP);
    printf("%s\n",puerto);
    printf("%s\n",puertoTrans);
    printf("%s\n",carpeta);
    hiloCliente();
}