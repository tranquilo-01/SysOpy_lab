// Napisz program w ktrym utworzysz kolejke komiunikatow
// wstaiwsz tam dwa komunikaty, "witaj swiecie" oraz "cala naprzod"
// nastepnie odbierz wszystkie komunikaty z tej kolejki
// zapisujac je do innego bufora i wypisz je na ekran

// wyciagnac strukture opisujaca kolejke i jakies parametry z niej

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

/* message buffer for msgsnd and msgrcv calls */
struct msgbuf
{
    long mtype;     /* typ komunikatu   */
    char mtext[64]; /* tresc komunikatu */
};

int main()
{
    key_t key = ftok(".", 'A');

    if (key == -1)
    {
        perror("ftok error");
        exit(EXIT_FAILURE);
    }

    int mymsgq = msgget(key, IPC_CREAT | 0666);

    if (mymsgq == -1)
    {
        perror("msgget error");
        exit(EXIT_FAILURE);
    }

    struct msgbuf msgnoo = {1, "Hello world"};
    struct msgbuf msgnot = {2, "Forward"};

    if (msgsnd(mymsgq, &msgnoo, sizeof(msgnoo), IPC_NOWAIT) == -1)
    {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }

    if (msgsnd(mymsgq, &msgnoo, sizeof(msgnot), IPC_NOWAIT) == -1)
    {
        perror("msgsnd error");
        exit(EXIT_FAILURE);
    }

    struct msgbuf buff1;

    if (msgrcv(mymsgq, &buff1, sizeof(buff1), 0, IPC_NOWAIT) == -1)
    {
        perror("msgrcv error");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", buff1.mtext);

    if(msgctl(mymsgq, IPC_RMID, NULL) == -1){
        perror("msgctl error");
        exit(EXIT_FAILURE);
    }

    return 0;
}