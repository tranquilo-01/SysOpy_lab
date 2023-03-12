#include <stdio.h>
#include <time.h>
#include <sys/times.h>

void changeLetter(char oldacsii, char newascii, char *inpath, char *outpath)
{
    // tworzenie wskaznikow i otwieranie plikow
    FILE *inFile = fopen(inpath, "r");
    FILE *outFile = fopen(outpath, "w");
    char ch;

    // test czy sie otwarly
    if (inFile == NULL || outFile == NULL)
    {
        printf("File is not available \n");
        return;
    }

    // czytanie po charze
    while ((ch = fgetc(inFile)) != EOF)
    {
        // zamiana
        if(ch == oldacsii){
            ch = newascii;
        }
        // zapis
        fputc(ch, outFile);
    }

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