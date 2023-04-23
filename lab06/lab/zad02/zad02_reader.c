// zmodyfikuj poprzedni program tak aby czytacz i pisacz to byly 2 osobne programy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include "common.h"

int main(int argc, char** argv) {
    // id kolejki przekazywane jako parametr
    int mymsgq = atoi(argv[1]);
    printf("%d\n", mymsgq);

    // stworenie bufora do odebrania wiadomosci
    struct msgbuf buff1;

    // odebranie obu wiadomocsi i ich wypisanie
    for (int i = 0; i < 2; i++) {
        if (msgrcv(mymsgq, &buff1, sizeof(buff1.mtext), 0, IPC_NOWAIT) == -1) {
            perror("reader: msgrcv error");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", buff1.mtext);
    }

    // usuniecie kolejki
    if (msgctl(mymsgq, IPC_RMID, NULL) == -1) {
        perror("reader: msgctl error");
        exit(EXIT_FAILURE);
    }
}