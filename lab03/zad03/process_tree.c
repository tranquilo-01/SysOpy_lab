#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments [%d], 2 arguments expected: <directory_ path>, <string>\n", argc - 1);
        exit(EXIT_FAILURE);
    }
}