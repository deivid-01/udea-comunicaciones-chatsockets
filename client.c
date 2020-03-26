#include <stdio.h>  //Entrada y Salida
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  //Padre de socket and netinet
#include <sys/socket.h>//Estructuras y definiciones para los sockets
#include <netinet/in.h> //Estructuras y deficiones necesitadas para la direccion de dominio
#include <netdb.h> 

void error(const char *msg) // Imprimir errores
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    //Definicion de variables

    int sockfd, portno, n; //Socket y numero de puerto
    struct sockaddr_in serv_addr; //Dirección del servidor
    struct hostent *server; //Informacion del host

    char buffer[256]; //Donde se va a almacenar la información
    
	//----Verificar Input---
	//argv[0]filename
	//argv[1] server_ipaddress
	//argv[2] portno
if (argc < 3) {
       fprintf(stderr,"ERROR Ingrese el puerto y la IP\n", argv[0]);
       exit(0);
    }
//----------------------------------
    portno = atoi(argv[2]); //Asignar puerto

//1. Crear Socket-----------------------------------------------------------
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    
//Verificar que se creo correctamente el socket y existe un servidor
    if (sockfd < 0) 
        error("ERROR Abriendo el Socket");
    server = gethostbyname(argv[1]);
    
if (server == NULL) {
        fprintf(stderr,"ERROR, No existe el servidor\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET; // Para tcp/ip
    
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);// asignar puerto oficial
//2.Conectar con el servidor
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR al intentar conectarse");

//3. Comunicacion con el Servidor: Escribir y Leer
printf("\033[0;33m");    
printf("Cliente: ");
    while(1)
    {
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer)); //Escribir mensaje
        if (n < 0) 
             error("ERROR escribiendo hacia el socket");
        bzero(buffer,256);
        n = read(sockfd,buffer,255); //Leer mensaje
        if (n < 0) 
             error("ERROR Leyendo  dede el socket");
printf("------------------------------------\n");
printf("\033[0;32m");       
printf("Servidor : %s",buffer);
printf("\033[0;0m"); 
printf("------------------------------------\n");
printf("\033[0;33m");
printf("Cliente: ");

        int i = strncmp("Adios" , buffer , 3);
        if(i == 0)
               break;
    }
    close(sockfd);
    return 0;
}
