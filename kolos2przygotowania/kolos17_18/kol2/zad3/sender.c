#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY "."

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    /******************************************************
    Utworz kolejke komunikatow systemu V "reprezentowana" przez KEY
    Wyslij do niej wartosc przekazana jako parametr wywolania programu
    Obowiazuja funkcje systemu V
    ******************************************************/
    key_t key = ftok(KEY, 'A');
    if (key == -1) {
        perror("ftok");
        return -1;
    }

    int q = msgget(key, IPC_CREAT | 0666);
    if (q == -1) {
        perror("msgget");
        return -1;
    }

    /* message buffer for msgsnd and msgrcv calls */
    struct msgbuf {
        long mtype; /* typ komunikatu   */
        int mint;   /* tresc komunikatu */
    };

    struct msgbuf message = {1, atoi(argv[1])};

    if (msgsnd(q, &message, sizeof(message.mint), IPC_NOWAIT) == -1) {
        perror("msgsnd");
        return -1;
    };

    return 0;
}
