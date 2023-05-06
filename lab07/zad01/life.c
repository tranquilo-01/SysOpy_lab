#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "grid.h"

int main() {
    srand(time(NULL));
    setlocale(LC_CTYPE, "");
    initscr();  // Start curses mode

    char* foreground = create_grid();
    char* background = create_grid();
    char* tmp;

    printf("Initializing grid\n");
    init_grid(foreground);
    printf("Grid initialized\n");
    printf("Initializing threads\n");
    init_threads(foreground, background);
    printf("Threads initialized\n");

    printf("Starting loop\n");
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
