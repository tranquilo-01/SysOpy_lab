#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

void wcc(){
    DIR *dir;
    struct dirent *ent;
    struct stat file_stat;
    long long total_size = 0;

    // otwarcie katalogu
    dir = opendir(".");
    if(dir == NULL){
        perror("Unable to open the catalog");
        return;
    }

    // odczyt plikow po kolei
    while((ent = readdir(dir)) != NULL){
        stat(ent->d_name, &file_stat);
        if (!S_ISDIR(file_stat.st_mode)) {
            // wypisanie rozmiaru i nazwy
            printf("%lld %s\n", (long long) file_stat.st_size, ent->d_name);
            total_size += file_stat.st_size;
        }
    }
    printf("%lld total\n", total_size);
}

int main() {
    wcc();
    return 0;
}
