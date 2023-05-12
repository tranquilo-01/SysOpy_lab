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

void attach_shms() {
    lobby_shm = attach_shm(LOBBY_QUEUE_NAME, QUEUE_SIZE);
    seats_shm = attach_shm(SEATS_QUEUE_NAME, QUEUE_SIZE);
    move_shm = attach_shm(MOVE_QUEUE_NAME, QUEUE_SIZE);
    reserved_shm = attach_shm(MOVE_QUEUE_NAME, QUEUE_SIZE);
}

void detach_shms() {
    detach_shm(lobby_shm);
    detach_shm(seats_shm);
    detach_shm(move_shm);
    detach_shm(reserved_shm);
}

void get_semas() {
    lobby_sem = get_sem(LOBBY_SEM_NAME, LOBBY_CAPACITY);
    seats_sem = get_sem(SEATS_SEM_NAME, SEATS_NUMBER);
}

int not_found = 0;

int main() {
    // getting semaphores and shared memory segments created in main
    attach_shms();
    get_semas();

    printf("Barber: %d started\n", getpid());

    while (1) {
        int found = 0;
        for (int i = 0; i < LOBBY_CAPACITY; i++) {
            // if client is in lobby and no other barber is looking for the seat for the client
            if (lobby_shm[i] != (char)0 && reserved_shm[i] == (char)0) {
                // mark that the barber is looking for a place for the client
                reserved_shm[i] = (char)1;
                found = 1;
                int haircut = lobby_shm[i];

                // look for a place for the client
                for (int j = 0; j < SEATS_NUMBER; j++) {
                    // if seat is free mark it as taken
                    if (seats_shm[j] == (char)0) {
                        seats_shm[j] = (char)1;

                        // incrementing semaphore related to the found seat, decrementing when finished cutting
                        increment_sem(seats_sem, j);
                        // setting that the client from place i in lobby was moced to seat j in barbershop
                        move_shm[i] = (char)(j);
                        // letting the client to move from lobby to the seat
                        decrement_sem(lobby_sem, i);
                        sleep(haircut);
                        // letting the client go with a fresh haircut
                        decrement_sem(seats_sem, j);
                        // freeing the seat in the barbershop
                        seats_shm[j] = (char)0;
                        break;
                    }
                }
                // un-reserving the place in the barbershop
                reserved_shm[i] = (char)0;
            }
        }
        // if a new client wasnt found for 5 seconds, end the process
        if (!found)
            ++not_found;
        if (not_found >= 5)
            break;
        sleep(1);
    }

    detach_shms();
    printf("Barber: %d ended his job\n", getpid());

    return 0;
}
