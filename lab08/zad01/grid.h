#pragma once
#include <stdbool.h>
#include "grid.h"
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

char *create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void update_grid(char *src, char *dst);
void* update_cell(void* arg);
void handler(int signo, siginfo_t* info, void* context);
void init_threads();
void update_grid_threads(char* src, char* dst);

typedef struct {
    char* src;
    char* dst;
    int cell;
} UpdateCellArgs;