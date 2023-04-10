#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

volatile int ack_received = 0;

void handle_ack(int signo) {
    printf("ack received\n");
    ack_received = 1;
    fflush(NULL);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Sender: invalid number of arguments. Catcher id and tasks expected\n");
        exit(1);
    }

    int catcher_pid = atoi(argv[1]);

    // ustawienie handlera dla ack
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handle_ack;
    sigaction(SIGUSR1, &sa, NULL);

    // idziemy po wszystkich argumentach po catcher_pid
    for (int i = 2; i < argc; i++) {
        // ustawianie wartosci do wyslania
        int task_to_send = atoi(argv[i]);
        printf("sending: %d\n", task_to_send);
        fflush(NULL);
        sigval_t sig_val = {task_to_send};

        // wysylanie
        sigqueue(catcher_pid, SIGUSR1, sig_val);

        // czekanie na zmiane ack_received przez handle_ack
        while (ack_received == 0) {
            continue;
        }
        ack_received = 0;
    }

    return (0);
}