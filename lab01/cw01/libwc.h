#ifndef __LIBWC_H__
#define __LIBWC_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LIBWC_COMMAND_BUFF_SIZE 2048

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