//
// Created by vislo on 31.05.2021.
//

#include <signal.h>

int main() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    while(1);
}