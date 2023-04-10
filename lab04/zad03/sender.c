#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Sender: invalid number of arguments. Catcher id and tasks expected\n");
        exit(1);
    }

    int catcher_pid = atoi(argv[1]);
    printf("%d\n", catcher_pid);

    // ustawianie wartosci do wyslania
    sigval_t sig_val = {1};
    // wysylanie
    sigqueue(catcher_pid, SIGUSR1, sig_val);
    printf("signal sent\n");

    return (0);
}