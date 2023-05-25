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
pthread_mutex_t santa_sleep_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_waking_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t elvesWithProblem_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t reindeer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeer_delivering_cond = PTHREAD_COND_INITIALIZER;

int reindeersCameBack = 0;
int elvesWithProblem = 0;
int presentsDelivered = 0;
int elvesQueue[ELVES_NEEDED];
bool presentsBeingDelivered = false;

void* santa(void* arg) {
    while (true) {
        pthread_mutex_lock(&santa_sleep_mutex);
        while (reindeersCameBack != REINDEERS_NEEDED && elvesWithProblem != ELVES_NEEDED) {
            pthread_cond_wait(&santa_waking_cond, &santa_sleep_mutex);
        }
        pthread_mutex_unlock(&santa_sleep_mutex);

        printf("MIKOLAJ: budze sie\n");

        pthread_mutex_lock(&elvesWithProblem_mutex);
        if (elvesWithProblem == ELVES_NEEDED) {
            // fixing all elves problems
            printf("MIKOLAJ: rozwiazuje problem elfow o id: %d, %d, %d\n", elvesQueue[0], elvesQueue[1], elvesQueue[2]);
            sleep(PROBLEM_FIXING_TIME);

            // clearing the queue
            for (int i = 0; i < ELVES_NEEDED; i++) {
                elvesQueue[i] = 0;
            }
            elvesWithProblem = 0;
        }
        pthread_mutex_unlock(&elvesWithProblem_mutex);

        if (reindeersCameBack == REINDEERS_NEEDED) {
            presentsBeingDelivered = true;
            presentsBeingDelivered = false;
            reindeersCameBack = 0;
        }

        printf("MIKOLAJ: zasypiam\n");
    }
}

void* elve(void* arg) {
    int ID = *((int*)arg);

    while (true) {
        sleep(ELVE_WORKING_TIME);

        pthread_mutex_lock(&elvesWithProblem_mutex);
        if (elvesWithProblem == ELVES_NEEDED) {
            pthread_mutex_unlock(&elvesWithProblem_mutex);
            continue;
        }

        elvesQueue[elvesWithProblem] = ID;
        elvesWithProblem++;
        printf("ELF:     na Mikolaja czeka %d elfow, ID: %d\n", elvesWithProblem, ID);

        // waking santa up if the queue is full
        pthread_mutex_lock(&santa_sleep_mutex);
        if (elvesWithProblem == ELVES_NEEDED) {
            printf("ELF:     wybudzam Mikolaja, ID: %d\n", ID);
            pthread_cond_broadcast(&santa_waking_cond);
        }
        pthread_mutex_unlock(&santa_sleep_mutex);
        pthread_mutex_unlock(&elvesWithProblem_mutex);
    }
}

void* reindeer(void* arg) {
    int ID = *((int*)arg);

    while (true) {
        // printf("While reindeer %d", ID);
        while (presentsBeingDelivered) {
            pthread_cond_wait(&reindeer_delivering_cond, &reindeer_mutex);
        }

        sleep(REINDEER_VACATION_TIME);
        reindeersCameBack++;
        printf("RENIFER: czeka %d reniferow na Mikolaja, ID: %d\n", reindeersCameBack, ID);

        if (reindeersCameBack == REINDEERS_NEEDED) {
            printf("RENIFER: wybudzam Mikolaja, ID: %d\n", ID);
        }
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
    // int* reindeersIDs = calloc(REINDEER_NUMBER, sizeof(int));
    // pthread_t* reindeersThreads = calloc(REINDEER_NUMBER, sizeof(pthread_t));
    // for (int i = 0; i < REINDEER_NUMBER; i++) {
    //     reindeersIDs[i] = i;
    //     pthread_create(&reindeersThreads[i], NULL, &reindeer, &reindeersIDs[i]);
    // }

    pthread_join(santaThread, NULL);

    for (int i = 0; i < ELVES_NUMBER; i++) {
        pthread_join(elvesThreads[i], NULL);
    }

    // for (int i = 0; i < REINDEER_NUMBER; i++) {
    //     pthread_join(reindeersThreads[i], NULL);
    // }

    return 0;
}