// zmodyfikup poprzednie w taki sposob ze zmienisz atrybuty mutexu idpowiedno na
// normal
// errorcheck
// recursive
// default

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

int global = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* thread_function(void* arg){
    pthread_mutex_lock(&mtx);
    global++;
    printf("global incremented to %d by thread %ld\n", global, pthread_self());
    pthread_mutex_unlock(&mtx);
}


int main(int argc, char** argv){

    thread_function(NULL);
    return 0;
    for(int i = 0; i < 10; i++){
        pthread_t* thread;
        pthread_create(thread, NULL, thread_function, NULL);
    }

}