#include <pthread.h>
#include <signal.h>
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

int renideersCameBack = 0;
int elvesHaveProblem = 0;
int presentsDelivered = 0;

void* santa(void* arg) {
    pthread_mutex_lock(&santa_mutex);
    while (renideersCameBack != REINDEERS_NEEDED || elvesHaveProblem != ELVES_NEEDED) {
        pthread_cond_wait(&santa_waking_cond, &santa_mutex);
    }
    pthread_mutex_unlock(&santa_mutex);
}