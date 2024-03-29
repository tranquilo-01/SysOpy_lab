// przygotuj wersje powyzszego przy zalozeniu domeny internetowe
// a takze zakladaja ze mozes miec wielu klientow i wiele przychadzachy polaczen

// domena internetowa, protokol datagramowy
//  Serwer:

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define PORT 7777

int main() {
    int fd = -1;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Error creating socket\n");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_zero[0] = '\0';
    if (bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1) {
        printf("Error binding\n");
    }

    char buf[64];
    if (read(fd, buf, 64) == -1) {
        printf("Error receiving message\n");
    }
    printf("Message from client: \"%s\"\n", buf);
    shutdown(fd, SHUT_RDWR);
    close(fd);

    return 0;
}
