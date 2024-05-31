#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define READ 0
#define WRITE 1

typedef struct {
    FILE *file;
    pid_t pid;
} PopenFile;

PopenFile *popen_(const char *command, const char *type) {
    int pipefd[2];
    pid_t pid;
    PopenFile *popenFile = malloc(sizeof(PopenFile));

    if (pipe(pipefd) == -1) {
        free(popenFile);
        return NULL;
    }

    if ((pid = fork()) == -1) {
        free(popenFile);
        return NULL;
    }

    if (pid == 0) { 
        if (strcmp(type, "r") == 0) {
            close(pipefd[READ]); 
            dup2(pipefd[WRITE], STDOUT_FILENO); 
            close(pipefd[WRITE]);
        } else if (strcmp(type, "w") == 0) {
            close(pipefd[WRITE]); 
            dup2(pipefd[READ], STDIN_FILENO); 
            close(pipefd[READ]);
        } else {
            free(popenFile);
            exit(EXIT_FAILURE);
        }

        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        _exit(EXIT_FAILURE); 
    }

    if (strcmp(type, "r") == 0) {
        close(pipefd[WRITE]); 
        popenFile->file = fdopen(pipefd[READ], type);
    } else {
        close(pipefd[READ]); 
        popenFile->file = fdopen(pipefd[WRITE], type);
    }

    if (popenFile->file == NULL) {
        close(pipefd[READ]);
        close(pipefd[WRITE]);
        free(popenFile);
        return NULL;
    }

    popenFile->pid = pid;
    return popenFile;
}

int pclose_(PopenFile *popenFile) {
    if (popenFile == NULL) {
        errno = EINVAL;
        return -1;
    }

    int status;
    FILE *file = popenFile->file;
    pid_t pid = popenFile->pid;

    fclose(file);
    free(popenFile);

    if (waitpid(pid, &status, 0) == -1) {
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    } else {
        return -1;
    }
}

int main() {
    PopenFile *fp;
    char buffer[128];
// Can be modified to read command from console        
    fp = popen_("ls -l", "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp->file) != NULL) {
        fputs(buffer, stdout);
    }

    if (pclose_(fp) == -1) {
        perror("pclose");
        exit(EXIT_FAILURE);
    }

    return 0;
}
