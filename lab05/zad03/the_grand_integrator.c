#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define WRITE_BUFF_SIZE 32
#define READ_BUFF_SIZE 1024
#define STREAM_PATH "./integrator_stream"

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments. 2 arguments expected: rectangle width and number of processes.\n");
        exit(EXIT_FAILURE);
    }

    // pomiar czasu
    struct timeval start_time, end_time;
    double elapsed_time;
    gettimeofday(&start_time, NULL);

    int n_procs = atoi(argv[2]);
    double seg_width = 1.0 / n_procs;

    // stworzenie potoku w biezacym katalogu
    mkfifo(STREAM_PATH, 0666);

    // uruchamianie programu obliczajacego odpowiednia liczbe razy
    for (int i = 0; i < n_procs; i++) {
        if (fork() == 0) {
            // ustalenie przedzialu i zamiana na string do parametru
            double curr_seg_start = i * seg_width;
            double curr_seg_end = curr_seg_start + seg_width;
            char start_str[WRITE_BUFF_SIZE];
            char end_str[WRITE_BUFF_SIZE];
            snprintf(start_str, WRITE_BUFF_SIZE, "%f", curr_seg_start);
            snprintf(end_str, WRITE_BUFF_SIZE, "%f", curr_seg_end);

            // uruchomienie programu obliczjacego calke
            execl("./the_little_worker", "the_little_worker", argv[1], start_str, end_str, NULL);
        }
    }

    // przygotowanie buforu do zapisu
    char read_buffer[READ_BUFF_SIZE];

    double integration_result = 0.0;

    // otwarcie potoku
    int stream = open(STREAM_PATH, O_RDONLY);
    int summed_component_number = 0;

    // dopoki nie zsumujemy tylu skladnikow ile programow zostalo uruchomionych
    while (summed_component_number < n_procs) {
        // rozmiar przeczytanego kawalka
        size_t read_size = read(stream, read_buffer, READ_BUFF_SIZE);
        // terminowanie buforu na koncu
        read_buffer[read_size] = 0;
        // pobranie pierwszego tokenu i kolejnych dopoki sa i sumowanie
        char* token = strtok(read_buffer, ",");
        while (token) {
            integration_result += strtod(token, NULL);
            summed_component_number++;
            token = strtok(NULL, ",");
        }
    }

    close(stream);

    remove(STREAM_PATH);

    gettimeofday(&end_time, NULL);
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    fflush(NULL);

    printf("Rectangle width: %s\nNumber of processes: %d\nIntegration result: %.10f\nElapsed time: %f s\n", argv[1], n_procs, integration_result, elapsed_time);

    return 0;
}
