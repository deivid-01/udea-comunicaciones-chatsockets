
#include <stdio.h> //Entrada y Salida
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> //Padre de socket and netinet
#include <sys/socket.h> //Estructuras y definiciones para los sockets
#include <netinet/in.h> //Estructuras y deficiones necesitadas para la direccion de dominio


void error(const char *msg) // Imprimir errores
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     
	//Definicion de variables
     int sockfd; //Socket
     int  newsockfd; //Socket para aceptar
     int portno; //Número de puerto
     socklen_t clilen;
     char buffer[255]; //Donde se va a almacenar la información
     struct sockaddr_in serv_addr, cli_addr; //serv_addr-> Almacenar direccion del servidor| cli_addr-> Almacenar direcion del cliente
     int n;

	//---------Verificar que ingrese el puerto y el nombre del archivo------
     if (argc < 2) {
         fprintf(stderr,"ERROR, debes ingresar un PUERTO\n");
         exit(1);
     }
	//----------------------

	//1. Creacion del Socket

     sockfd = socket(AF_INET, SOCK_STREAM, 0);

	////---------Verificar que no hay errores------
     if (sockfd < 0) 
        error("ERROR Abriendo el socket");
	//--------

     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]); //asignar numero de puerto;
     serv_addr.sin_family = AF_INET; // Para tcp/ip
     serv_addr.sin_addr.s_addr = INADDR_ANY; //Asignar direccion del servidor
     serv_addr.sin_port = htons(portno); //asignar puerto
     
	//2. Asignar una direccion al socket
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR asignando dirección");


     //3. Atender Conexiones
     listen(sockfd,5);//maximo numero de clientes 5 para escuchar
     
     clilen = sizeof(cli_addr); //tamaño de la direccion del cliente
     
      //4. Aceptar Conexiones
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);

	////---------Verificar que acepto al cliente------
     if (newsockfd < 0) 
          error("ERROR, No aceptó el cliente");

	//--------------------------------------------------
     
	//Comunicación entre cliente y servidor

	//5. Leer y escribir
	
     while(1)
     {
           bzero(buffer,255); //Limpiar buffer
           n = read(newsockfd,buffer,255);  //Leer mensaje

           if (n < 0) error("ERROR Leyendo el Mensaje");
	printf("----------------------------------------\n");  
	printf("\033[0;32m"); //Cambiar color a verde
          printf("Cliente: %s",buffer);
	  printf("\033[0;0m"); //resetar color
printf("----------------------------------------\n"); 
          printf("\033[0;33m"); //Cambiar color a amarillo
          
	printf("Servidor : ");
	  printf("\033[0;0m"); //Resetear color
	
          bzero(buffer,255); //limpiar buffer
          fgets(buffer,255,stdin);

          n = write(newsockfd,buffer,strlen(buffer)); //Escribir mensaje
		
           if (n < 0) error("ERROR Escrbiendo el mensaje");
	
           int i=strncmp("Adios" , buffer, 3);
           if(i == 0)
               break;
     }

	//Cierra el socket del cliente y el del server
     close(newsockfd);
     close(sockfd);
     return 0; 
}
