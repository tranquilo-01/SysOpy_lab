#include <stdio.h>

void changeLetter(char oldacsii, char newascii, char* inpath, char* outpath){
    FILE* infile = fopen(inpath, "r");
    FILE* outfile = fopen(outpath, "w");

    char buff;

    while ((buff = fgetc(infile)) != EOF)
        {
            printf("%c", buff);
        }

}



int main(int argc, char* argv[]){
    // if (argc != 5){
    //     printf("Wrong number of arguments [%d], 4 args expected", argc-1);
    // }

    changeLetter('a', 'b', argv[1], 'aa');
}