/*

   Conexion UDP
   Codigo del servidor

   Nombre Archivo: udpserver.c   
   Fecha: Febrero 2023

   Compilacion: cc udpserver.c -lnsl -o udpserver
   Ejecución: ./udpserver

*/

// server program for udp connection 
#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h>

#define PORT 15000 
#define MAXLINE 1000 
  
// Driver code 
int main() 
{    
    char buffer[MAXLINE]; 
    char *message = "Hello Client"; 
    int listenfd, len; 
    struct sockaddr_in servaddr, cliaddr; 
    int empezar;
    printf("Listening in port number: %d", PORT);
    //printf("\nPresione cualquier tecla para empezar \n");
    //scanf("%d",&empezar);

    //bzero(&servaddr, sizeof(servaddr)); 
  
    // Create a UDP Socket 
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(listenfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }         

    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET;  

    // bind server address to socket descriptor 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
       
    //receive the datagram 
    len = sizeof(cliaddr); 
    int n = recvfrom(listenfd, buffer, MAXLINE, 
            0, (struct sockaddr*)&cliaddr,&len); //receive message from server 
    if(n < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }else{
        buffer[n] = '\0'; 
        printf("\nHe recibido del cliente: ");
        printf("%s\n",buffer);
    }       
    // send the response 
    sendto(listenfd, message, strlen(message), 0, 
          (struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
    close(listenfd);
    printf("Conexion cerrada\n");
    return 0;
} 