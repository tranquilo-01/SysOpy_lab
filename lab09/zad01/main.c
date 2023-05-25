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
pthread_cond_t reindeer_waking_cond = PTHREAD_COND_INITIALIZER;

int renideersCameBack = 0;
int elvesWithProblem = 0;
int presentsDelivered = 0;
int elvesQueue[ELVES_NEEDED];
bool problemsBeingFixed = false;

void* santa(void* arg) {
    // printf("Santa locked\n");
    while (true) {
        // printf("In santa while\n");

        pthread_mutex_lock(&santa_mutex);
        while (renideersCameBack != REINDEERS_NEEDED && elvesWithProblem != ELVES_NEEDED) {
            // printf("Santa sleeps\n");
            pthread_cond_wait(&santa_waking_cond, &santa_mutex);
        }
        pthread_mutex_unlock(&santa_mutex);

        printf("MIKOLAJ: budze sie\n");

        pthread_mutex_lock(&elve_mutex);
        problemsBeingFixed = true;
        if (elvesWithProblem == ELVES_NEEDED) {
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
        }
        printf("MIKOLAJ: zasypiam\n");
        pthread_mutex_unlock(&elve_mutex);
        problemsBeingFixed = false;
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

    pthread_join(santaThread, NULL);

    for (int i = 0; i < ELVES_NUMBER; i++) {
        pthread_join(elvesThreads[i], NULL);
    }

    return 0;
}