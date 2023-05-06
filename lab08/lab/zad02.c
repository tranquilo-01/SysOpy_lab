//  zmodyfikuj poprzednie zadanie tak aby program glowny zabijal
//  w sposob asynchroniczmy wszystkie watki po wypisaniu na konsole swojego napisu 10 razy

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUMBER 10

void* thread_function(void* arg) {
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    sleep(1);
    int number = *(int*)arg;
    pthread_t tid = pthread_self();
    int i = 0;

    while (1) {
        printf("Thread number %d, tid %ld, iteration, %d\n", number, tid, i);
        i++;
        sleep(2);
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

    while (n < 10) {
        printf("Program glowny, iteracja %d\n", n);
        n++;
        sleep(2);
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        pthread_cancel(threads[i]);
    }

    printf("Ubito watki\n");
    sleep(3);
    printf("Quitting\n");
    return 0;
}