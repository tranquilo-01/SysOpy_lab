#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

size_t getFileSize(FILE* file){
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    return size;
}

// czemu nie uzyc fgetc i fputc? -jest latwiej i chyba lepiej
void changeLetter(char oldacsii, char newascii, char *inpath, char *outpath)
{
    // tworzenie wskaznikow i otwieranie plikow
    FILE *inFile = fopen(inpath, "r");
    FILE *outFile = fopen(outpath, "w");


    // test czy sie otwarly
    if (inFile == NULL || outFile == NULL)
    {
        printf("File is not available \n");
        return;
    }

    // sprawdzenie rozmiaru pliku i zaalokowanie pamieci na bufor
    size_t fileSize = getFileSize(inFile);
    char* buff = calloc(fileSize, sizeof(char));

    // zaladowanie pliku do buforu
    fread(buff, sizeof(char), fileSize, inFile);

    // ustawienie chara, ktory bedziemy sprawdzac na pierwszy
    char* currentChar = buff;

    // przejscie po pliku i ewentualna zamiana charu w buforze
    while(*currentChar){
        if(*currentChar == oldacsii){
            *currentChar = newascii;
        }
        currentChar++;
    }
    
    // zapis do out
    fwrite(buff, sizeof(char), fileSize, outFile);

    // zamkniecie plikow
    fclose(inFile);
    fclose(outFile);
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("Wrong number of arguments [%d], 4 args expected", argc - 1);
        return -1;
    }

    struct timespec timespec_buff_start, timespec_buff_end;
    clock_gettime(CLOCK_REALTIME, &timespec_buff_start);

    char inLetter = argv[1][0];
    char outLetter = argv[2][0];
    char* inputName = argv[3];
    char* outputName = argv[4];

    changeLetter(inLetter, outLetter, inputName, outputName);

    clock_gettime(CLOCK_REALTIME, &timespec_buff_end);
    printf("EXECUTION TIME: %fs\n", (float) (timespec_buff_end.tv_nsec - timespec_buff_start.tv_nsec) / 1000000000.0);

}