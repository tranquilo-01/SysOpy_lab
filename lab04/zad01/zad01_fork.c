#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// handler wypisujacy sygnal
void handle_signal(int sig) {
    printf("(%d): Received signal: %d\n", getpid(), sig);
}

void check_pending() {
    sigset_t pending;
    sigpending(&pending);
    if (sigismember(&pending, SIGUSR1)) {
        printf("(%d): signal is pending\n", getpid());
    } else {
        printf("(%d): no signal is pending\n", getpid());
    }
}

void set_and_raise_signal(char* action) {
    struct sigaction sa;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    if (strcmp(action, "ignore") == 0) {
        sigaction(SIGUSR1, NULL, &sa);
        sa.sa_handler = SIG_IGN;
        sigaction(SIGUSR1, &sa, NULL);
        printf("(%d): raise(SIGUSR1)\n", getpid());
        raise(SIGUSR1);
    } else if (strcmp(action, "handler") == 0) {
        sigaction(SIGUSR1, NULL, &sa);
        sa.sa_handler = handle_signal;
        sigaction(SIGUSR1, &sa, NULL);
        printf("(%d): raise(SIGUSR1)\n", getpid());
        raise(SIGUSR1);
    } else if (strcmp(action, "mask") == 0) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
        printf("(%d): raise(SIGUSR1)\n", getpid());
        raise(SIGUSR1);
    } else if (strcmp(action, "pending") == 0) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
        printf("(%d): raise(SIGUSR1)\n", getpid());
        raise(SIGUSR1);
        check_pending();
    } else {
        fprintf(stderr, "Unknown action %s\n", action);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ignore/handler/mask/pending>\n", argv[0]);
        return 1;
    }

    int parent_process = 1;

    // ustawienie sygnalu w procesie przodka
    set_and_raise_signal(argv[1]);
    fflush(NULL);

    // stworzenie potomka
    parent_process = fork();
    if (parent_process == -1) {
        perror("Error while forking");
    }

    if (!parent_process) {
        if (strcmp(argv[1], "pending") == 0) {
            check_pending();
        } else {
            printf("(%d): raise(SIGUSR1)\n", getpid());
            raise(SIGUSR1);
        }
    } else {
        wait(NULL);
    }
    return 0;
}
