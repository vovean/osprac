#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("Command line arguments:\r\n");
    for (int i = 0; i < argc; ++i) {
        printf("%d) %s\r\n", i, argv[i]);
    }
    printf("\r\nEnv params:\r\n");
    int i = 0;
    while (1) {
        if (envp[i] != NULL) {
            printf("%d) %s\r\n", i, envp[i]);
        }
        else{
            break;
        }
        i++;
    }

    return 0;
}