#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#define PORT 8080
struct sockaddr_in sock_addr;

int sizeAddr = sizeof(sock_addr);
int clienteSocket;
int socket_fd;
char txt[1024]="Hola cliente";
int valread;


void crearArchivo(char* buff){
    //Se recibe los datos de los archivos
}

/*void getMetodo(){

        char buff[4096];
        memset(buff, 0, 4096);
        read(fd_client, buff, 4095);
        char *method = strtok(buff, " ");
        //char *filename = strtok(NULL, "/");
        //filename = strtok(filename, " ");

        if (strcmp(method, "SEND") == 0) {
            crearArchivo(buf);
            //get_method(fd_client, filename);
        }if (strcmp(method, "FIND") == 0) {
        //get_method(fd_client, filename);
        }

        close(fd_client);

    //Sacar protocolo SEND, FIND
    // SEND crea archivo
    // FIND buscar en el archivo y devuelve todos los que tiene ese nombre (toda la linea)
    //
    //Llamar a la función respectiva
}*/

int crearSocket(){
    //Se crea el socket
    socket_fd = socket(AF_INET,SOCK_STREAM,0); //Si el retorno es -1 ocurrió un error
    if( socket_fd == -1 ){
        printf("No se creó el socket :c\n");
        return -1;
    }
    return 0;
}

int enlazarSocket(){
    if( (bind(socket_fd, (struct sockaddr *) &sock_addr, sizeAddr)) < 0){
        close(socket_fd);
        printf("Error bind\n");
        return -1;
    }
    return 0;
}


int escucharCliente(){
    //Se llama a listen con el socket y el máximo de conexiones
    if( (listen(socket_fd,1000)) < 0 ){
        close(socket_fd);
        printf("Error listen\n");
        return -1;
    }else{
        printf("Escuchando...\n");
    }
    return 0;
}


int main (int argc, char const *argv[]){

    sock_addr.sin_family= AF_INET;
    sock_addr.sin_addr.s_addr= htonl(INADDR_ANY);
    sock_addr.sin_port = htons(PORT);

    crearSocket();
    enlazarSocket();
    escucharCliente();

    //Lee lo que viene del cliente
    char request[1024] = {0};
    while (1){
        if( (clienteSocket = accept(socket_fd, (struct sockaddr *) &sock_addr, (socklen_t*)&sizeAddr)) < 0){
            printf("Cliente no aceptado\n");
            return -1;
        }
        printf("Cliente aceptado\n");

       /* send(clienteSocket, txt, strlen(txt), 0);
        valread = read( clienteSocket , request, 1024);
        printf("%s\n",request);*/
    }
}
