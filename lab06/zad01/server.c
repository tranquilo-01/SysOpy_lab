#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include "common.h"

int clients[MAX_CLIENT_NUMBER];

int main() {
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
        clients[i] = -1;
    }

    // klucz serwera na podstawie sciezki katalogu domowego i stalej serverID
    const key_t serverKey = ftok(HOME_PATH, PROJECT_ID);

    // stworzenie kolejki komunikatow po stronie serwera
    const int serverQ = msgget(serverKey, IPC_CREAT | 0666);

    // bufor na przechowywanie odebranych wiadomosci
    struct msgbuf msgBuffer;

    while (1) {
        msgrcv(serverQ, &msgBuffer, MSG_BUFF_SIZE, 0, 0);
        printf("message type: %ld\n", msgBuffer.type);
    }
    return 0;
}