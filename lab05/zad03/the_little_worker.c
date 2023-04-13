#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 64
#define STREAM_PATH "./integrator_stream"

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
    double dx = strtod(argv[1], NULL);
    double a = strtod(argv[2], NULL);
    double b = strtod(argv[3], NULL);

    printf("worker running\n");

    double result = integrate(a, b, dx);

    char result_str[BUFF_SIZE];

    snprintf(result_str, BUFF_SIZE, "%f\n", result);

    int stream = open(STREAM_PATH, O_WRONLY);

    write(stream, snprintf, BUFF_SIZE);

    close(stream);
}
