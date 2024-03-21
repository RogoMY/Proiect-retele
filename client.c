#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 2024
#define BUFFER_SIZE 5000
extern int errno;
int port;
int main(int argc,char *argv[]) {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[5000];
   if(argc!=3)
{printf("Sintaxa: %s <adresa_server> <port>\n",argv[0]);
return -1;}
port=atoi(argv[2]);

    // Creare socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Eroare la crearea socketului");
        exit(EXIT_FAILURE);
    }

    // Inițializare structură sockaddr_in pentru server
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);
    server_addr.sin_port = htons(port);

    // Conectare la server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Eroare la conectarea la server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Buclă infinită pentru a permite trimiterea mai multor comenzi
    while (1) {
        // Citire comanda de la utilizator și trimitere la server
        printf("Introduceti comanda (sau 'exit' pentru a iesi): ");
        bzero(buffer, BUFFER_SIZE); 
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // Eliminați newline de la sfârșitul șirului
	
        if (write(sockfd, buffer, strlen(buffer)) <= 0) {
            perror("Eroare la trimiterea comenzii la server");
            break; // Ieșire din buclă în caz de eroare
        }
        printf("Trimitem mesajul %s\n",buffer);
        if(strcmp(buffer,"exit")==0)
        	{
        	return 0;
        	}
        printf("Mesajul a fost trimis\n");

        // Așteptare răspuns de la server și afișare   
                bzero(buffer, BUFFER_SIZE); 
        if (read(sockfd, buffer, 5000) <= 0) {
            perror("Eroare la citirea răspunsului de la server");
            break; // Ieșire din buclă în caz de eroare
        }    else        
            printf("Răspuns de la server: %s\n", buffer);    
}

    // Închidere socket
   close(sockfd);

    return 0;
}
