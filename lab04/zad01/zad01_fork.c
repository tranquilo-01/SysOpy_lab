#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// handler wypisujacy sygnal
void handle_signal(int sig) {
    printf("Received signal %d\n", sig);
}

void check_pending(char* name) {
    sigset_t pending;
    sigpending(&pending);
    if (sigismember(&pending, SIGUSR1)) {
        printf("Signal %s pending\n", name);
    } else {
        printf("No signal %s pending\n", name);
    }
}

void set_signal(char* name, char* action) {
    struct sigaction sa;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    if (strcmp(action, "ignore") == 0) {
        sigaction(SIGUSR1, NULL, &sa);
        sa.sa_handler = SIG_IGN;
        sigaction(SIGUSR1, &sa, NULL);
        printf("Ignoring signal %s\n", name);
    } else if (strcmp(action, "handler") == 0) {
        sigaction(SIGUSR1, NULL, &sa);
        sa.sa_handler = handle_signal;
        sigaction(SIGUSR1, &sa, NULL);
        printf("Setting handler for signal %s\n", name);
    } else if (strcmp(action, "mask") == 0) {
        sigprocmask(SIG_BLOCK, &mask, NULL);
        printf("Masking signal %s\n", name);
    } else if (strcmp(action, "pending") == 0) {
        check_pending(name);
    } else {
        fprintf(stderr, "Unknown action %s\n", action);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ignore/handler/mask/pending>\n", argv[0]);
        return 1;
    }

    // ustawienie sygnalu w procesie przodka
    set_signal("parent", argv[1]);

    // wyslanie sygnalu do samego siebie (przodek)
    raise(SIGUSR1);

    // stworzenie potomka
    if (fork() == -1) {
        perror("Error while forking");
    }

    if (strcmp(argv[1], "pending") != 0) {
        raise(SIGUSR1);
    }
    sleep(1);

    return 0;
}
