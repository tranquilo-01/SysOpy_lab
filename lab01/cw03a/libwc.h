#ifndef __LIBWC_H__
#define __LIBWC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMAND_SIZE 2048
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_BOLD  "\x1b[1m"


typedef struct {
    char** arr;
    size_t element_count;
    size_t size;
} LibWCData;

LibWCData LibWCData_create(size_t size);

void LibWCData_init(LibWCData* LibWCData, size_t size);

void LibWCData_clear(LibWCData* LibWCData);

void LibWCData_destroy(LibWCData* LibWCData);

char* LibWCData_get(LibWCData* LibWCData, size_t index);

void LibWCData_delete(LibWCData* LibWCData, size_t index);

void LibWCData_add(LibWCData* LibWCData, char* input_filename);

#endif