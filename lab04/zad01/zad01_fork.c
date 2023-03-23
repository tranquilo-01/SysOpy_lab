#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void set_signal(char* name, int action) {
    struct sigaction;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    if (action == 0) {  // ignore
        sigaction(SIGUSR1, NULL, &sa);
        sa.sa_handler = SIG_IGN;
        sigaction(SIGUSR1, &sa, NULL);
        printf("Ignoring signal %s\n", name);
    } else if (action == 1) {  // handler
        sigaction(SIGUSR1, NULL, &sa);
        sa.sa_handler = handle_signal;
        sigaction(SIGUSR1, &sa, NULL);
        printf("Setting handler for signal %s\n", name);
    } else if (action == 2) {  // mask
        sigprocmask(SIG_BLOCK, &mask, NULL);
        printf("Masking signal %s\n", name);
    } else if (action == 3) {  // pending
        check_pending(name);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <ignore/handler/mask/pending>\n", argv[0]);
        return 1;
    }
    char* action_name = argv[1];
    int action = -1;
    if (strcmp(action_name, "ignore") == 0) {
        action = 0;
    } else if (strcmp(action_name, "handler") == 0) {
        action = 1;
    } else if (strcmp(action_name, "mask") == 0) {
        action = 2;
    } else if (strcmp(action_name, "pending") == 0) {
        action = 3;
    } else {
        fprintf(stderr, "Unknown action %s\n", action_name);
        return 1;
    }

    set_signal("parent", action);

    if (fork() == 0) {  // child process
        set_signal("child", action);
        if (action == 3) {  // pending
            raise(SIGUSR1);
        }
        printf("Child process exiting\n");
        return 0;
    }

    if (action != 3) {  // not pending
        raise(SIGUSR1);
    }

    sleep(1);

    return 0;
}
