// napisz program w ktorym stworzysz proces potomny  a nastepnie
// przeslesz od rodzica do potomka komunikat typu hello world z
// wykorzystaniem potoku nienazwanego, w potomku odczytujemy
// przeslany komunikat i go drukujemy na stdout

// dorobic komunikacje zwrotna dziecko - rodzic
// po otrzymaniu komunikatu dziecko modyfikuje go i odsyla rodzicowi

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>


#define BUFFER_SIZE 50

int main()
{
    int fd[2];
    pipe(fd);

    int talk_do_daddy[2];
    pipe(talk_do_daddy);


    pid_t pid = fork();

    if (pid == 0)
    { // dziecko
        char buff[BUFFER_SIZE];

        close(talk_do_daddy[0]);
        close(fd[1]);
        read(fd[0], buff, BUFFER_SIZE); // - odczyt danych z potoku
        printf("%s\n", buff);
        strcat(buff, " daddy");
        write(talk_do_daddy[1], buff, strlen(buff)+1);
        
        
    }
    else
    { // rodzic
        char buff[BUFFER_SIZE];
        char message[] = "Hello world";
        close(fd[0]);
        close(talk_do_daddy[1]);
        write(fd[1], message, strlen(message)+1); // - zapis danych do potoku
        read(talk_do_daddy[0], buff, BUFFER_SIZE); // - odczyt danych z potoku
        printf("%s\n", buff);


    }
}