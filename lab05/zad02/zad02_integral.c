#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

double f(double x) {
    return 4.0 / (x * x + 1);
}

double integrate(double a, double b, double dx) {
    double result = 0.0;
    for (double x = a; x < b; x += dx)
        result += f(x) * dx;
    return result;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Wrong number of arguments. 2 arguments expected: rectangle width and number of processes.");
    }

    struct timeval start_time, end_time;
    double elapsed_time;
    gettimeofday(&start_time, NULL);

    double integration_result = 0.0;

    double rect_width = strtod(argv[1], NULL);
    int n_procs = atoi(argv[2]);
    double seg_width = 1.0 / n_procs;

    // tablica na "konce potokow"
    int fd[n_procs][2];
    // i wypelnienie jej potokami
    for (int i = 0; i < n_procs; i++) {
        if (pipe(fd[i]) == -1) {
            fprintf(stderr, "Error while creating a pipe");
            exit(EXIT_FAILURE);
        }
    }

    // tworzenie procesow dzieci
    for (int i = 0; i < n_procs; i++) {
        int pid = fork();
        if (pid == -1) {
            fprintf(stderr, "Error while forking");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // w procesie dziecka
            // zamykam koniec pipe do odczytu
            close(fd[i][0]);

            double seg_start = i * seg_width;
            double seg_end = seg_start + seg_width;

            double child_result = integrate(seg_start, seg_end, rect_width);

            // zapis do pipe i zakonczenie procesu
            write(fd[i][1], &child_result, sizeof(double));
            close(fd[i][1]);
            exit(EXIT_SUCCESS);
        }
    }

    // proces rodzica
    for (int i = 0; i < n_procs; i++) {
        close(fd[i][1]);
        double component = 0.0;
        read(fd[i][0], &component, sizeof(double));
        integration_result += component;
        close(fd[i][0]);
    }

    gettimeofday(&end_time, NULL);
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
    printf("Rectangle width: %.10f\nNumber of processes: %d\nIntegration result: %.10f\nElapsed time: %f s\n", rect_width, n_procs, integration_result, elapsed_time);
    return 0;
}