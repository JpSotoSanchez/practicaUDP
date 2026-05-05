/*
   Conexion UDP
   Codigo del servidor corregido
*/

#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdlib.h>

#define MAXLINE 65535 // Ajustado para recibir paquetes grandes

int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <puerto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int PORT = atoi(argv[1]);
    if (PORT <= 0 || PORT > 65535) {
        fprintf(stderr, "Puerto inválido: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    char buffer[MAXLINE];
    int listenfd;
    socklen_t len; // Cambio a socklen_t
    struct sockaddr_in servaddr, cliaddr;

    printf("Listening in port %d\n", PORT);

    // Crear socket
    listenfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listenfd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Limpiar y configurar estructura (IMPORTANTE)
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET;

    // Bind con verificación
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    len = sizeof(cliaddr);
    ssize_t n = recvfrom(listenfd, buffer, MAXLINE - 1, 0, (struct sockaddr*)&cliaddr, &len);

    if (n < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0'; // Terminación segura
    printf("He recibido un paquete de %ld bytes.\n", n);


    char* token_pos = strchr(buffer, '\n');
    if (token_pos != NULL) {
        *token_pos = '\0'; 
        char* nombreArchivo = buffer;
        char* contenido = token_pos + 1; // El contenido empieza justo después

        printf("Nombre del archivo a crear: %s\n", nombreArchivo);

        FILE* archivo = fopen(nombreArchivo, "w");
        if (archivo == NULL) {
            perror("Error al crear el archivo");
        }
        else {
            fputs(contenido, archivo);
            fclose(archivo);
            printf("Archivo guardado exitosamente.\n");
        }
    }
    else {
        printf("Error: Formato de mensaje incorrecto (no se encontró '\\n')\n");
    }

    char* ack = "Archivo recibido";
    sendto(listenfd, ack, strlen(ack), 0, (struct sockaddr*)&cliaddr, len);

    close(listenfd);
    printf("Conexion cerrada\n");
    return 0;
}