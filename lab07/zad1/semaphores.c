#include "semaphores.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

Semaphore init_sem(char* name, int amount, int initial_value) {
    key_t key = ftok(getenv("HOME"), name[0]);
    if (key == -1) {
        perror("ftok: ");
        return 0;
    }

    Semaphore sem_id = semget(key, amount, 0664 | IPC_CREAT);
    if (sem_id == -1) {
        perror("semget: ");
        return 0;
    }

    for (int i = 0; i < amount; i++) {
        if (semctl(sem_id, i, SETVAL, initial_value) == -1) {
            perror("semctl: ");
            return 0;
        }
    }

    return sem_id;
}

Semaphore get_sem(char* name, int amount) {
    key_t key = ftok(getenv("HOME"), name[0]);
    if (key == -1) {
        perror("ftok: ");
        return 0;
    }

    Semaphore sem_id = semget(key, amount, 0);
    if (sem_id == -1) {
        return 0;
    }
    return sem_id;
}

void increment_sem(Semaphore sem_id, int room_id) {
    struct sembuf ops = {room_id, 1, 0};
    if (semop(sem_id, &ops, 1)) {
        perror("increment_sem");
    }
}

void wait_sem(Semaphore sem_id, int room_id) {
    struct sembuf ops = {room_id, 0, 0};
    if (semop(sem_id, &ops, 1)) {
        printf("%d\n", room_id);
        perror("wait");
    }
}

void decrement_sem(Semaphore sem_id, int room_id) {
    struct sembuf ops = {room_id, -1, 0};
    if (semop(sem_id, &ops, 1)) {
        perror("decrement_sem");
    }
}

void remove_sem(char* name, int amount) {
    Semaphore sem_id = get_sem(name, amount);
    semctl(sem_id, 0, IPC_RMID);
}
