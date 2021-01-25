#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    printf("UID: %d\r\n", (int) getpid());
    printf("GID: %d\r\n", (int) getppid());
    return 0;
}


