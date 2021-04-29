//
// Created by vislo on 26.04.2021.
//

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int msqid;      // IPC descriptor for the message queue
    char pathname[] = "task2_1.c"; // The file name used to generate the key.
    // A file with this name must exist in the current directory.
    key_t key;     // IPC key
    long len, maxlen;      // Cycle counter and the length of the informative part of the message

    struct mymsgbuf // Custom structure for the message
    {
        long mtype;
        struct {
            short sinfo;
            float finfo;
        } info;
    } mybuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    //
    // Trying to get access by key to the message queue, if it exists,
    // or create it, with read & write access for all users.
    //
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    /* Send information */

    for (short i = 0; i < 5; i++) {
        //
        // Fill in the structure for the message and
        // determine the length of the informative part.
        //
        // 1...5
        mybuf.mtype = 1 + i;
        mybuf.info.sinfo = i;
        mybuf.info.finfo = (float) i;
        len = sizeof(mybuf.info);
        //
        // Send the message. If there is an error,
        // report it and delete the message queue from the system.
        //
        if (msgsnd(msqid, &mybuf, len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
    }
    printf("task2_1 done sending messages\n\r");


    for (short i = 0; i < 5; i++) {

        maxlen = sizeof(mybuf.info);
        //11...15
        if ((len = msgrcv(msqid, &mybuf, maxlen, 11 + i, 0)) < 0) {
            printf("Can\'t receive message from queue\n\r");
            exit(-1);
        }

        printf("task2_1 received a message: message type = %ld, sinfo = %hd, finfo = %f\n", mybuf.mtype,
               mybuf.info.sinfo, mybuf.info.finfo);
    }

    printf("task2_1 has received all messages\n\r");

    return 0;
}
