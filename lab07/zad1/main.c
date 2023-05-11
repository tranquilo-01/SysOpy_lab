#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"
#include "semaphores.h"
#include "sharedmemory.h"

// pointers for shared memeory segments
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
    lobby_sem = init_sem(LOBBY_SEM_NAME, LOBBY_CAPACITY, 0);
    seats_sem = init_sem(SEATS_SEM_NAME, SEATS_NUMBER, 0);
}

void destroy_semas() {
    remove_sem(LOBBY_SEM_NAME, LOBBY_CAPACITY);
    remove_sem(SEATS_SEM_NAME, SEATS_NUMBER);
}

int main() {
    // creating new shared memory segments
    create_shm();

    // preparing an array for each place in lobby
    for (int i = 0; i < LOBBY_CAPACITY; i++) {
        lobby_shm[i] = (char)0;
        reserved_shm[i] = (char)0;
    }
    lobby_shm[LOBBY_CAPACITY] = '\0';
    reserved_shm[LOBBY_CAPACITY] = '\0';

    // preparing an array for each seat in barbershop
    for (int i = 0; i < SEATS_NUMBER; i++) {
        seats_shm[i] = (char)0;
    }
    seats_shm[SEATS_NUMBER] = '\0';

    // creating semaphores
    destroy_semas();
    create_semas();

    // spawning barbers
    for (int i = 0; i < BARBER_NUMBER; i++) {
        if (fork() == 0)
            execl(BARBER_EXE, BARBER_EXE, NULL);
    }

    // spawning clients
    for (int i = 0; i < CLIENT_NUMBER; i++) {
        if (fork() == 0)
            execl(CLIENT_EXE, CLIENT_EXE, NULL);
    }

    // waiting for every process to finish its job
    while (wait(NULL) > 0)
        ;

    // destroying used resources and quitting
    destroy_shm();
    destroy_semas();
    return 0;
}
