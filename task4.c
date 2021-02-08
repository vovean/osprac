#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid = fork();

    if (pid == -1) {
        printf("Smth has gone wrong\r\n");
    } else if (pid == 0) {
        printf("I'm a child; start program '/bin/ls' with param '-la'\r\n");
        execle("/bin/ls", "/bin/ls", "-la", 0, envp);
    } else {
        printf("I'm a parent. Child PID: %d\r\n", getpid());
    }

    return 0;
}