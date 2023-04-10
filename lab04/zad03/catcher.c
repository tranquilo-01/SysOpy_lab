#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int request_count = 0;

void print_current_time() {
    static time_t raw_time;
    static struct tm* time_info;
    time(&raw_time);
    time_info = localtime(&raw_time);
    printf("Catcher: Current time: %s", asctime(time_info));
}

void print_numbers() {
    for (int i = 1; i <= 100; i++)
        printf("Catcher: %i\n", i);
}

void print_request_number() {
    printf("Catcher: Number of requests: %d\n", request_count);
}

void finish() {
    printf("Catcher: Closing\n");
    exit(0);
}

void send_ack(int sender_pid) {
    kill(sender_pid, SIGUSR1);
}

void handler(int signo, siginfo_t* info, void* context) {
    int sender_pid = info->si_pid;
    int task = info->si_status;

    send_ack(sender_pid);

    printf("Catcher: Handling task: %d\n", task);

    request_count++;

    switch (task) {
        case 1:
            print_numbers();
            break;
        case 2:
            print_current_time();
            break;
        case 3:
            print_request_number();
            break;
        case 5:
            finish();
            break;
        default:
            fprintf(stderr, "Invalid task number\n");
            exit(1);
            break;
    }
    printf("\n");
}

int main(int argc, char** argv) {
    printf("Catcher: %d\n\n", getpid());

    // ustawienie handlera
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);

    // oczekiwanie na sygnal
    while (1) {
        continue;
    }

    return 0;
}