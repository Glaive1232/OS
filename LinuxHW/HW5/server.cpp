#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 7777
#define MAX 1000
#define SA struct sockaddr


int main() {
    int sock = 0;
    int conn = 0;

    struct sockaddr_in servaddr, cli;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Socket not created...\n");
        exit(0);
    }
 printf("socket successfully created\n");

    memset(&servaddr, '\0', sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    int optval = 1;

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int res_bind = bind(sock, (SA*)(&servaddr), sizeof(servaddr));
    if (res_bind != 0) {
        printf("Bind not successful\n");
        exit(0);
    }

    int res_listen = listen(sock, 7);
    if (res_listen != 0) {
        printf("Listening not successful\n");
        exit(0);
    }
    socklen_t length = sizeof(cli);

    conn = accept(sock, (SA*)(&cli), &length);
    if (conn < 0) {
        printf("Connection not successful\n");
        exit(0);
    }

 while(1){

 char message[MAX];
 int bytes_received = read(conn, message, MAX - 1);
    if (bytes_received == -1) {
        printf("Error receiving data\n");
    } else if (bytes_received == 0) {
        printf("Connection closed by peer\n");
    } else {
        printf("From client: %s\n", message);
    }


 char message_[MAX];
 printf("             To client: ");
 fgets(message_, MAX, stdin);
  message_[strcspn(message_, "\n")] = 0;
 write(conn,message_,MAX - 1);
 if(strcmp(message_, "exit") == 0){
  printf("Server Exit...\n");
  break;
 }
 }
 close(sock);
   
    

    return 0;
}
