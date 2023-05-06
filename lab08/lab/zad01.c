// napisz program w ktorym toworzysz n watkow w ktorym kazdy watek
// w nieskonczonej petli wypisuje stwoj tid i iteracej
// dodatkowo po stworzeniu wszystkich watkow program glowny w nieskonczonej
// petli wypisuje na konsole napis program glowyny iteracja n

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_NUMBER 10

void* thread_function(void* arg) {
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

    while (1) {
        printf("Program glowny, iteracja %d\n", n);
        n++;
        sleep(2);
    }
    return 0;
}