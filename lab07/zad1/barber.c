#include <stdio.h>
#include <unistd.h>
#include "common.h"
#include "semaphores.h"
#include "sharedmemory.h"

char* lobby_shm;
char* seats_shm;
char* move_shm;
char* reserved_shm;

Semaphore lobby_sem;
Semaphore seats_sem;

void create_shm() {
    lobby_shm = attach_shm(LOBBY_QUEUE_NAME, QUEUE_SIZE);
    seats_shm = attach_shm(SEATS_QUEUE_NAME, QUEUE_SIZE);
    move_shm = attach_shm(MOVE_QUEUE_NAME, QUEUE_SIZE);
    reserved_shm = attach_shm(MOVE_QUEUE_NAME, QUEUE_SIZE);
}

void destroy_shm() {
    remove_shm(lobby_shm);
    remove_shm(seats_shm);
    remove_shm(move_shm);
    remove_shm(reserved_shm);
}

void create_semas() {
    lobby_sem = get_sem(LOBBY_SEM_NAME, LOBBY_CAPACITY);
    seats_sem = get_sem(SEATS_SEM_NAME, SEATS_NUMBER);
}

int not_found = 0;

int main() {
    create_shm();
    create_semas();

    printf("Barber o id: %d zaczal prace!\n", getpid());

    while (1) {
        int found = 0;
        for (int i = 0; i < LOBBY_CAPACITY; i++) {
            if (lobby_shm[i] != (char)0 && reserved_shm[i] == (char)0) {
                reserved_shm[i] = (char)1;
                found = 1;
                int haircut = lobby_shm[i];

                for (int j = 0; j < SEATS_NUMBER; j++) {
                    if (seats_shm[j] == (char)0) {
                        seats_shm[j] = (char)1;

                        increment_sem(seats_sem, j);
                        move_shm[i] = (char)(j);
                        decrement_sem(lobby_sem, i);
                        sleep(haircut);
                        decrement_sem(seats_sem, j);
                        seats_shm[j] = (char)0;
                        break;
                    }
                }
                reserved_shm[i] = (char)0;
            }
        }
        if (!found)
            ++not_found;
        if (not_found >= 5)
            break;
        sleep(1);
    }

    destroy_shm();
    printf("Barber o id: %d zakonczyl prace!\n", getpid());

    return 0;
}
