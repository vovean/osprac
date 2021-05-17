//
// Created by vislo on 17.05.2021.
//

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main() {

    int msqid;
    char pathname[] = "task3_server.c";
    key_t key;

    struct requestDataBuf {
        long mtype;
        struct {
            pid_t pid;
            double message;
        } info;
    } clientBuf;
    clientBuf.mtype = 1;
    clientBuf.info.pid = getpid();

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

    // read number to send
    double payload;
    printf("Input a double number:\n");
    scanf("%lf", &payload);

    int msgSz = sizeof(clientBuf.info);
    clientBuf.info.message = payload;
    printf("PID: %d; Data: %lf; Sending...\n", clientBuf.info.pid, clientBuf.info.message);
    if (msgsnd(msqid, (struct requestDataBuf *) &clientBuf, msgSz, 0) < 0) {
        printf("Failed to send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Getting response from server...\n");

    int maxMsgSz = sizeof(serverBuf.info);
    if (msgrcv(msqid, &serverBuf, maxMsgSz, getpid(), 0) < 0) {
        printf("Failed to receive message from queue\n");
        exit(-1);
    }
    printf("Received response from server:  %lf; Exiting...\n", serverBuf.info.message);

    return 0;
}