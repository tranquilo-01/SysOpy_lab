#ifndef __LIBWC_H__
#define __LIBWC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LIBWC_COMMAND_BUFF_SIZE 2048

typedef struct {
    char** arr;
    bool* active;
    size_t top;
    size_t size;
} LibWCData;

LibWCData LibWCData_create(size_t size);

void LibWCData_init(LibWCData* LibWCData, size_t size);

void LibWCData_clear(LibWCData* LibWCData);

void LibWCData_destruct(LibWCData* LibWCData);

char* LibWCData_get(LibWCData* LibWCData, size_t index);

void LibWCData_pop(LibWCData* LibWCData, size_t index);

void LibWCData_push(LibWCData* LibWCData, char* input_filename);

#endif