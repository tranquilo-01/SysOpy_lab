#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "grid.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments. 1 argument - number of threads expected");
        exit(-1);
    }

    int thread_number = atoi(argv[1]);

    calculate_block_sizes(thread_number);

    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr();  // Start curses mode

    char* foreground = create_grid();
    char* background = create_grid();
    char* tmp;

    init_grid(foreground);
    init_threads(foreground, background);

    while (true) {
        draw_grid(foreground);
        usleep(500 * 1000);

        // Step simulation
        update_grid_threads(foreground, background);
        tmp = foreground;
        foreground = background;
        background = tmp;
    }

    endwin();  // End curses mode
    destroy_grid(foreground);
    destroy_grid(background);

    return 0;
}
