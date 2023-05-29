#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define FILE_NAME "common.txt"
#define SEM_NAME "/kol_sem"

int main(int argc, char** args) {
    if (argc != 4) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    /**************************************************
    Stworz semafor systemu V
    Ustaw jego wartosc na 1
    ***************************************************/
    key_t sem_key = ftok(getenv("HOME"), 'A');
    if (sem_key == -1) {
        perror("ftok");
    }

    int sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
    }

    if (semctl(sem_id, 0, SETVAL, 1) == -1) {
        perror("semctl (setting value to 1)");
    };

    // ***************************************

    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int parentLoopCounter = atoi(args[1]);
    int childLoopCounter = atoi(args[2]);

    char buf[50];
    pid_t childPid;
    int max_sleep_time = atoi(args[3]);

    if ((childPid = fork())) {
        int status = 0;
        srand((unsigned)time(0));

        while (parentLoopCounter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);

            /*****************************************
            sekcja krytyczna zabezpiecz dostep semaforem
            **********************************************/
            struct sembuf sem_op;
            sem_op.sem_num = 0;  // semaphore index
            sem_op.sem_op = -1;  // decrement semaphore value by 1
            sem_op.sem_flg = 0;  // no special flags

            if (semop(sem_id, &sem_op, 1) == -1) {
                perror("semop");
                return 1;  // Added: Return error
            }

            sprintf(buf, "Wpis rodzica. Petla %d. Spalem %d\n", parentLoopCounter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

            /*********************************
            Koniec sekcji krytycznej
            **********************************/
            sem_op.sem_op = 1;  // increment semaphore value by 1
            if (semop(sem_id, &sem_op, 1) == -1) {
                perror("semop");
                return 1;  // Added: Return error
            }
        }
        waitpid(childPid, &status, 0);
    } else {
        srand((unsigned)time(0));
        while (childLoopCounter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);

            /*****************************************
            sekcja krytyczna zabezpiecz dostep semaforem
            **********************************************/
            struct sembuf sem_op;
            sem_op.sem_num = 0;  // semaphore index
            sem_op.sem_op = -1;  // decrement semaphore value by 1
            sem_op.sem_flg = 0;  // no special flags

            sprintf(buf, "Wpis dziecka. Petla %d. Spalem %d\n", childLoopCounter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

            /*********************************
            Koniec sekcji krytycznej
            **********************************/

            // Release the semaphore
            sem_op.sem_op = 1;  // increment semaphore value by 1
            if (semop(sem_id, &sem_op, 1) == -1) {
                perror("semop");
                _exit(1);  // Fixed: Use _exit to exit child process
            }
        }
        _exit(0);
    }

    /*****************************
    posprzataj semafor
    ******************************/
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl (deleting semaphore)");
        return 1;  // Added: Return error
    }

    close(fd);
    return 0;
}
