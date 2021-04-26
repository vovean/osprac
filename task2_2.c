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
    int msqid;
    char pathname[] = "task2_1.c";
    key_t key;
    long len, maxlen;

    struct mymsgbuf {
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

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

    for (short i = 1; i <= 5; i++) {
        maxlen = sizeof(mybuf.info);
        // here to show some difference lets accept only messages of type 2
        if ((len = msgrcv(msqid, &mybuf, maxlen, 2, 0)) < 0) {
            printf("Can\'t receive message of type 2 from queue\n\r");
            exit(-1);
        }

        printf("task2_2 received a message: message type = %ld, sinfo = %hd, finfo = %f\n\r", mybuf.mtype,
               mybuf.info.sinfo, mybuf.info.finfo);
    }


    printf("task2_2 has received all messages\n\r");


    for (short i = 0; i < 5; i++) {
        //
        // Fill in the structure for the message and
        // determine the length of the informative part.
        //
        mybuf.mtype = i;
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

    printf("task2_2 done sending messages\n\r");

    return 0;
}