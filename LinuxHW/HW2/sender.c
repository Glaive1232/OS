#include "prog.h"

int main(){
  int fd;

    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

     printf("Named pipe created successfully.\n");

     printf("Process 1 (Sender) is running...\n");

    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }


while (1) {
        printf("Enter message (or 'exit' to quit): ");
        fgets(BUF, MAX_BUF, stdin);

        if (strcmp(BUF, "exit\n") == 0)
            break;

        write(fd, BUF, strlen(BUF) + 1);

        printf("Waiting for acknowledgment from Process 2...\n");
        sleep(5);
        read(fd, BUF, MAX_BUF);
        printf("Acknowledgment received from Process 2.\n");
    }


    close(fd);



}