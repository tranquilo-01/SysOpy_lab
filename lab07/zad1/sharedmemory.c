#include "sharedmemory.h"
#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int get_shm(char* name, int size) {
    key_t key = ftok(getenv("HOME"), name[0]);
    if (key == -1) {
        perror("ftok");
        return -1;
    }
    return shmget(key, size, 0664 | IPC_CREAT);
}

char* attach_shm(char* name, int size) {
    int shm_id = get_shm(name, size);
    if (shm_id == -1) {
        perror("attach_shm");
        return NULL;
    }

    char* shm = shmat(shm_id, NULL, 0);

    if (shm == (char*)(-1)) {
        perror("connect_shared");
        return NULL;
    }
    return shm;
}

void detach_shm(char* shm) {
    shmdt(shm);
}

void remove_shm(char* name) {
    int shm_id = get_shm(name, 0);
    if (shm_id == -1)
        return;
    shmctl(shm_id, IPC_RMID, NULL);
}
