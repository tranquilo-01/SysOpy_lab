// napisz program w ktrym wszystko co zapiszesz na stodout
// wyladuje w pliku out.txt
// dup2("out.txt", 1)

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    if(fork == 0){
        int fd = open(file, O_RDWR | O_CREAT, S_ISUSR | S_IWUSR);
        dup2(fd, 1); // make stdout go to file

        close(fd);

        // exec(...);

    }
}