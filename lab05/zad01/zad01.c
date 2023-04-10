#include <stdio.h>

int main(int argc, char** argv) {
    if (argc != 2 || argc != 4) {
        fprintf(stderr, "Wrong number of arguments. Usage:\n1 argument: sender/data\n3 arguments: <e-mail> <title> <contents>");
    }
}