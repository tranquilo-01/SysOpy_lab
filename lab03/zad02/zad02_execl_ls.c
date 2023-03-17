#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments [%d], 1 argument (number of processes to create) required\n", argc - 1);
        return 1;
    }

    // setbuf(stdout, NULL);

    printf("Program name: %s", argv[0]);
    // ciekawe ze jak jest w princie wyzej newline to nie trzeba tego wolac i dziala
    fflush(stdout);

    execl("/bin/ls", "ls", argv[1], "-l", NULL);

    // jak execl zadziala to sie nie wyswietli
    perror("execl failed");
    exit(EXIT_FAILURE);
}