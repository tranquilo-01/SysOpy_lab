#include "grid.h"
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int grid_width = 30;
const int grid_height = 30;
const int cell_number = grid_height * grid_width;
int thread_number;
int* block_starts = NULL;
int* block_sizes = NULL;
pthread_t* threads = NULL;  // array for storing tids

// function calculates block sizes, so the are as close to each other as possible
void calculate_block_sizes(int thread_n) {
    if (thread_n > cell_number) {
        fprintf(stderr, "Thread number cannot be more than cell number\n");
        exit(-1);
    }

    thread_number = thread_n;
    block_sizes = calloc(thread_number, sizeof(int));
    block_starts = calloc(thread_number, sizeof(int));

    int min_block_size = cell_number / thread_number;
    int remainder = cell_number % thread_number;
    int sum = 0;

    for (int i = 0; i < thread_number; i++) {
        block_starts[i] = sum;
        if (i < remainder) {
            block_sizes[i] = min_block_size + 1;
        } else {
            block_sizes[i] = min_block_size;
        }
        sum += block_sizes[i];
    }
}

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

// function "waking up" all the threads using sugusr1
void update_grid_threads(char* src, char* dst) {
    for (int i = 0; i < thread_number; i++) {
        pthread_kill(threads[i], SIGUSR1);
    }
}

// handler ignoring the signal
// czasem jakby nie dzialalo nie ma pojecia dlaczego
void sigusr1_handler(int signo, siginfo_t* info, void* context) {}

// thread function updating cell
void* update_block(void* arg) {
    // reading arguments
    UpdateBlockArgs* args = (UpdateBlockArgs*)arg;

    while (1) {
        // number of the block in block_sizes array
        int block_number = args->block;
        int current_cell = block_starts[block_number];

        // goes from the begginnng to the end of the block and checks if the cell is still alive
        for (int i = 0; i < block_sizes[block_number]; i++) {
            args->dst[current_cell] = is_alive(current_cell / grid_width, current_cell % grid_width, args->src);
            current_cell++;
        }

        // pausing and waiting for the sigusr1 to "wake up" the thread
        pause();

        // switching the cells in the grid
        char* tmp = args->src;
        args->src = args->dst;
        args->dst = tmp;
    }

    return NULL;
}

// function initializing all threads before the simulation starts
void init_threads(char* src, char* dst) {
    if (threads != NULL) {
        fprintf(stderr, "Error: threads have been initialized already\n");
        exit(-1);
    }

    // setting a handler to ignore sigusr1
    // czasem jakby nie dzialalo
    struct sigaction action;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = sigusr1_handler;
    sigaction(SIGUSR1, &action, NULL);

    // initializing the array storing pids
    threads = calloc(thread_number, sizeof(pthread_t));

    // creating threads with their arguments
    for (int i = 0; i < thread_number; i++) {
        UpdateBlockArgs* args = malloc(sizeof(UpdateBlockArgs));

        args->dst = dst;
        args->src = src;
        args->block = i;

        pthread_create(&threads[i], NULL, update_block, args);
    }
}