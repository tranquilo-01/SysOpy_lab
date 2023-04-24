#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include "common.h"

// tablica pod id klienta przechowuje identyfikator jego kolejki
int clients[MAX_CLIENT_NUMBER];
int activeClients = 0;

void handleInit(msgbuf* buffer) {
    printf("Handling init: %d\n", buffer->clientKey);

    if (activeClients >= MAX_CLIENT_NUMBER) {
        printf("The maximum number of clients has been reached");
        return;
    }

    // szukanie pierwszego wolnego id
    int idx = 0;
    while (clients[idx] != -1) {
        idx++;
    }

    // uzyskanie dostepu do kolejki klienta
    int newClientQ = msgget(buffer->clientKey, 0);

    // przypisanie identyfikatora kolejki pod odpowiedni indeks tablicy
    clients[idx] = newClientQ;

    // wyslanie potwierdzenia o przyznaniu id do klienta
    struct msgbuf message = {.type = INIT, .text = 0, .clientID = idx, .clientKey = -1};
    msgsnd(newClientQ, &message, MSG_BUFF_SIZE, 0);
}

void handleStop(msgbuf* buffer) {
    printf("handling stop: %ld\n", buffer->type);
}

void handleList(msgbuf* buffer) {
    printf("handling list: %ld\n", buffer->type);
}

void handleTall(msgbuf* buffer) {
    printf("handling tall: %ld\n", buffer->type);
}

void handleTone(msgbuf* buffer) {
    printf("handling tone: %ld\n", buffer->type);
}

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
        // funkcja oczekuje az wiadomosc pojawi sie w kolejce i odbiera ja
        msgrcv(serverQ, &msgBuffer, MSG_BUFF_SIZE, 0, 0);

        switch (msgBuffer.type) {
            case INIT:
                handleInit(&msgBuffer);
                break;
            case STOP:
                handleStop(&msgBuffer);
                break;
            case LIST:
                handleList(&msgBuffer);
                break;
            case TALL:
                handleTall(&msgBuffer);
                break;
            case TONE:
                handleTone(&msgBuffer);
                break;
        }
    }
    return 0;
}