#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PHRASE_LENGTH 255

void process_file(char* path, char* phrase) {
    FILE* file = fopen(path, "r");

    // sprawdzenie czy da sie otworzyc
    if (file == NULL) {
        perror("Error: cannot open file");
        return;
    }

    // alokacja bufora
    char buffer[MAX_PHRASE_LENGTH];

    // jesli da sie otworzyc i zaczyna sie od szukanej frazy to wypisuje dane
    if (fread(buffer, sizeof(char), strlen(phrase), file) != 0) {
        if (strncmp(buffer, phrase, strlen(phrase)) == 0) {
            printf("%s, %d\n", path, getpid());
        }
    }

    close(file);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments [%d], 2 arguments expected: <directory_ path>, <string>\n", argc - 1);
        exit(EXIT_FAILURE);
    }
    if (strlen(argv[2]) > MAX_PHRASE_LENGTH) {
        fprintf(stderr, "Phrase too long. Max phrase length: %d", MAX_PHRASE_LENGTH);
        exit(EXIT_FAILURE);
    }
    if (strlen(argv[1]) > PATH_MAX) {
        fprintf(stderr, "Path too long. Max path length: %d", PATH_MAX);
        exit(EXIT_FAILURE);
    }
}