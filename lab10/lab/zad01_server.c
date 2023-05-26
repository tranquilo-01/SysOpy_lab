// napisz program serwera w ktorym stworzysz socket domeny uniksowej
// typu datagramowego
// https://docs.google.com/document/d/1JNC6_znM5S_xl-Q1TONtJoHG8sjPxsROYRj9b2hRVBY/edit

#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define UNIX_PATH_MAX 108

int main(){
    int fd = -1;
    if(fd = socket(AF_UNIX, SOCK_DGRAM, 0) == -1){
        perror("error creating socket");
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';

    if(bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1){
        perror("Error binding");
    }

    char buf[20];

    if(read(fd, buf, 20) == -1){
        perror("Error receiving message");
    }

    printf("Message from client \"%s\"\n", buf);

    shutdown(fd, SHUT_RDWR);
    close(fd);
    return 0;
}