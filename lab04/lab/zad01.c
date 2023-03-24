#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handle_signal(int sig) {
    printf("ja cie ale sygnal");
}

int main(){
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigaction(SIGUSR1, &sa, NULL);
    raise(SIGUSR1);

    return 0;

}