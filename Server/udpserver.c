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

#define PORT 15000 
#define MAXLINE 65535 // Ajustado para recibir paquetes grandes

int main()
{
    char buffer[MAXLINE];
    int listenfd;
    socklen_t len; // Cambio a socklen_t
    struct sockaddr_in servaddr, cliaddr;

    printf("Listening in port number: %d\n", PORT);

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
    // Recibir el datagrama
    ssize_t n = recvfrom(listenfd, buffer, MAXLINE - 1, 0, (struct sockaddr*)&cliaddr, &len);

    if (n < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0'; // Terminación segura
    printf("He recibido un paquete de %ld bytes.\n", n);

    // --- LÓGICA DE EXTRACCIÓN (Protocolo Cliente) ---
    // El cliente envía: "nombre_archivo\ncontenido..."

    char* token_pos = strchr(buffer, '\n');
    if (token_pos != NULL) {
        // 1. Separar el nombre del archivo
        *token_pos = '\0'; // Reemplaza el '\n' por un terminador para obtener el nombre
        char* nombreArchivo = buffer;
        char* contenido = token_pos + 1; // El contenido empieza justo después

        printf("Nombre del archivo a crear: %s\n", nombreArchivo);

        // 2. Abrir el archivo para escritura
        FILE* archivo = fopen(nombreArchivo, "w");
        if (archivo == NULL) {
            perror("Error al crear el archivo");
        }
        else {
            // 3. Escribir el contenido recibido
            fputs(contenido, archivo);
            fclose(archivo);
            printf("Archivo guardado exitosamente.\n");
        }
    }
    else {
        printf("Error: Formato de mensaje incorrecto (no se encontró '\\n')\n");
    }

    // Opcional: Responder al cliente
    char* ack = "Archivo recibido";
    sendto(listenfd, ack, strlen(ack), 0, (struct sockaddr*)&cliaddr, len);

    close(listenfd);
    printf("Conexion cerrada\n");
    return 0;
}