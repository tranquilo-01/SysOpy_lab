// napisz program w ktorym stworzysz n watkow
// po stworzeniu program glowny zapetlamy i inkrememntujemy w petli zmienna globalna
// watki powinny takze inkrementowac w petli wartosc zmiennaj globalnej
// ale powinny zaczac to robic dopiero po uzyskaniu przez zmienna globalna wartosci wiekszej niz 1000
// do realizacji wykorzystaj mechanizm zmiennych warunkowych

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define THREAD_NUMBER 5
#define TARGET_VALUE 4

int global = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* thread_function(void* arg) {
    pthread_mutex_lock(&mutex);
    while (global <= TARGET_VALUE) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    while (1) {
        // printf("Thread iterating\n");
        pthread_mutex_lock(&mutex);
        // printf("Mutex locked in thread\n");
        global++;
        pthread_mutex_unlock(&mutex);
        // printf("Mutex unlocked in thread\n");
        printf("Thread: Incrementing global to %d\n", global);
        pthread_cond_signal(&cond);

        sleep(1);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_NUMBER];

    for (int i = 0; i < THREAD_NUMBER; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    while (1) {
        // printf("Main iterating\n");
        pthread_mutex_lock(&mutex);
        // printf("Mutex locked in main\n");
        global++;
        pthread_mutex_unlock(&mutex);
        // printf("Mutex unlocked in main\n");
        printf("Main: Incrementing global to %d\n", global);

        pthread_cond_broadcast(&cond);
        sleep(1);
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}