/*

   Conexion UDP
   Codigo del cliente

   Nombre Archivo: udpcliente.c   
   Fecha: Febrero 2023

   Compilacion: cc udpcliente.c -lnsl -o udpcliente
   Ejecución: ./udpclient
*/
// udp client driver program 
#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h> 
  
//#define PORT 15000 
#define MAXLINE 1000 
  
int max = 60000;
// Driver code 
int main(int argc, char* argv[])
{    
    if (argc != 4) {
        return 1;
    }

    char buffer[MAXLINE]; 
    char *ip = argv[2];
    char *fileName = argv[1]; 
    char contenido[max];
    contenido[0] = '\0';
    int sockfd; 
    int port = atoi(argv[3]);
    struct sockaddr_in servaddr; 
      
    // clear servaddr 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_addr.s_addr = inet_addr(ip); 
    servaddr.sin_port = htons(port); 
    servaddr.sin_family = AF_INET; 
      
    // create datagram socket 
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if(sockfd==-1){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    int contChar = 0;
    strcat(contenido, fileName);
    strcat(contenido, "\n");
    contChar = strlen(contenido);

    //Leer archivo 
    FILE* archivo;
    char linea[100];

    // 1. Abrir el archivo en modo lectura ("r")
    archivo = fopen(fileName, "r");

    // 2. Verificar si el archivo existe
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

    int lenLinea = 0;
    // 3. Leer el archivo línea por línea
    printf("Contenido del archivo:\n");
    while (fgets(linea, 100, archivo) != NULL) {
        
        lenLinea = strlen(linea);
        if (contChar + lenLinea > max - 1) {
            printf("\nError: Demasiados Caracteres (límite excedido)\n");
            break;
        }
        else {
            strcat(contenido, linea);
            contChar += lenLinea;
        }
        printf("%s", linea);
    }

    // 4. Cerrar el archivo
    fclose(archivo);

    //it is not required to establish a connection
    //directly sending a message
    ssize_t r = sendto(sockfd, contenido, strlen(contenido), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(r==-1){
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }
      
   
    // close the descriptor 
    close(sockfd); 
    printf("Conexion cerrada\n");
} 
