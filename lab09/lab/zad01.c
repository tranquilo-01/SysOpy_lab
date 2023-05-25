// napisz program w ktorym uruchomisz n watkow realizujacych te sama funkcje
// w ramach swojego dzialania watki maja za zadanie inkrementowac wartosc zmiennej globalnej
// i wypiswac jej watosc na konsole wraz z informacja identyfikujaca watek dokonujacy wypisania
// dostep do zmiennej zabezpiecz mutexem

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#define THREAD_NUMBER 12

int global = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_t* threads;

void* thread_function(void* arg){
    pthread_mutex_lock(&mtx);
    global++;
    printf("global incremented to %d by thread %ld\n", global, pthread_self());
    pthread_mutex_unlock(&mtx);

    pthread_exit(NULL);

}


int main(int argc, char** argv){

    threads = malloc(THREAD_NUMBER*sizeof(pthread_t));

    for(int i = 0; i < THREAD_NUMBER; i++){
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    for (int i = 0; i < THREAD_NUMBER; i++) {
        pthread_join(threads[i], NULL);
    }

}