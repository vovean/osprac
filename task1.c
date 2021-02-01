#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("PID: %d\r\n", getpid());
    printf("Parent PID: %d\r\n", getppid());

    return 0;
}