#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

int main() {
    int fd_parent[2], fd_child[2], result;

    size_t size_parent, size_child;
    char string_parent[7], string_child[6];

    if (pipe(fd_parent) < 0) {
        printf("Can\'t open parent pipe\n");
        exit(-1);
    }

    if (pipe(fd_child) < 0) {
        printf("Can\'t open child pipe\n");
        exit(-1);
    }

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {

        /* Parent process */

        if (close(fd_parent[READ]) < 0) {
            printf("parent: Can\'t close reading side of parent pipe\n");
            exit(-1);
        }

        if (close(fd_child[WRITE]) < 0) {
            printf("parent: Can\'t close writing side of child pipe\n");
            exit(-1);
        }

        printf("log: parent write string into child pipe\n");
        size_parent = write(fd_parent[WRITE], "parent", 7);

        if (size_parent != 7) {
            printf("parent: Can\'t write all string to pipe\n");
            exit(-1);
        }

        if (close(fd_parent[WRITE]) < 0) {
            printf("parent: Can\'t close writing side of parent pipe\n");
            exit(-1);
        }

        size_parent = read(fd_child[READ], string_child, 6);

        if (size_parent < 0) {
            printf("parent: Can\'t read from child pipe\n");
            exit(-1);
        }

        printf("parent: read string (%zu bytes) from child: %s\n", size_parent, string_child);

        if (close(fd_child[READ]) < 0) {
            printf("parent: Can\'t close reading side of child pipe\n");
            exit(-1);
        }

        printf("parent: exit\n");

    } else {

        /* Child process */

        if (close(fd_parent[WRITE]) < 0) {
            printf("child: Can\'t close writing side of parent pipe\n");
            exit(-1);
        }

        if (close(fd_child[READ]) < 0) {
            printf("child: Can\'t close reading side of child pipe\n");
            exit(-1);
        }

        size_child = read(fd_parent[0], string_parent, 7);

        if (size_child < 0) {
            printf("child: Can\'t read string from parent pipe\n");
            exit(-1);
        }

        if (close(fd_parent[READ]) < 0) {
            printf("child: Can\'t close writing side of parent pipe\n");
            exit(-1);
        }

        printf("child: read string (%zu bytes) from parent pipe: %s\n", size_child, string_parent);

        printf("log: child write string into parent pipe\n");
        size_child = write(fd_child[WRITE], "child", 6);

        if (size_child != 6) {
            printf("child: Can\'t write all string to pipe\n");
            exit(-1);
        }

        if (close(fd_child[WRITE]) < 0) {
            printf("child: Can\'t close writing side of child pipe\n");
            exit(-1);
        }

        printf("child: exit\n");
    }

    return 0;
}
