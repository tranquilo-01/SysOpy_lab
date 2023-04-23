// zmodyfikuj poprzedni program tak aby czytacz i pisacz to byly 2 osobne programy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include "common.h"

int main() {
    // tworzenie unikalnego klucza do identyfikacji kolejki
    key_t key = ftok(".", 'A');

    if (key == -1) {
        perror("writer: ftok error");
        exit(EXIT_FAILURE);
    }

    // utworzenie nowej kolejki komunikatow uzywajac wygenerowanego klucza
    int mymsgq = msgget(key, IPC_CREAT | 0666);
    printf("%d\n", mymsgq);

    if (mymsgq == -1) {
        perror("writer: msgget error");
        exit(EXIT_FAILURE);
    }

    // utworzenie i wypelnienie buforow na wiadomosci do kolejki
    struct msgbuf msg1 = {1, "Hello world"};
    struct msgbuf msg2 = {2, "Forward"};

    // wyslanie pierwszej wiadomosci
    if (msgsnd(mymsgq, &msg1, sizeof(msg1.mtext), IPC_NOWAIT) == -1) {
        perror("writer: msgsnd error");
        exit(EXIT_FAILURE);
    }

    // wyslanie drugiej wiadomosci
    if (msgsnd(mymsgq, &msg2, sizeof(msg2.mtext), IPC_NOWAIT) == -1) {
        perror("writer: msgsnd error");
        exit(EXIT_FAILURE);
    }
    return 0;
}
