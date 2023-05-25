// napisz program w ktorym stwowrzysz n watkow
// po stworzeniu program glowny zapetlamy i inkrememntyjemy w petli zmienna globalna
// watki powinny takze inkrementowac w petli wartosc zmiennaj globalnej
// ale powinny zaczac to robic dopiero po uzyskaniu przez zmienna globalna wartosci wiekszej niz 1000
// do realizacji wykorzystaj mechanizm zmiennych warunkowych

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