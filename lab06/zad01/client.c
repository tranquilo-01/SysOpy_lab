#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include "common.h"

int clientID = -1;
int serverQ;
int clientQ;
char timeBuff[TIME_BUFF_SIZE];

void sendList() {
    struct msgbuf message = {.type = LIST, .clientID = clientID};
    msgsnd(serverQ, &message, MSG_BUFF_SIZE, 0);
}

void send2All(char* text) {
    getTime(timeBuff);
    struct msgbuf message = {
        .type = TALL,
        .clientID = clientID,
    };
    strcpy(message.text, text);
    msgsnd(serverQ, &message, MSG_BUFF_SIZE, 0);
}

void send2One(char* text) {
    struct msgbuf message = {.type = TONE, .clientID = clientID};
    strcpy(message.text, text);
    msgsnd(serverQ, &message, MSG_BUFF_SIZE, 0);
}

void sendStop() {
    struct msgbuf message = {.type = STOP, .clientID = clientID};
    msgsnd(serverQ, &message, MSG_BUFF_SIZE, 0);

    msgctl(clientQ, IPC_RMID, 0);
    exit(0);
}

int main() {
    // uzyskanie dostepu do kolejki komunikatow po stronie serwera
    const key_t serverKey = ftok(HOME_PATH, PROJECT_ID);
    serverQ = msgget(serverKey, 0);

    signal(SIGINT, sendStop);

    // stworzenie kolejki po stronie klienta
    srand(time(NULL));
    const key_t clientKey = ftok(HOME_PATH, rand() % 255 + 1);
    // printf("%d\n", clientKey);
    clientQ = msgget(clientKey, IPC_CREAT | 0666);
    // printf("clientQ: %d\n", clientQ);

    // bufor na inicjalna wiadomosc i wyslanie jej
    struct msgbuf initMessage = {.type = INIT, .text = 0, .clientID = -1, .clientKey = clientKey};
    msgsnd(serverQ, &initMessage, MSG_BUFF_SIZE, 0);

    struct msgbuf receivedMessage;
    char orderBuff[ORDER_BUFF_SIZE];
    char* text;

    while (1) {
        if (clientID == -1) {
            // jezeli nie ma przyznanego id to czeka na wiadomosc zwrotna typu 1, czyli init
            msgrcv(clientQ, &receivedMessage, MSG_BUFF_SIZE, 0, 0);
            clientID = receivedMessage.clientID;
            printf("ID set to: %d\n", clientID);
        } else {
            printf("Type the order: ");
            fgets(orderBuff, ORDER_BUFF_SIZE, stdin);

            if (strncmp(orderBuff, "LIST", 4) == 0) {
                printf("sending list\n");
                sendList();
            } else if (strncmp(orderBuff, "2ALL", 4) == 0) {
                // wskaznik na pierwszy char stringa do wyslania
                text = &orderBuff[5];
                send2All(text);
            } else if (strncmp(orderBuff, "2ONE", 4) == 0) {
                text = &orderBuff[5];
                send2One(text);
            } else if (strncmp(orderBuff, "STOP", 4) == 0) {
                sendStop();
            } else {
                printf("Unknown order\n");
            }
        }
    }
    return 0;
}