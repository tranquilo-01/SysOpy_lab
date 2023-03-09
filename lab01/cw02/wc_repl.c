#ifdef USE_DLL
    #include "libwc.so.h"
#else
    #include "libwc.h"
#endif

#include "dllmanager.h"
#include <regex.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/times.h>

#define MAX_PATH_SIZE 2048
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_BOLD  "\x1b[1m"
#define ANSI_COLOR_RED "\x1b[31m"


typedef enum {
    EXIT,
    INVALID,
    INIT,
    COUNT,
    SHOW,
    DELETE,
    DESTROY,
} COMMAND;

LibWCData* data = NULL;

bool running = true;
int b;
COMMAND id;
int num_input;
char path[MAX_PATH_SIZE];

bool is_initialised = false;

regex_t rEXIT;
regex_t rINIT;
regex_t rCOUNT;
regex_t rSHOW;
regex_t rDELETE;
regex_t rDESTROY;

void regex_compile() {
    regcomp(&rEXIT, "exit", 0);
    regcomp(&rINIT, "init [0-9][0-9]*", 0);
    regcomp(&rCOUNT, "count ..*", 0);
    regcomp(&rSHOW, "show [0-9][0-9]*", 0);
    regcomp(&rDELETE, "delete [0-9][0-9]*", 0);
    regcomp(&rDESTROY, "destroy", 0);
}

size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

void parse_input(char* input, size_t input_len) {
    if (regexec(&rINIT, input, 0, NULL, 0) == 0) {
        id = INIT;
        sscanf(input, "init %d", &num_input);
    } else if (regexec(&rCOUNT, input, 0, NULL, 0) == 0) {
        id = COUNT;
        input += strlen("count ");;
        memset(path, '\0', sizeof(path));
        size_t newline_position = strcspn(input, "\n");
        strncpy(path, input, min(newline_position, MAX_PATH_SIZE));
    } else if (regexec(&rSHOW, input, 0, NULL, 0) == 0) {
        id = SHOW;
        sscanf(input, "show %d", &num_input);
    } else if (regexec(&rDELETE, input, 0, NULL, 0) == 0) {
        id = DELETE;
        sscanf(input, "delete %d", &num_input);
    } else if (regexec(&rDESTROY, input, 0, NULL, 0) == 0) {
        id = DESTROY;
    } else if (regexec(&rEXIT, input, 0, NULL, 0) == 0) {
        id = EXIT;
    } else {
        id = INVALID;
    }
    
    if ((id != INVALID) && (id != DELETE) && (id != COUNT)) {
        if (num_input < 0) {
            id = INVALID;
            fprintf(stderr, ANSI_COLOR_RED ANSI_COLOR_BOLD "ERROR: WCLIB: EXPECTED A POSITIVE ARGUMENT\n"ANSI_COLOR_RESET);
        } else if ((num_input < 1) && (id == INVALID)) {
            id = INVALID;
            fprintf(stderr, ANSI_COLOR_RED ANSI_COLOR_BOLD "ERROR: WCLIB: SIZE HAS TO BE GREATER THAN 0\n"ANSI_COLOR_RESET);
        }
    }
}

void execute_command() {
    if ((id > INIT) && (!is_initialised)) {
        fprintf(stderr, ANSI_COLOR_RED ANSI_COLOR_BOLD "ERROR: RUN 'init {size}' FIRST\n"ANSI_COLOR_RESET);
        return;
    }

    if ((id == INIT) && is_initialised) {
        fprintf(stderr, ANSI_COLOR_RED ANSI_COLOR_BOLD "ERROR: WCLIB already initialised\n"ANSI_COLOR_RESET);
        return;
    }

    switch (id) {
    case INIT:
        LibWCData_init(data, num_input);
        is_initialised = true;
        break;
    case COUNT:
        LibWCData_add(data, path);
        break;
    case SHOW:
        puts(LibWCData_get(data, num_input));
        break;
    case DELETE:
        LibWCData_delete(data, num_input);
        break;
    case DESTROY:
        LibWCData_destroy(data);
        is_initialised = false;
        break;
    case EXIT:
        running = false;
        break;
    case INVALID:
        fprintf(stderr, ANSI_COLOR_RED ANSI_COLOR_BOLD "ERROR: INVALID COMMAND, TRY INSTEAD:\n"ANSI_COLOR_RESET);
        fprintf(stderr, "init {size > 0}\n");
        fprintf(stderr, "count {path}\n");
        fprintf(stderr, "show {index >= 0}\n");
        fprintf(stderr, "delete {index >= 0}\n");
        fprintf(stderr, "destroy\n");
    }
}

bool is_whitespace(const char *s) {
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return false;
    s++;
  }
  return true;
}


int main(int argc, char** argv) {
    load_dll_symbols("libwc.so");
    regex_compile();

    data = malloc(sizeof(LibWCData));

    struct timespec timespec_buff_start, timespec_buff_end;
    struct tms tms_buff_start, tms_buff_end;

    while (running) {
        printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD "WCLIB: >>> " ANSI_COLOR_RESET);

        char* line = NULL;
        size_t line_len;

        b = getline(&line, &line_len, stdin);
        fflush(NULL);

        if (is_whitespace(line)) {
            free(line);
            continue;
        }

        parse_input(line, line_len);
        fflush(NULL);

        clock_gettime(CLOCK_REALTIME, &timespec_buff_start);
        times(&tms_buff_start);
        execute_command();
        clock_gettime(CLOCK_REALTIME, &timespec_buff_end);
        times(&tms_buff_end);
        fflush(NULL);

        puts("EXECUTION TIME:");
        printf("REAL: %ldns\n", timespec_buff_end.tv_nsec - timespec_buff_start.tv_nsec);

        printf("USER: %ldticks\n", tms_buff_end.tms_cutime - tms_buff_start.tms_cutime);
        printf("SYST: %ldticks\n", tms_buff_end.tms_cstime - tms_buff_start.tms_cstime);
        fflush(NULL);
        free(line);
    }

    return 0;
}