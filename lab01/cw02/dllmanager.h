#ifndef __DLLMANAGER_H__
#define __DLLMANAGER_H__

#ifdef USE_DLL
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#define WCLIB_SYMBOL_TABLE_SIZE 7
void* _dll_ptrs[WCLIB_SYMBOL_TABLE_SIZE];

char* _dll_symbol_names[] = {
    "LibWCData",
    "LibWCData_init",
    "LibWCData_clear",
    "LibWCData_destroy",
    "LibWCData_get",
    "LibWCData_delete",
    "LibWCData_add",
};

void load_dll_symbols(const char* filepath) {
    void *handle = dlopen(filepath, RTLD_LAZY);

    if (handle == NULL) {
        fprintf(stderr, "[DLL MANAGER] DLL NOT FOUND (%s)\n", filepath);
        return;
    }

    // rapidly dwindling sanity of C developers
    *(void **) (&LibWCData_create) = dlsym(handle,"LibWCData_create");
    *(void **) (&LibWCData_init) = dlsym(handle,"LibWCData_init");
    *(void **) (&LibWCData_clear) = dlsym(handle,"LibWCData_clear");
    *(void **) (&LibWCData_destroy) = dlsym(handle,"LibWCData_destroy");
    *(void **) (&LibWCData_get) = dlsym(handle,"LibWCData_get");
    *(void **) (&LibWCData_delete) = dlsym(handle,"LibWCData_delete");
    *(void **) (&LibWCData_add) = dlsym(handle,"LibWCData_add");
}

#else
void load_dll_symbols(const char* filepath) {}
#endif

#endif

