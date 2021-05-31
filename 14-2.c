//
// Created by vislo on 31.05.2021.
//

#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
    switch (nsig) {
        case SIGINT:
            printf("Signal %d (Ctrl+C pressed) was received and handled\n", nsig);
            return;
        case SIGQUIT:
            printf("Signal %d (Ctrl+4 pressed) was received and handled\n", nsig);
            return;
        default:
            printf("Unknown signal %d was received\n", nsig);
    }
}

int main() {
    signal(SIGINT, my_handler);
    signal(SIGQUIT, my_handler);
    while (1);
}