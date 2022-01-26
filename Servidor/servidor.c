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
    puerto= obtenerDato(request,3);
    puertoB = strtok(puerto, "\n");
    printf("\n------------------\nCreando archivo\n------------------\n\n");
    printf("REQUEST RECIBIDO:\n%s\n\n",request);
    FILE *f = fopen("tabla.txt", "a");
    if (f == NULL)
    {
        printf("No se puede abrir el archivo!\n");
        exit(1);
    }
    char token[1024];
    char *str1, *temp;
    int j;
    int len;
    printf("ARCHIVO:\n");
    for (j = 1, str1=request; ; ++j, str1 = NULL) {
        memset(token, 0, 1024);
        temp = strtok(str1, "\n");
        if (temp == NULL)
            break;

        strcpy(token,temp);
        len = strlen(token);
        if(token[len - 1] == '\n')//Verificamos si el último caracter es un salto de línea
            token[len - 1] = '\0';//Eliminamos el salto de línea

        else if (j!=1){
            snprintf(temp, MAX, " %s %s\n",ipAdress,puertoB);
            strcat(token,temp);
            fprintf(f, "%s", token);
            printf("%d: %s", j-1, token);
        }

    }
    fclose(f);
    printf("\n------------------\nArchivo Creado\n------------------\n\n");
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
    int matches=0;
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
                char *tamano= obtenerDato(line,2);
                char *hash= obtenerDato(line,3);
                snprintf(temp, MAX, "%s %s\n",tamano,hash);
                strcat(archivos,temp);
            }
        }
        write(clienteSocket, archivos, strlen(archivos));
        printf("\nArchivos encontrados: %d\n", matches);
        printf("\nArchivos enviados:\n%s\n", archivos);
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
    printf("REQUEST RECIBIDO:\n%s\n", buffFind);
    nameFile= obtenerDato(buffFind,2);
    buscarArchivo(nameFile, clienteSocket);
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
