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

LibWCData (*LibWCData_create)(size_t);
void (*LibWCData_init)(LibWCData*, size_t);
void (*LibWCData_clear)(LibWCData*);
void (*LibWCData_destroy)(LibWCData*);
char* (*LibWCData_get)(LibWCData*, size_t);
void (*LibWCData_delete)(LibWCData*, size_t);
void (*LibWCData_add)(LibWCData*, char* );

#endif