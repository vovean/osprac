#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>

void set_buf(struct sembuf *buffer, int op) {
    buffer->sem_op = op;
    buffer->sem_flg = 0;
    buffer->sem_num = 0;
}

int inc(int semid, struct sembuf *buffer) {
    set_buf(buffer, 1);
    return semop(semid, buffer, 1);
}

int dec(int semid, struct sembuf *buffer) {
    set_buf(buffer, -1);
    return semop(semid, buffer, 1);
}

#define READ 0
#define WRITE 1

/**
 * Logic:
 * Initial semaphore value is 0, also we have 2 functions to work with semaphore:
 * inc - increments semaphore counter
 * dec - decrements semaphore counter
 * If semaphore counter is less than0, that we are waiting
 *
 * Father logic:
 * father checks that N>=2, other wise it quits
 * then in a for-loop father sends message o the pipe and increments semaphore, allowing child to read
 * then father does 'decrement' by that waiting for the child to write something back and increment the semaphore
 * as sons as father gets positive semaphore counter he decrements it, reads the message
 * after that repeat the loop
 *
 * Child logic:
 * In an infinite loop child tries to decrement the semaphore counter
 * as soon as he does that he reads the message from the pipe
 * then he writes his own message back to the pipe
 * and increments the semaphore allowing father to read child's message
 *
 * !!P. S.
 * Semaphore is not being deletend in the end, so if we run the program multiple times, something may break :(
 */
int main() {
    int pipes[2], fork_status;
    key_t key;
    char pathname[] = "05-3.c";
    struct sembuf buffer;

    int semid;

    if (pipe(pipes) < 0) {
        printf("Failed to create a pipe");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Failed to generate a key");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("Failed to locate a semaphore\n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Failed to obtain a semaphore, exiting...");
            exit(-1);
        }
        printf("Semaphore created\n");
    }

    int N;
    printf("Input N:\n");
    scanf("%d", &N);

    fork_status = fork();

    if (fork_status < 0) {
        printf("Failed to fork");
        exit(-1);
    }

    size_t size;
    char resstring[6];
    if (fork_status > 0) {
        // We are father
        if (N < 2) {
            printf("N is lower than 2, exiting...");
            exit(-1);
        }

        for (size_t i = 0; i < N; i++) {
            if (write(pipes[WRITE], "Parent", 6) != 6) {
                printf("Failed to write a string");
                exit(-1);
            }

            printf("Pair %lu) Parent sent a message to the child\n", i + 1);

            inc(semid, &buffer);
            dec(semid, &buffer);
            size = read(pipes[READ], resstring, 6);

            if (size != 6) {
                printf("Failed to read a message from the child");
                exit(-1);
            }

            printf("Message received from child: %s\n", resstring);
        }
        close(pipes[0]);
    } else {
        int pairs = 0;
        while (1) {
            dec(semid, &buffer);
            size = read(pipes[READ], resstring, 6);

            if (size < 0) {
                close(pipes[WRITE]);
                close(pipes[READ]);
                printf("Exiting...");
                return 0;
            }
            pairs++;
            printf("Pair %d) Child received a message %s\n", pairs, resstring);

            if (write(pipes[WRITE], "Childd", 6) != 6) {
                printf("Failed to write a string");
                exit(-1);
            }
            inc(semid, &buffer);
        }
    }

    return 0;
}