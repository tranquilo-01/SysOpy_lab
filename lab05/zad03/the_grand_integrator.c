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

#define WRITE_BUFF_SIZE 16
#define READ_BUFF_SIZE 1024
#define STREAM_PATH "./integrator_stream"

int main(int argc, char** argv) {
    printf("starting main\n");
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments. 2 arguments expected: rectangle width and number of processes.\n");
        exit(EXIT_FAILURE);
    }

    struct timeval start_time, end_time;
    double elapsed_time;
    gettimeofday(&start_time, NULL);

    int n_procs = atoi(argv[2]);
    double seg_width = 1.0 / n_procs;

    printf("running mkfifo\n");

    // stworzenie potoku w biezacym katalogu
    mkfifo(STREAM_PATH, 0666);
    printf("mkfifo done\n");

    for (int i = 0; i < n_procs; i++) {
        if (fork() == 0) {
            double curr_seg_start = i * seg_width;
            double curr_seg_end = curr_seg_start + seg_width;
            char start_buff[WRITE_BUFF_SIZE];
            char end_buff[WRITE_BUFF_SIZE];
            snprintf(start_buff, WRITE_BUFF_SIZE, "%f", curr_seg_start);
            snprintf(end_buff, WRITE_BUFF_SIZE, "%f", curr_seg_end);

            execl("./the_little_worker", "the_little_worker", argv[1], start_buff, end_buff, NULL);
        }
    }

    printf("processes created\n");

    char* p;
    char* end;
    char* delimiter = ",";
    char read_buffer[READ_BUFF_SIZE];

    double component = 0.0;
    double integration_result = 0.0;

    printf("opening stream\n");
    int stream = open(STREAM_PATH, O_RDONLY);
    printf("stream opened\n");
    int already_read = 0;
    while (read(stream, read_buffer, READ_BUFF_SIZE) > 0) {
        p = strtok(read_buffer, delimiter);

        while (p != NULL) {
            component = strtod(p, &end);

            if (end != p) {  // check if conversion succeeded
                integration_result += component;
                printf("read\n");
            }

            p = strtok(NULL, delimiter);
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
