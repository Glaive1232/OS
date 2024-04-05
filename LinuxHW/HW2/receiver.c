#include "prog.h"

int main(){
    printf("Process 2 (Receiver) is running...\n");
    int fd;

    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while (1) {

        read(fd, BUF, MAX_BUF);
        if (strcmp(BUF, "exit\n") == 0)
            break;

        printf("Received message: %s", BUF);

        printf("Sending acknowledgment to Process 1...\n");
        write(fd, "ACK", 4);
    }

    close(fd);

    if (unlink(FIFO_NAME) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    printf("Named pipe deleted.\n");
}