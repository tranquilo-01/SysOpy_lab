//  #include <stdio.h>
//    main(){
//                printf("Poczatek\n");
//                fork();
//                printf("Koniec\n");
// }

// #include <stdio.h>
// #include <sys/types.h>
// #include <unistd.h>

// int main() {
//     pid_t child_pid;
//     printf("PID glownego programu: %d\n", (int)getpid());
//     child_pid = fork();
//     if(child_pid!=0) {
//         printf("Proces rodzica: Proces rodzica ma pid:%d\n", (int)getpid());
//         printf("Proces rodzica: Proces dziecka ma pid:%d\n", (int)child_pid);
//     } else {
//         printf("Proces dziecka: Proces rodzica ma pid:%d\n",(int)getppid());
//         printf("Proces dziecka: Proces dziecka ma pid:%d\n",(int)getpid());
//     }

//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Wrong number of arguments [%d], 1 argument (number of processes to create) required\n", argc - 1);
        return 1;
    }

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("Error: fork failed\n");
            return 1;
        } else if (pid == 0) {
            printf("[%d]: Pid procesu rodzica: %d, pid procesu dziecka: %d\n", i, (int)getppid(), (int)getpid());
            return 0;
        }
    }

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    printf("Pid procesu macierzystego: %d, argv[1]: %d\n", (int)getpid(), n);

    return 0;
}
