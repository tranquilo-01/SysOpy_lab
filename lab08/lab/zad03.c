// zmodyfikuj poprzednie zadanie tak ze:
// w watku masz miec dwa miejsca drukowania komunikatu na konsole
// nastepnie program glowny zabija swoje watki w sposob synchroniczny
// po wypisaniu n razy swojego komuniaktu
// ustaw punkt synchronizazji na miejsce pomiedzy wypisaniem pierwszego i drugiego
// komunikatu na konsole

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUMBER 10

void* thread_function(void* arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    sleep(1);
    int number = *(int*)arg;
    int i = 0;

    while (1) {
        printf("Thread number %d, iteration, %d, first\n", number, i);
        sleep(2);  // sleep jest cancellation pointem
        printf("Thread number %d, iteration, %d, second\n", number, i);

        i++;
    }
}

int main() {
    pthread_t threads[THREAD_NUMBER];
    int thread_args[THREAD_NUMBER];

    for (int i = 0; i < THREAD_NUMBER; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
    }

    int n = 0;

    while (n < 3) {
        printf("Program glowny, iteracja %d\n", n);
        n++;
        sleep(2);
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        pthread_cancel(threads[i]);
    }

    printf("Ubito watki\n");
    sleep(5);
    printf("Quitting\n");
    return 0;
}
