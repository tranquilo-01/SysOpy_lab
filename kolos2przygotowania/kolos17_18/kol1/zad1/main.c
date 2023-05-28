#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void* hello(void* arg) {
    sleep(1);
    while (1) {
        printf("Hello world from thread number %d\n", *(int*)arg);
        /****************************************************
                przerwij dzialanie watku jesli bylo takie zadanie
        *****************************************************/
        pthread_testcancel();
        printf("Hello again world from thread number %d\n", *(int*)arg);
        sleep(2);
    }
    return NULL;
}

int main(int argc, char** args) {
    if (argc != 3) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    int n = atoi(args[1]);
    /**************************************************
        Utworz n watkow realizujacych funkcje hello
    **************************************************/

    pthread_t* threadIDs = malloc(sizeof(pthread_t) * n);
    int* threadArgs = malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        threadArgs[i] = i;
        pthread_create(&threadIDs[i], NULL, hello, &threadArgs[i]);
    }

    // ***********************************************

    int i = 0;
    while (i++ < atoi(args[2])) {
        printf("Hello from main %d\n", i);
        sleep(2);
    }

    /*******************************************
        "Skasuj" wszystke uruchomione watki i poczekaj na ich zakonczenie
    *******************************************/
    // printf("Joining threads\n");
    // for (int i = 0; i < n; i++) {
    // if (pthread_join(threadIDs[i], NULL) != 0) {
    //         fprintf(stderr, "Failed to join thread %d\n", i + 1);
    //     }
    // }
    // printf("Joined threads\n");

    for (int i = 0; i < n; i++) {
        // pthread_cancel(threadIDs[i]);
    }

    // ************************************************

    printf("DONE");

    return 0;
}
