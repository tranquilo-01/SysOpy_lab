#include "libwc.h"

char command[MAX_COMMAND_SIZE] = "";
int a;

// stworzenie nowej struktury
LibWCData LibWCData_create(size_t size) {
    return (LibWCData) {
        .arr = calloc(size, sizeof(char*)),
        .element_count = 0,
        .size = size,
    };
}

// inicjalizacja podanej struktury
void LibWCData_init(LibWCData* LibWCData, size_t size) {
    LibWCData->arr = calloc(size, sizeof(char*));
    LibWCData->element_count = 0;
    LibWCData->size = size;
}

// wyczyszczenie calej struktury
void LibWCData_clear(LibWCData* LibWCData) {
    for (size_t i = 0; i < LibWCData->element_count; i++)
        free(LibWCData->arr[i]);
    LibWCData->element_count = 0;
}

// destrukcja struktury
void LibWCData_destroy(LibWCData* LibWCData) {
    LibWCData_clear(LibWCData);
    free(LibWCData->arr);
}

// funkcja pomocnicza sprawdzajaca czy podany indeks jest w tablicy
bool LibWCData_is_in_range(LibWCData* LibWCData, size_t index) {
    if (LibWCData->element_count <= index) {
        fprintf(stderr, ANSI_COLOR_RED  ANSI_COLOR_BOLD "ERROR: INDEX OUT OF RANGE\n" ANSI_COLOR_RESET);
        return false; 
    }
    return true;
}

// funkcja zwracajaca dane spod podanego indeksu
char* LibWCData_get(LibWCData* LibWCData, size_t index) {
    if (LibWCData_is_in_range(LibWCData, index))
        return LibWCData->arr[index];
    return "";
}

// funkcja usuwajaca dane z podanego indeksu
void LibWCData_delete(LibWCData* LibWCData, size_t index) {
    if (LibWCData_is_in_range(LibWCData, index)) {
        int i = index+1;
        while (i<LibWCData->element_count)
        {
            LibWCData->arr[i-1]=LibWCData->arr[i];
            i++;
        }
        LibWCData->element_count--;       
    }
}


// funkcja przyjmuje sciezke do pliku i zwraca jego zawartosc
char* get_file_content(char* filename) {
    FILE* file = fopen(filename, "r");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* content = calloc(size, sizeof(char));
    a = fread(content, sizeof(char), size, file);
    fclose(file);
    return content;
}

//funkcja przyjmuje strukture LibWCData i sciezke do pliku
// wykonuje operacje wc na pliku i jej wynik dodaje do tablicy w strukturze
void LibWCData_add(LibWCData* LibWCData, char* input_filename) {
    // tworzenie pliku tmp
    char tmp_filename[] = "/tmp/wclib_XXXXXX";
    int tmp_file = mkstemp(tmp_filename);

    if (tmp_file == 0) {
        fprintf(stderr,ANSI_COLOR_RED  ANSI_COLOR_BOLD  "ERROR: FAILED TO CREATE A TEMPORARY FILE\n"ANSI_COLOR_RESET);
        return;
    }

    // wywolanie komendy wc na pliku i zapis wyniku do tmp
    snprintf(command, MAX_COMMAND_SIZE, "wc '%s' 1> '%s' 2>/dev/null", input_filename, tmp_filename);
    a = system(command);
    
    // ladowanie danych z tmp
    char* wc_output = get_file_content(tmp_filename);
    if (strlen(wc_output) == 0) {
        fprintf(stderr, ANSI_COLOR_RED  ANSI_COLOR_BOLD "ERROR: FAILED TO READ THE INPUT FILE (%s)\n"ANSI_COLOR_RESET, input_filename);
        return;
    }

    // usuwanie pliku tmp
    snprintf(command, MAX_COMMAND_SIZE, "rm -f '%s' 2>/dev/null", tmp_filename);
    a = system(command);

    // zapisanie wyniku dzialania komendy do tablicy w strukturze
    if (LibWCData->element_count < LibWCData->size) {
        LibWCData->arr[LibWCData->element_count] = wc_output;
        (LibWCData->element_count)++;
    } else {
        fprintf(stderr, ANSI_COLOR_RED  ANSI_COLOR_BOLD "ERROR: NOT ENOUGH MEMORY\n"ANSI_COLOR_RESET);
    }
}