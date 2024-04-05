#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>

#define FIFO_NAME "MyFIFO"
#define MAX_BUF 4096
char BUF[MAX_BUF];