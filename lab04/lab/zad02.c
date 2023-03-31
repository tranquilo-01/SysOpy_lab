// napisz program ktory tworzy proces potomny a  nastepnie w niesknczinej petli
// wysyla do swojego potomka sygnaly SIGUSR1. Potomek powinien zliczac otrzymane
// od przodka sygnaly SIGUSR1 i wyswietlac na biezaco stan licznika

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int receivedSignals = 0;

void signalHandler(int signum) {
    receivedSignals++;
    printf("Received signal number %d\n", receivedSignals);
}

int main() {
    int pid;
    pid = fork();
    if (pid) {
        sleep(1);
        while (1) {
            sleep(1);
            kill(pid, SIGUSR1);
        }
    } else {
        signal(SIGUSR1, signalHandler);
        while (1) {
            sleep(1);
        }
    }
}