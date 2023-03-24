#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void handler(int sig) {
    printf("ja cie ale sygnal");
}

int main(){
    int pid;
    pid  = fork();

    if(pid){
        sleep(1);
        while(1){
            sleep(1);
            kill(pid, SIGUSR1);
        }
    }else{
        signal(SIGUSR1, handler);
        while(1){
            sleep(1);
        }
    }
}