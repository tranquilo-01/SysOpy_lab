// napisz program ktory tworzy proces potomny a  nastepnie w nieskinczinej petli
// wysyla do swojego potomka sygnaly SIGUSR1. Potomek powinien zliczac otrzymane
// od przodka sygnaly SIGUSR1 i wyswietlac na biezaco stan licznika

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void handle_signal(int sig) {
    printf("ja cie ale sygnal");
}

int main(){
    pid_t parentPid = getpid();
    printf("Parent: %d\n", parentPid);

    pid_t childPid = fork();
    if(childPid == -1){
        perror("Error while forking");
        exit(EXIT_FAILURE);
    }

    printf("Child: %d\n", childPid);


    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sigaction(SIGUSR1, &sa, NULL);

    // if(getpid() == childPid){
    //     printf("in child");
    //     while(1){
            
    //     }
    // }
    if(getpid() == parentPid){
        printf("in parent");
        while(1){
            // printf("powinno sie wyslac");
            kill(childPid, SIGUSR1);
            sleep(0.9);        
            }
    }
    

    // return 0;

}

// zmodyfikuj program w taki sposob ze w procesie potomnym 