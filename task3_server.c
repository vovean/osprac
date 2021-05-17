//
// Created by vislo on 17.05.2021.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>


int main() {
    int msqid;
    char pathname[] = "task3_server.c";
    key_t key;
    int msgSz, maxMsgSz;

    struct requestDataBuf {
        long mtype;
        struct {
            pid_t pid;
            double message;
        } info;
    } clientBuf;

    struct responseDataBuf {
        long mtype;
        struct {
            double message;
        } info;
    } serverBuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Failed to generate key\n");
        exit(-1);
    }
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Failed to get msqid\n");
        exit(-1);
    }

    while (true) {
        maxMsgSz = sizeof(clientBuf.info);
        if (msgrcv(msqid, (struct clientmsgbuf *) &clientBuf, maxMsgSz, 1, 0) < 0) {
            printf("Failed to receive message from queue\n");
            exit(-1);
        }
        printf("Received data: Client pid: %d; Data: %lf\n", clientBuf.info.pid, clientBuf.info.message);
        double responseData = clientBuf.info.message * clientBuf.info.message;
        printf("The response will be %lf\n", responseData);

        serverBuf.mtype = clientBuf.info.pid;
        serverBuf.info.message = responseData;
        msgSz = sizeof(serverBuf.info);
        if (msgsnd(msqid, (struct sermsgbuf *) &serverBuf, msgSz, 0) < 0) {
            printf("Failed to send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("Successfully sent the response. Waiting for the next message...\n");
    }
    return 0;
}