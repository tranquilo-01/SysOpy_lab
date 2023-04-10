#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
        // zamkniece koncowek do zapisu dla procesu rodzica
        close(fd[i][1]);
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
            printf("%f\n", child_result);

            // zapis do pipe i zakonczenie procesu
            write(fd[i][1], &child_result, sizeof(double));
            exit(EXIT_SUCCESS);
        }
    }

    // proces rodzica
    for (int i = 0; i < n_procs; i++) {
        double component = 0.0;
        read(fd[i][0], &component, sizeof(double));
        printf("%f\n", component);
        integration_result += component;
    }

    printf("Rectangle width: %f\nNumber of processes: %d\nIntegration result: %f\n", rect_width, n_procs, integration_result);
    return 0;
}