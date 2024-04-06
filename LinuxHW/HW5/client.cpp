#include <arpa/inet.h>
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

 struct sockaddr_in cli;

 sock = socket(AF_INET, SOCK_STREAM, 0);
 if (sock == -1) {
   printf("Socket not created...\n");
   exit(0);
  }

  memset(&cli, '\0', sizeof(cli));

 cli.sin_family = AF_INET;
  cli.sin_addr.s_addr = inet_addr("127.0.0.1");
  cli.sin_port = htons(PORT);
 
  int res_conn = connect(sock, (SA*)(&cli), sizeof(cli));
  
 if (res_conn == -1) {
    perror("Connection failed");
    exit(EXIT_FAILURE);
}


 while(1){
    printf("Enter your message to the server: ");
 char message[MAX];
 
 fgets(message, MAX, stdin);
  message[strcspn(message, "\n")] = 0;
 write(sock,message,strlen(message));

 char message_[MAX];
 int bytes_received = read(sock, message_, MAX - 1);
    if (bytes_received == -1) {
        printf("Error receiving data\n");
    } else if (bytes_received == 0) {
        printf("Connection closed by peer\n");
    } else {
  if(strcmp(message_, "exit") == 0){
   printf("Client Exit...\n");
   break;
  }
        printf("From server: %s\n", message_);
    }
 }
 

 close(sock);

 return 0;
}
