//
// Created by vislo on 01.03.2021
// В этом файле мы считываем данные из Shared Memory
//
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    char *shm;
    int shmid;
    char writerFile[] = "task1_a.c";
    key_t key;

    printf("Generating key for %s\r\n", writerFile);
    if ((key = ftok(writerFile, 0)) < 0) {
        printf("Failed to generate key for \"%s\"\r\n", writerFile);
        exit(-1);
    }

    printf("Obtaining shared memory\r\n");
    int symbolsCount = 2652;
    if ((shmid = shmget(key, symbolsCount * sizeof(char), 0666 | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Existing shared memory not found\r\n");
            exit(-1);
        } else {
            if ((shmid = shmget(key, symbolsCount * sizeof(char), 0)) < 0) {
                printf("Failed to obtain existing shared memory\r\n");
                exit(-1);
            }
        }
    }

    printf("Attaching shared memory\r\n");
    if ((shm = (char *) shmat(shmid, 0, 0)) == (char *) (-1)) {
        printf("Failed to attach shared memory\r\n");
        exit(-1);
    }
    printf("Successfully read next data:\r\n%s\r\n", shm);

    printf("Detaching shared memory\r\n");
    if (shmdt(shm) < 0) {
        printf("Failed to detach shared memory\r\n");
        exit(-1);
    }

    printf("Deleting shared memory\r\n");
    if (shmctl(shmid, IPC_RMID, 0)) {
        printf("Failed to delete shared memory\r\n");
        exit(-1);
    }

    return 0;
}

