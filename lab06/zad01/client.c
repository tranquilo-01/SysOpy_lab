#include <pthread.h>
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

void send2All(char* input) {
    getTime(timeBuff);
    char* text = &input[5];

    struct msgbuf message = {
        .type = TALL,
        .clientID = clientID,
    };
    strcpy(message.text, text);
    msgsnd(serverQ, &message, MSG_BUFF_SIZE, 0);
}

void send2One(char* input) {
    char* text = &input[5];
    int recipientID = atoi(strtok(text, " "));
    char* content = &input[7];
    struct msgbuf message = {.type = TONE, .clientID = clientID, .recipientID = recipientID};
    strcpy(message.text, content);
    msgsnd(serverQ, &message, MSG_BUFF_SIZE, 0);
}

void sendStop() {
    struct msgbuf message = {.type = STOP, .clientID = clientID};
    msgsnd(serverQ, &message, MSG_BUFF_SIZE, 0);

    msgctl(clientQ, IPC_RMID, 0);
    exit(0);
}

void handleReceivedMessage(msgbuf* buffer) {
    printf("\nReceived message from client: %d:\n", buffer->clientID);
    printf("%s", buffer->text);
    printf("Type the order: ");
}

void handleStop() {
    printf("\nSTOPPING\n");
    msgctl(clientQ, IPC_RMID, 0);
    exit(0);
}

// funkcja zajmujaca sie obsluga nadawanych polecen dzialajaca na osobnym watku
void* orderHandlerThread(void* arg) {
    char orderBuff[ORDER_BUFF_SIZE];
    while (1) {
        printf("Type the order: ");
        fgets(orderBuff, ORDER_BUFF_SIZE, stdin);

        if (strncmp(orderBuff, "LIST", 4) == 0) {
            sendList();
        } else if (strncmp(orderBuff, "2ALL", 4) == 0) {
            send2All(orderBuff);
        } else if (strncmp(orderBuff, "2ONE", 4) == 0) {
            send2One(orderBuff);
        } else if (strncmp(orderBuff, "STOP", 4) == 0) {
            sendStop();
        } else {
            printf("Unknown order\n");
        }
    }
}

// funkcja zajmujaca sie obsluga otrzymanych wiadomosci dzialajaca na osobnym watku
void* messageHandlerThread(void* arg) {
    struct msgbuf receivedMessage;

    // w petli czeka na wiadomosc i odczytuje ja ze swojej kolejki
    while (1) {
        msgrcv(clientQ, &receivedMessage, MSG_BUFF_SIZE, 0, 0);
        switch (receivedMessage.type) {
            case TALL:
            case TONE:
                handleReceivedMessage(&receivedMessage);
                break;
            case STOP:
                handleStop();
                break;
        }
    }
}

int main() {
    // wylaczenie buforowania bo sie nie wypisywalo
    setvbuf(stdout, NULL, _IONBF, 0);

    // uzyskanie dostepu do kolejki komunikatow po stronie serwera
    const key_t serverKey = ftok(HOME_PATH, PROJECT_ID);
    serverQ = msgget(serverKey, 0);

    signal(SIGINT, sendStop);

    // stworzenie kolejki po stronie klienta
    srand(time(NULL));
    const key_t clientKey = ftok(HOME_PATH, rand() % 255 + 1);
    clientQ = msgget(clientKey, IPC_CREAT | 0666);

    // bufor na inicjalna wiadomosc i wyslanie jej
    struct msgbuf initMessage = {.type = INIT, .text = 0, .clientID = -1, .clientKey = clientKey};
    msgsnd(serverQ, &initMessage, MSG_BUFF_SIZE, 0);

    // czeka na wiadomosc zwrotna typu 1, czyli init
    struct msgbuf receivedMessage;
    msgrcv(clientQ, &receivedMessage, MSG_BUFF_SIZE, 0, 0);
    clientID = receivedMessage.clientID;
    printf("ID set to: %d\n", clientID);

    // tworzenie watkow
    pthread_t orderThread, messageThread;
    pthread_create(&orderThread, NULL, orderHandlerThread, NULL);
    pthread_create(&messageThread, NULL, messageHandlerThread, NULL);

    // oczekiwanie na zakonczenie watkow
    pthread_join(orderThread, NULL);
    pthread_join(messageThread, NULL);

    return 0;
}
