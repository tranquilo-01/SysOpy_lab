#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <time.h>
#include "common.h"

// tablica pod id klienta przechowuje identyfikator jego kolejki
int clients[MAX_CLIENT_NUMBER];
int serverQ;
char timeBuff[TIME_BUFF_SIZE];
int activeClients = 0;

void logToFile(msgbuf* message) {
    getTime(timeBuff);

    FILE* log = fopen("log.txt", "a");
    fprintf(log, "TIME: %s\n", timeBuff);

    switch (message->type) {
        case INIT:
            fprintf(log, "INIT\nClient ID: %d\n", message->clientID);
            break;
        case LIST:
            fprintf(log, "LIST\nClient ID: %d\n", message->clientID);
            break;
        case TALL:
            fprintf(log, "2ALL\nClient ID: %d\n", message->clientID);
            fprintf(log, "Message: %s\n", message->text);
            break;
        case TONE:
            fprintf(log, "2ONE\nSender ID: %d, Recipient ID %d\n", message->clientID, message->recipientID);
            fprintf(log, "Message: %s\n", message->text);
            break;
        case STOP:
            fprintf(log, "STOP\nClient ID: %d\n", message->clientID);
            break;
    }
    fprintf(log, "\n");

    fclose(log);
}

void handleInit(msgbuf* buffer) {
    printf("Handling init: %d\n", buffer->clientKey);
    logToFile(buffer);

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
    activeClients++;

    // wyslanie potwierdzenia o przyznaniu id do klienta
    struct msgbuf message = {.type = INIT, .text = 0, .clientID = idx, .clientKey = -1};
    msgsnd(newClientQ, &message, MSG_BUFF_SIZE, 0);
}

void handleStop(msgbuf* buffer) {
    logToFile(buffer);

    // usuniecie klienta z kolejki
    clients[buffer->clientID] = -1;
    activeClients--;
}

void handleList(msgbuf* buffer) {
    logToFile(buffer);

    printf("List of clients:\n");
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
        if (clients[i] != -1) {
            printf("ID: %d, Q: %d\n", i, clients[i]);
        }
    }
}

void handleTall(msgbuf* buffer) {
    logToFile(buffer);

    struct msgbuf message = {.type = TALL, .clientID = buffer->clientID};
    strcpy(message.time, timeBuff);
    strcpy(message.text, buffer->text);
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
        if (clients[i] != -1 && i != buffer->clientID) {
            if (msgsnd(clients[i], &message, MSG_BUFF_SIZE, 0) == -1) {
                perror("msgsnd");
            }
        }
    }
}

void handleTone(msgbuf* buffer) {
    logToFile(buffer);

    if (clients[buffer->recipientID] == -1) {
        printf("Client wint given ID does not exist\n");
        return;
    }

    struct msgbuf message = {.type = TONE, .clientID = buffer->clientID, .recipientID = buffer->recipientID};
    strcpy(message.time, timeBuff);
    strcpy(message.text, buffer->text);
    if (msgsnd(clients[buffer->recipientID], &message, MSG_BUFF_SIZE, 0) == -1) {
        perror("msgsnd");
    }
}

void shutdown() {
    // kazdemu klientowi z tablicy wysylam wiadomosc stop i usuwam go z tablicy
    printf("\nSERVER SHUTTING DOWN\n");
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
        if (clients[i] != -1) {
            struct msgbuf shutdownMessage = {.type = STOP, .clientID = i};
            msgsnd(clients[i], &shutdownMessage, MSG_BUFF_SIZE, 0);
            clients[i] = -1;
        }
    }

    // usuniecie kolejki serwera
    msgctl(serverQ, IPC_RMID, 0);
    exit(0);
}

int main() {
    // funkcja zdefiniowana w headerze
    getTime(timeBuff);
    printf("%s\n", timeBuff);

    // inicjalizacja tablicy z identyfikatorami kolejek klientow
    for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
        clients[i] = -1;
    }

    // ustawienie handlera dla sygnalu ubicia
    signal(SIGINT, shutdown);

    // klucz serwera na podstawie sciezki katalogu domowego i stalej serverID
    const key_t serverKey = ftok(HOME_PATH, PROJECT_ID);

    // stworzenie kolejki komunikatow po stronie serwera
    serverQ = msgget(serverKey, IPC_CREAT | 0666);

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