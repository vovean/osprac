//
// Created by vislo on 01.03.2021
// В этом файле мы считываем наш код и пишем в Shared Memory
// here we have 2652 symbols
//
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define kb 1024

int main() {
    char textArr[8 * kb];
    char *text = textArr;
    char *shm;      // Pointer to shared memory
    int shmid;     // IPC descriptor for the shared memory
    char thisFile[] = "task1_a.c";  // The fileDesc name used to generate the key.

    printf("Opening %s\r\n", thisFile);
    int fileDesc;
    if ((fileDesc = open(thisFile, O_RDONLY)) < 0) {
        printf("Failed to open %s for reading\r\n", thisFile);
        exit(-1);
    }

    printf("Reading from %s\r\n", thisFile);
    int length = 0;
    char symbol;
    int readBytes;
    while (1) {
        readBytes = read(fileDesc, &symbol, 1);
        if (readBytes < 0) {
            printf("Failed to read from %s\r\n", thisFile);
            exit(-1);
        }
        if (readBytes == 0) break;

        textArr[length] = symbol;
        length++;
    }
    printf("Read %d symbols from %s.\r\n", length, thisFile);

    if (close(fileDesc) < 0) {
        printf("Failed to close %s\r\n", thisFile);
        exit(-1);
    }


    printf("Generating key for %s\r\n", thisFile);
    key_t key;         // IPC key
    if ((key = ftok(thisFile, 0)) < 0) {
        printf("Failed to generate key for \"%s\"\r\n", thisFile);
        exit(-1);
    }

    printf("Obtaining shared memory\r\n");
    if ((shmid = shmget(key, length * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Failed to create shared memory\r\n");
            exit(-1);
        } else {
            if ((shmid = shmget(key, length * sizeof(char), 0)) < 0) {
                printf("Failed to find existing shared memory\r\n");
                exit(-1);
            }
        }
    }

    printf("Attaching shared memory\r\n");
    if ((shm = (char *) shmat(shmid, 0, 0)) == (char *) (-1)) {
        printf("Failed to attach shared memory\r\n");
        exit(-1);
    }

    printf("Writing %d chars to shared memory\r\n", length);
    for (int i = 0; i < length; i++) {
        shm[i] = text[i];
    }
    printf("Successfully wrote %d chars to shared memory\r\n", length);

    printf("Detaching shared memory\r\n");
    if (shmdt(shm) < 0) {
        printf("Failed to detach shared memory\r\n");
        exit(-1);
    }

    return 0;
}

