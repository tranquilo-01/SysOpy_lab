#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>
#include "common.h"
#include "semaphores.h"
#include "sharedmemory.h"

char* lobby_shm;
char* move_shm;

Semaphore lobby_sem;
Semaphore seats_sem;

void attach_shms() {
    lobby_shm = attach_shm(LOBBY_QUEUE_NAME, QUEUE_SIZE);
    move_shm = attach_shm(MOVE_QUEUE_NAME, QUEUE_SIZE);
}

void detach_shms() {
    detach_shm(lobby_shm);
    detach_shm(move_shm);
}

void get_semas() {
    lobby_sem = get_sem(LOBBY_SEM_NAME, LOBBY_CAPACITY);
    seats_sem = get_sem(SEATS_SEM_NAME, SEATS_NUMBER);
}

int main() {
    srand(time(NULL) + getpid());

    // getting semaphores and shared memory segments created in main
    attach_shms();
    get_semas();

    printf("Client: %d started\n", getpid());

    int found = 0;
    // looking for a free place in the lobby
    for (int i = 0; i < LOBBY_CAPACITY; i++) {
        if (lobby_shm[i] == (char)0) {
            found = 1;
            // generating a random haircut
            lobby_shm[i] = (char)(rand() % 8 + 1);

            // incrementing semaphore related to the place taken in lobby
            // and waiting till its value is set to 0
            increment_sem(lobby_sem, i);
            wait_sem(lobby_sem, i);

            // moving to the seat found by the braber
            int seat_id = move_shm[i];
            lobby_shm[i] = (char)0;

            // waiting for the barber to finish cutting
            wait_sem(seats_sem, seat_id);
            break;
        }
    }

    detach_shms();

    if (!found) {
        printf("There is no free place in the lobby, client %d has left\n", getpid());
    } else {
        printf("Client %d finished with a fresh haicut\n", getpid());
    }

    return 0;
}
