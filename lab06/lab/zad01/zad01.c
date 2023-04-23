// Napisz program w ktorym utworzysz kolejke komiunikatow
// wstaiwsz tam dwa komunikaty, "witaj swiecie" oraz "cala naprzod"
// nastepnie odbierz wszystkie komunikaty z tej kolejki
// zapisujac je do innego bufora i wypisz je na ekran

// wyciagnac strukture opisujaca kolejke i jakies parametry z niej

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

/* message buffer for msgsnd and msgrcv calls */
struct msgbuf {
    long mtype;     /* typ komunikatu   */
    char mtext[64]; /* tresc komunikatu */
};

int main() {
    // tworzenie unikalnego klucza do identyfikacji kolejki
    key_t key = ftok(".", 'A');

    if (key == -1) {
        perror("ftok error");
        exit(EXIT_FAILURE);
    }

    // utworzenie nowej kolejki komunikatow uzywajac wygenerowanego klucza
    int mymsgq = msgget(key, IPC_CREAT | 0666);

    if (mymsgq == -1) {
        perror("msgget error");
        exit(EXIT_FAILURE);
    }

    // utworzenie i wypelnienie buforow na wiadomosci do kolejki
    struct msgbuf msg1 = {1, "Hello world"};
    struct msgbuf msg2 = {2, "Forward"};

    // wyslanie pierwszej wiadomosci
    if (msgsnd(mymsgq, &msg1, sizeof(msg1.mtext), IPC_NOWAIT) == -1) {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }

    // wyslanie drugiej wiadomosci
    if (msgsnd(mymsgq, &msg2, sizeof(msg2.mtext), IPC_NOWAIT) == -1) {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }

    // stworenie bufora do odebrania wiadomosci
    struct msgbuf buff1;
    // printf("msgbuf: %ld, msg1: %ld, msg2, %ld\n", sizeof(buff1), sizeof(msg1), sizeof(msg2));

    // odebranie obu wiadomocsi i ich wypisanie
    for (int i = 0; i < 2; i++) {
        if (msgrcv(mymsgq, &buff1, sizeof(buff1.mtext), 0, IPC_NOWAIT) == -1) {
            perror("msgrcv error");
            exit(EXIT_FAILURE);
        }

        printf("%s\n", buff1.mtext);
    }

    // usuniecie kolejki
    if (msgctl(mymsgq, IPC_RMID, NULL) == -1) {
        perror("msgctl error");
        exit(EXIT_FAILURE);
    }

    return 0;
}