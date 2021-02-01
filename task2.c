#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        printf("Smth has gone wrong\r\n");
    } else if (pid == 0) {
        printf("I'm a child\r\n");
    } else {
        printf("I'm a parent. Child PID: %d\r\n", getpid());
    }

    return 0;
}