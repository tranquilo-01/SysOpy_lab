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

int main(int argc, char **argv){
    if(argc != 2){
        printf("Wrong number of arguments [%d], 1 argument (number of processes to create) required");
        return 1;
    }

    for(int i  = 0; i < argv[1]; i++){
        fork();
        printf("Proces dziecka: Proces rodzica ma pid:%d\n",(int)getppid());
        printf("Proces dziecka: Proces dziecka ma pid:%d\n",(int)getpid());
    }
    return 0;
}