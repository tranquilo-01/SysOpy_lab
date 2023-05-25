#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define REINDEER_NUMBER 9
#define ELVES_NUMBER 10

#define ELVE_WORKING_TIME rand() % 4 + 2
#define PROBLEM_FIXING_TIME rand() % 2 + 1
#define REINDEER_VACATION_TIME rand() % 6 + 5
#define PRESENT_DELIVERY_TIME rand() % 3 + 2

#define REINDEERS_NEEDED 9
#define ELVES_NEEDED 3

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_waking_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t elve_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t elve_waking_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t reindeer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeer_delivering_cond = PTHREAD_COND_INITIALIZER;

int reindeersCameBack = 0;
int elvesWithProblem = 0;
int presentsDelivered = 0;
int elvesQueue[ELVES_NEEDED];
bool problemsBeingFixed = false;
bool presentsBeingDelivered = false;

void* santa(void* arg) {
    // printf("Santa locked\n");
    while (true) {
        // printf("In santa while\n");

        pthread_mutex_lock(&santa_mutex);
        while (reindeersCameBack != REINDEERS_NEEDED && elvesWithProblem != ELVES_NEEDED) {
            // printf("Santa sleeps\n");
            pthread_cond_wait(&santa_waking_cond, &santa_mutex);
        }
        pthread_mutex_unlock(&santa_mutex);

        printf("MIKOLAJ: budze sie\n");

        if (elvesWithProblem == ELVES_NEEDED) {
            problemsBeingFixed = true;

            pthread_mutex_lock(&elve_mutex);

            // fixing all elves problems
            for (int i = 0; i < ELVES_NEEDED; i++) {
                printf("MIKOLAJ: rozwiazuje problem elfa o id: %d\n", elvesQueue[i]);
                sleep(PROBLEM_FIXING_TIME);
            }
            // clearing the queue
            for (int i = 0; i < ELVES_NEEDED; i++) {
                elvesQueue[i] = 0;
            }
            elvesWithProblem = 0;

            pthread_mutex_unlock(&elve_mutex);
            problemsBeingFixed = false;
        }

        if (reindeersCameBack == REINDEERS_NEEDED) {
            pthread_mutex_lock(&reindeer_mutex);
            presentsBeingDelivered = true;
            pthread_cond_broadcast(&reindeer_delivering_cond);
            sleep(PRESENT_DELIVERY_TIME);
            presentsBeingDelivered = false;
            reindeersCameBack = 0;
            pthread_cond_broadcast(&reindeer_delivering_cond);
            pthread_mutex_unlock(&reindeer_mutex);
        }

        printf("MIKOLAJ: zasypiam\n");
    }
}

void* elve(void* arg) {
    int ID = *((int*)arg);
    // printf("ELVE STARTED: %d\n", ID);

    while (true) {
        // printf("ELVE %d sleeps\n", ID);
        sleep(ELVE_WORKING_TIME);
        // printf("ELVE %d woke up\n", ID);

        // if the queue is full and santa is fixing problems - go back to work
        // wydawalo by sie ze podczas tej operacji jak i podczas
        // wykonywania linii 65 mutex elfa powinien byc zablokowany, ale wtedy nie dziala
        // pthread_mutex_lock(&elve_mutex);
        if (problemsBeingFixed) {
            // pthread_mutex_unlock(&elve_mutex);
            continue;
        }

        pthread_mutex_lock(&elve_mutex);
        elvesQueue[elvesWithProblem] = ID;
        elvesWithProblem++;
        printf("ELF:     na Mikolaja czeka %d elfow, ID: %d\n", elvesWithProblem, ID);

        // waking santa up if the queue is full
        if (elvesWithProblem == ELVES_NEEDED) {
            printf("ELF:     wybudzam Mikolaja, ID: %d\n", ID);
            pthread_mutex_lock(&santa_mutex);
            pthread_cond_broadcast(&santa_waking_cond);
            pthread_mutex_unlock(&santa_mutex);
        }
        pthread_mutex_unlock(&elve_mutex);
    }
}

void* reindeer(void* arg) {
    int ID = *((int*)arg);

    while (true) {
        // printf("While reindeer %d", ID);
        pthread_mutex_lock(&reindeer_mutex);
        while (presentsBeingDelivered) {
            pthread_cond_wait(&reindeer_delivering_cond, &reindeer_mutex);
        }
        pthread_mutex_unlock(&reindeer_mutex);

        sleep(REINDEER_VACATION_TIME);
        pthread_mutex_lock(&reindeer_mutex);
        reindeersCameBack++;
        printf("RENIFER: czeka %d reniferow na Mikolaja, ID: %d\n", reindeersCameBack, ID);

        if (reindeersCameBack == REINDEERS_NEEDED) {
            printf("RENIFER: wybudzam Mikolaja, ID: %d\n", ID);
            pthread_mutex_lock(&santa_mutex);
            pthread_cond_broadcast(&santa_waking_cond);
            pthread_mutex_unlock(&santa_mutex);
        }
        pthread_mutex_unlock(&reindeer_mutex);
    }
}

int main() {
    setbuf(stdout, NULL);
    srand(time(NULL));

    for (int i = 0; i < ELVES_NEEDED; i++) {
        elvesQueue[i] = 0;
    }

    // creating santa thread
    pthread_t santaThread;
    pthread_create(&santaThread, NULL, &santa, NULL);

    // creating elves threads
    int* elvesIDs = calloc(ELVES_NUMBER, sizeof(int));
    pthread_t* elvesThreads = calloc(ELVES_NUMBER, sizeof(pthread_t));
    for (int i = 0; i < ELVES_NUMBER; i++) {
        elvesIDs[i] = i;
        pthread_create(&elvesThreads[i], NULL, &elve, &elvesIDs[i]);
    }

    // creating reindeers threads
    int* reindeersIDs = calloc(REINDEER_NUMBER, sizeof(int));
    pthread_t* reindeersThreads = calloc(REINDEER_NUMBER, sizeof(pthread_t));
    for (int i = 0; i < REINDEER_NUMBER; i++) {
        reindeersIDs[i] = i;
        pthread_create(&reindeersThreads[i], NULL, &reindeer, &reindeersIDs[i]);
    }

    pthread_join(santaThread, NULL);

    for (int i = 0; i < ELVES_NUMBER; i++) {
        pthread_join(elvesThreads[i], NULL);
    }

    for (int i = 0; i < REINDEER_NUMBER; i++) {
        pthread_join(reindeersThreads[i], NULL);
    }

    return 0;
}