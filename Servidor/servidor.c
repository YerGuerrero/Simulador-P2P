#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
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
#define MAX 1000
char request[MAX];


char* obtenerDato(char* request, int pos){
    char copiaRequest[1024];
    strcpy(copiaRequest,request);
    char *ptr = strtok(copiaRequest, " ");
    for (int i=1;i<pos;i++){
        ptr = strtok(NULL, " ");
    }
    return ptr;
}


void crearArchivo(char* request){
    char *ipAdress;
    char *puerto, *puertoB;
    ipAdress= obtenerDato(request,2);
    printf("IP %s\n",ipAdress);
    printf("REQUEST %s\n",request);
    puerto= obtenerDato(request,3);
    puertoB = strtok(puerto, "\n");
    printf("PUERTO %s\n",puertoB);
    //printf("%s\n",request);

    printf("Metodo SEND-> Crear archivo\n");
    FILE *f = fopen("tabla.txt", "a");
    if (f == NULL)
    {
        printf("No se puede abrir el archivo!\n");
        exit(1);
    }
    char *str1, *token, *temp2;
    int j;

    for (j = 1, str1=request; ; ++j, str1 = NULL) {// Revisar
        token = strtok(str1, "\n");
        printf("TOKEN: %s", token);
        strncpy(temp2,token, strlen(token)-2);
        char temp[MAX];
        if (token == NULL)
            break;
        else if(j!=1){
            snprintf(temp, MAX, " %s %s",ipAdress,puertoB);
            //printf("TEMP %s",temp);
            strcat(temp2,temp);
            strcat(temp2,"\n");
            //fprintf(f, "%s\n", token);
            printf("%d: %s\n", j, temp2);
        }
    }
    fclose(f);
    return;
}

void getMetodoSEND(int clienteSocket){
    char buff[4096];
    char request[4096];
    memset(buff, 0, 4096);
    valread = read(clienteSocket, buff, 4096);
    strcpy(request, buff);
    char *method = strtok(buff, " ");

    if (strcmp(method, "SEND") == 0) {
        crearArchivo(request);
    }
    return;
}

void buscarArchivo(char *nameFile,int clienteSocket){
    char name[MAX];
    char archivos[MAX];
    int matches;
    strcpy(name,nameFile);

    FILE *file = fopen("tabla.txt", "r");
    FILE *fTemp = fopen("listaArchivos.txt","w");
    if ( file != NULL )
    {
        char line [MAX];
        while ( fgets ( line, sizeof line, file ) != NULL ){
            if(strstr(line,name)!= NULL) {
                matches++;
                char temp[MAX];
                fprintf(fTemp, "%s", line);
                //Escribir en el cliente
                //hacer archivo para almacenarlo y leerlo en el cliente
                //
                char *tamano= obtenerDato(line,2);
                char *hash= obtenerDato(line,3);
                snprintf(temp, MAX, "%s %s\n",tamano,hash);
                strcat(archivos,temp);
                //Se envía request
            }
        }
        write(clienteSocket, archivos, strlen(archivos));
        printf("\n%d\n", matches);

    }
    fclose(file);
    fclose(fTemp);
    return ;

}

void getMetodoFIND(int clienteSocket){
    char buffFind[4096];
    char * nameFile;
    memset(buffFind, 0, 4096);
    valread = read(clienteSocket, buffFind, 4096);
    printf("%s\n", buffFind);
    nameFile= obtenerDato(buffFind,2);
    printf("%s\n", nameFile);
    buscarArchivo(nameFile, clienteSocket);

   //Buscar en archivo
}


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

        getMetodoSEND(clienteSocket);
        getMetodoFIND(clienteSocket);


       /* */
    }
}
