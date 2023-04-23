#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include "common.h"

int main() {
    // uzyskanie dostepu do kolejki komunikatow po stronie serwera
    const key_t serverKey = ftok(HOME_PATH, PROJECT_ID);
    const int serverQ = msgget(serverKey, IPC_CREAT | 0666);

    // stworzenie kolejki po stronie klienta
    const key_t clientKey = ftok(HOME_PATH, PROJECT_ID);
    const int clientQ = msgget(clientKey, IPC_CREAT | 0666);

    // bufor na inicjalna wiadomosc
    struct msgbuf initMessage = {INIT, 0, 0, clientKey};
    msgsnd(serverQ, &initMessage, MSG_BUFF_SIZE, 0);
}