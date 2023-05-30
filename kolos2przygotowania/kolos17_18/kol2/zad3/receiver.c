#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define KEY "."

struct msgbuf {
    long mtype; /* typ komunikatu   */
    int mint;   /* tresc komunikatu */
};

int main() {
    sleep(1);
    int val = 0;

    /**********************************
    Otworz kolejke systemu V "reprezentowana" przez KEY
    **********************************/

    key_t key = ftok(KEY, 'A');
    if (key == -1) {
        perror("ftok");
        return -1;
    }

    int q = msgget(key, 0);
    if (q == -1) {
        perror("msgget");
        return -1;
    }

    while (1) {
        /**********************************
        Odczytaj zapisane w kolejce wartosci i przypisz je do zmiennej val
        obowiazuja funkcje systemu V
        ************************************/
        struct msgbuf msg;

        int rcv = msgrcv(q, &msg, sizeof(msg.mint), 0, IPC_NOWAIT);
        if (rcv == -1) {
            break;
        }
        val = msg.mint;

        printf("%d square is: %d\n", val, val * val);
    }

    /*******************************
    posprzataj
    ********************************/
    msgctl(q, IPC_RMID, NULL);

    return 0;
}
