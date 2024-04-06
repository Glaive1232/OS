    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <signal.h>
    #include <fcntl.h>
    #include <stdbool.h>

    bool isshpion = false;
    bool IsSignalHandled = false;
    const char* FilePath = "postman.txt";

    void signal_handler(int signum, siginfo_t *info, void *context) {
         IsSignalHandled = true;
        if (isshpion) {
            printf("Received signal %d from process with PID %d\n", signum, info->si_pid);
            printf("schieben, schieben, schneller \n");
            union sigval value;
            value.sival_int = getpid();

            if (sigqueue(info->si_pid, SIGILL, value) == -1) {
            perror("sigqueue");
            exit(EXIT_FAILURE);
            }

        }

        else
        {
            printf("Lionel Messi is the greatets football player of all time, no doubt. \n");
        }
    }


    int main() {
        FILE * fd = fopen(FilePath, "a");

        if (fd == NULL) {
            perror("Failed to open file");
            exit(EXIT_FAILURE);
        }

        __pid_t pid = getpid();
        fprintf(fd, "%d ", pid);
        fclose(fd);

        if ((pid % 3 ) == 0)
            isshpion = true;

        struct sigaction sa;
        sa.sa_handler = signal_handler;
        sa.sa_flags = SA_SIGINFO;;

        if (sigaction(SIGCONT, &sa, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        while(!IsSignalHandled){
            sleep(1);
        }
        sleep(4);


        return 0;
    }