#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

//*********************************************************
// Uzupelnij petle w taki sposob aby stworzyc dokladnie argv[1] procesow potomnych, bedacych dziecmi
//   tego samego procesu macierzystego.
// Kazdy proces potomny powinien:
// - "powiedziec ktorym jest dzieckiem",
//-  jaki ma pid,
//- kto jest jego rodzicem
//******************************************************

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Not a suitable number of program arguments");
        exit(2);
    } else {
        for (int i = 0; i < atoi(argv[1]); i++) {
            if (fork() == 0) {
                printf("Process number %d, pid %d, ppid %d\n", i, getpid(), getppid());
                exit(EXIT_SUCCESS);
            }
        }

        for (int i = 0; i < atoi(argv[1]); i++) {
            wait(NULL);
        }
    }
    return 0;
}
