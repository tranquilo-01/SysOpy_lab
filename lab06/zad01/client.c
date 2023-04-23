#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include "common.h"

int clientID = 0;

int main() {
    // uzyskanie dostepu do kolejki komunikatow po stronie serwera
    const key_t serverKey = ftok(HOME_PATH, PROJECT_ID);
    const int serverQ = msgget(serverKey, 0);

    // stworzenie kolejki po stronie klienta
    srand(time(NULL));
    const key_t clientKey = ftok(HOME_PATH, rand() % 255 + 1);
    printf("%d\n", clientKey);
    const int clientQ = msgget(clientKey, IPC_CREAT | 0666);
    printf("clientQ: %d\n", clientQ);

    // bufor na inicjalna wiadomosc i wyslanie jej
    struct msgbuf initMessage = {.type = INIT, .text = 0, .clientID = -1, .clientKey = clientKey};
    msgsnd(serverQ, &initMessage, MSG_BUFF_SIZE, 0);

    struct msgbuf receivedMessage;

    while (1) {
        if (clientID == -1) {
            // jezeli nie ma przyznanego id to czeka na wiadomosc zwrotna typu 1, czyli init
            msgrcv(clientQ, &receivedMessage, MSG_BUFF_SIZE, 0, 0);
            clientID = receivedMessage.clientID;
            printf("ID set to: %d\n", clientID);
        }
    }
    return 0;
}