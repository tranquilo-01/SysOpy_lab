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

#define BUFF_SIZE 64
#define STREAM_PATH "./integrator_stream"

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments. 2 arguments expected: rectangle width and number of processes.\n");
        exit(EXIT_FAILURE);
    }

    struct timeval start_time, end_time;
    double elapsed_time;
    gettimeofday(&start_time, NULL);

    double integration_result = 0.0;

    double rect_width = strtod(argv[1], NULL);
    int n_procs = atoi(argv[2]);
    double seg_width = 1.0 / n_procs;

    // stworzenie potoku w biezacym katalogu
    int stream = mkfifo(STREAM_PATH, 0666);

    for (int i; i < n_procs; i++) {
        if (fork() == 0) {
            double curr_seg_start = i * seg_width;
            double curr_seg_end = curr_seg_start + seg_width;
            char start_buff[BUFF_SIZE];
            char end_buff[BUFF_SIZE];
            snprintf(start_buff, BUFF_SIZE, "%f", curr_seg_start);
            snprintf(end_buff, BUFF_SIZE, "%f", curr_seg_end);

            execl("./the_little_worker", "the_little_worker", argv[1], start_buff, end_buff, NULL);
        }
    }

    remove(STREAM_PATH);

    return 0;
}
