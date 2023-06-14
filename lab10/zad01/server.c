#include "common.h" k

int main() {
    int fd = -1;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Error creating socket\n");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_zero[0] = '\0';
    if (bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) ==
        -1) {
        printf("Error binding\n");
    }

    if (listen(fd, 3) == -1) {
        printf("Error listening\n");
    }

    struct sockaddr_in cl_addr;
    int cl_fd = -1;
    socklen_t addr_size = sizeof(cl_addr);

    if ((cl_fd = accept(fd, (struct sockaddr*)&cl_addr, &addr_size)) ==
        -1) {
        printf("Error accepting\n");
    }

    char buf[64];
    if (read(cl_fd, buf, 64) == -1) {
        printf("Error receiving message\n");
    }
    printf("Message from client: \"%s\"\n", buf);
    shutdown(fd, SHUT_RDWR);
    close(fd);

    return 0;
}
