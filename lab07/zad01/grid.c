#include "grid.h"
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int grid_width = 30;
const int grid_height = 30;
const int thread_number = grid_height * grid_width;

// array for storing tids
pthread_t* threads = NULL;
int* threads_args = NULL;

char* create_grid() {
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char* grid) {
    free(grid);
}

void draw_grid(char* grid) {
    for (int i = 0; i < grid_height; ++i) {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j) {
            if (grid[i * grid_width + j]) {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            } else {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char* grid) {
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int row, int col, char* grid) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width) {
                continue;
            }
            if (grid[grid_width * r + c]) {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col]) {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    } else {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(char* src, char* dst) {
    for (int i = 0; i < grid_height; ++i) {
        for (int j = 0; j < grid_width; ++j) {
            dst[i * grid_width + j] = is_alive(i, j, src);
        }
    }
}

void update_grid_threads(char* src, char* dst) {
    for (int i = 0; i < thread_number; i++) {
        pthread_kill(threads[i], SIGUSR1);
    }
}

void handler(int signo, siginfo_t* info, void* context) {}

void* update_cell(void* arg) {
    UpdateCellArgs* args = (UpdateCellArgs*)arg;

    while (1) {
        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_sigaction = handler;
        sigaction(SIGUSR1, &action, NULL);

        int cell = args->cell;

        args->dst[cell] = is_alive(cell / grid_width, cell % grid_width, args->src);

        pause();

        char* tmp = args->src;
        args->src = args->dst;
        args->dst = tmp;
    }

    return NULL;
}

void init_threads(char* src, char* dst) {
    if (threads != NULL) {
        fprintf(stderr, "Error: threads have been initialized already\n");
        exit(-1);
    }

    threads = calloc(thread_number, sizeof(pthread_t));
    printf("calloc succesfull\n");

    for (int i = 0; i < thread_number; i++) {
        UpdateCellArgs* args = malloc(sizeof(UpdateCellArgs));
        printf("malloc succesfull %d\n", i);

        args->dst = dst;
        args->src = src;
        args->cell = i;
        printf("reading from args succesfull\n");

        pthread_create(&threads[i], NULL, update_cell, args);
    }
}