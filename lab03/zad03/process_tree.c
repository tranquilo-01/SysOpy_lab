#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
            printf("%s %d\n", path, getpid());
        }
    }

    fclose(file);
}

void process_directory(char* path, char* phrase) {
    DIR* dir = opendir(path);
    struct dirent* ent;
    struct stat file_stat;

    if (dir == NULL) {
        perror("Error: cannot open directory");
        return;
    }

    while ((ent = readdir(dir)) != NULL) {
        // ladowanie danych o obiekcie do file_stat
        char child_path[PATH_MAX];
        snprintf(child_path, PATH_MAX, "%s/%s", path, ent->d_name);

        stat(child_path, &file_stat);

        // jezeli jest katalogiem
        if (S_ISDIR(file_stat.st_mode)) {
            // jezeli zaczyna sie od . lub .. to pomija
            if (strncmp(ent->d_name, ".", 1) == 0 || strncmp(ent->d_name, "..", 2) == 0) {
                continue;
            }

            pid_t pid = fork();
            if (pid == -1) {
                perror("Error while forking");
                continue;
            }
            if (pid == 0) {
                process_directory(child_path, phrase);
                exit(EXIT_SUCCESS);
            }
        } else if (S_ISREG(file_stat.st_mode)) {
            process_file(child_path, phrase);
        }
    }
    if (closedir(dir) == -1) {
        perror("Error while closing the directory");
    }
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

    process_directory(argv[1], argv[2]);
    usleep(10000);
    return 0;
}