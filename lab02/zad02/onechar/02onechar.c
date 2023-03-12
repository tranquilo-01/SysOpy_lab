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

void reverse(char *inpath, char *outpath)
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
    char* buff = calloc(1, sizeof(char));

    // ustawiam sie na koniec pliku i cofajac zapisuje chary do out
    for(long i = fileSize-1; i >= 0; i-=1){
        fseek(inFile, i, SEEK_SET);
        fread(buff, sizeof(char), 1, inFile);
        fwrite(buff, sizeof(char), 1, outFile);
    }

    // zamkniecie plikow
    fclose(inFile);
    fclose(outFile);
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Wrong number of arguments [%d], 2 args expected\n", argc - 1);
        return -1;
    }

    struct timespec timespec_buff_start, timespec_buff_end;
    clock_gettime(CLOCK_REALTIME, &timespec_buff_start);

    char* inputPath = argv[1];
    char* outputPath = argv[2];

    reverse(inputPath, outputPath);
    clock_gettime(CLOCK_REALTIME, &timespec_buff_end);
    printf("EXECUTION TIME: %fs\n", (float) (timespec_buff_end.tv_nsec - timespec_buff_start.tv_nsec) / 1000000000.0);

}