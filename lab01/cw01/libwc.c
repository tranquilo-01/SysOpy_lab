#include "libwc.h"

char command[LIBWC_COMMAND_BUFF_SIZE] = "";

// stworzenie nowej struktury
LibWCData LibWCData_create(size_t size) {
    return (LibWCData) {
        .arr = calloc(size, sizeof(char*)),
        .top = 0,
        .size = size,
    };
}

// inicjalizacja podanej struktury
void LibWCData_init(LibWCData* LibWCData, size_t size) {
    LibWCData->arr = calloc(size, sizeof(char*));
    LibWCData->top = 0;
    LibWCData->size = size;
}

// wyczyszczenie calej struktury
void LibWCData_clear(LibWCData* LibWCData) {
    for (size_t i = 0; i < LibWCData->top; i++)
        free(LibWCData->arr[i]);
    LibWCData->top = 0;
}

// destrukcja struktury
void LibWCData_destruct(LibWCData* LibWCData) {
    LibWCData_clear(LibWCData);
    free(LibWCData->arr);
}

// funkcja pomocnicza sprawdzajaca czy podany indeks jest w tablicy
bool LibWCData_range_check(LibWCData* LibWCData, size_t index) {
    if (LibWCData->top <= index) {
        fprintf(stderr, "[LIB WC] INDEX OUT OF RANGE\n");
        return false; 
    }
    return true;
}

// funkcja zwracajaca dane spod podanego indeksu
char* LibWCData_get(LibWCData* LibWCData, size_t index) {
    if (LibWCData_range_check(LibWCData, index))
        return LibWCData->arr[index];
    return "";
}

// funkcja usuwajaca dane z podanego indeksu
void LibWCData_pop(LibWCData* LibWCData, size_t index) {
    if (LibWCData_range_check(LibWCData, index)) {
        free(LibWCData->arr[index]);
        int i = index+1;
        while (i<LibWCData->top)
        {
            LibWCData->arr[i-1]=LibWCData->arr[i];
            i++;
        }
        free(LibWCData->arr[i-1]);
        LibWCData->top--;       
    }
}

// get file size
long get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

// funkcja laduje plik do buforu (char*)
char* get_file_content(char* filename) {
    FILE* file = fopen(filename, "r");
    size_t size = get_file_size(file);
    char* buff = calloc(size, sizeof(char));
    fread(buff, sizeof(char), size, file);
    fclose(file);
    return buff;
}

// mieso biblioteki
// bierze plik, robi wc, zapisuje do tmp, czyta z tmp i wrzuca do struktury
void LibWCData_push(LibWCData* LibWCData, char* input_filename) {
    char tmp_filename[] = "/tmp/wclib_XXXXXX";
    int tmp_file = mkstemp(tmp_filename);

    if (tmp_file == 0) {
        fprintf(stderr, "[LIB WC] FAILED TO CREATE A TEMPORARY FILE\n");
        return;
    }

    snprintf(command, LIBWC_COMMAND_BUFF_SIZE, "wc '%s' 1> '%s' 2>/dev/null", input_filename, tmp_filename);
    system(command);
    
    char* wc_output = get_file_content(tmp_filename);
    if (strlen(wc_output) == 0) {
        fprintf(stderr, "[LIB WC] FAILED TO READ THE INPUT FILE (%s)\n", input_filename);
        return;
    }

    snprintf(command, LIBWC_COMMAND_BUFF_SIZE, "rm -f '%s' 2>/dev/null", tmp_filename);
    system(command);

    if (LibWCData->top < LibWCData->size) {
        LibWCData->arr[LibWCData->top] = wc_output;
        (LibWCData->top)++;
    } else {
        fprintf(stderr, "[LIB WC] NOT ENOUGH MEMORY\n");
    }
}