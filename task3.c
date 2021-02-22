#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // create pipe
    int piped[2];
    if (pipe(piped) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }
    //read pipe size
    long pipe_size = (long) fcntl(piped[0], F_GETPIPE_SZ);
    if (pipe_size == -1) {
        perror("get read pipe size failed.");
    }
    printf("default read pipe size: %ld\n", pipe_size);
    if (close(piped[0]) < 0) {
        printf("parent: Can\'t close read pipe\n");
        exit(-1);
    }

    //write pipe size
    pipe_size = (long) fcntl(piped[1], F_GETPIPE_SZ);
    if (pipe_size == -1) {
        perror("get write pipe size failed.");
    }
    printf("default read pipe size: %ld\n", pipe_size);
    if (close(piped[1]) < 0) {
        printf("Can\'t close write pipe\n");
        exit(-1);
    }

    return 0;
}
