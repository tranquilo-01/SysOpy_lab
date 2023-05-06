#pragma once
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "grid.h"

char* create_grid();
void destroy_grid(char* grid);
void draw_grid(char* grid);
void init_grid(char* grid);
bool is_alive(int row, int col, char* grid);
void update_grid(char* src, char* dst);
void* update_block(void* arg);
void handler(int signo, siginfo_t* info, void* context);
void init_threads();
void update_grid_threads(char* src, char* dst);
void calculate_block_sizes(int thread_number);

typedef struct {
    char* src;
    char* dst;
    int block;
} UpdateBlockArgs;