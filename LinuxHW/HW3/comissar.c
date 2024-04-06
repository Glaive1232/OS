#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>

#define MAXBUFFSIZE 10000
const char* FilePath = "postman.txt";

void signal_handler(int signum, siginfo_t *info, void *context) {
    printf("WE FOUND SPY, READY TO FIRE \n");
    kill(info->si_pid, SIGKILL);
    sleep(4);
    printf("SPY killed, continue searching !\n");
}

int main(){
    FILE * fd = fopen(FilePath, "r");
    if (fd == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char* processes = (char*) malloc(sizeof(char) * MAXBUFFSIZE + 1);
    if (processes == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    fgets(processes, MAXBUFFSIZE, fd);

    char *token = strtok(processes, " ");

    struct sigaction sa;
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;

if (sigaction(SIGILL, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }



    while (token != NULL ) {
        pid_t pid = atoi(token);
        printf("Sending SIGCONT to PID: %d\n", pid);
         union sigval value;
         value.sival_int = getpid();

         if (sigqueue(pid, SIGCONT, value) == -1) {
             perror("sigqueue");
        }
        sleep (4);
        token = strtok(NULL, " "); 
    }

    printf("ALL DETECTED SHPIONS WERE DESTROYED, CONGRATULAIONS, COMRADES!! \n");
    fflush(fd);
    fclose(fd);
    fd = fopen(FilePath, "wb");
    fclose(fd);
    free(processes);

    return 0;
}
