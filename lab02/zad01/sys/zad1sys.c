#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/times.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void changeLetter(char oldacsii, char newascii, char *inpath, char *outpath)
{
    // stworzenie deskryptorow i otwarcie plikow
    int in_fd, out_fd;
    in_fd = open(inpath, O_RDONLY);
    out_fd = open(outpath, O_WRONLY);

    char currentChar;

    // czytanie po charze, ewentualna zamiana i zapis do pliku
    while(read(in_fd, &currentChar, 1) == 1){
        if(currentChar == oldacsii){
            write(out_fd, &newascii, 1);
        }else{
            write(out_fd, &currentChar, 1);
        }
    }

    // zamkniecie chyba niepotrzebne ale ladnie wyglada
    close(in_fd);
    close(out_fd);
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