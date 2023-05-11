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

void create_shm() {
    lobby_shm = attach_shm(LOBBY_QUEUE_NAME, QUEUE_SIZE);
    move_shm = attach_shm(MOVE_QUEUE_NAME, QUEUE_SIZE);
}

void destroy_shm() {
    remove_shm(lobby_shm);
    remove_shm(move_shm);
}

void create_semas() {
    lobby_sem = get_sem(LOBBY_SEM_NAME, LOBBY_CAPACITY);
    seats_sem = get_sem(SEATS_SEM_NAME, SEATS_NUMBER);
}

int main() {
    srand(time(NULL) + getpid());

    // getting semaphores and shared memory segments created in main
    create_shm();
    create_semas();

    printf("Client: %d started\n", getpid());

    int found = 0;
    // looking for a free place in the lobby
    for (int i = 0; i < LOBBY_CAPACITY; i++) {
        if (lobby_shm[i] == (char)0) {
            found = 1;
            // generating a random haircut
            lobby_shm[i] = (char)(rand() % 8 + 1);

            increment_sem(lobby_sem, i);
            wait_sem(lobby_sem, i);

            int seat_id = move_shm[i];
            lobby_shm[i] = (char)0;
            wait_sem(seats_sem, seat_id);
            break;
        }
    }
    destroy_shm();

    if (!found) {
        printf("There is no free place in the lobby, client has left\n");
    } else {
        printf("Client %d finished with a fresh haicut\n", getpid());
    }

    return 0;
}
