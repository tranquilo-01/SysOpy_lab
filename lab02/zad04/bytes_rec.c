#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ftw.h>

static long long total_size = 0;

int process_file(const char *fpath, const struct stat *sb, int typeflag) {
    if (!S_ISDIR(sb->st_mode)) {
        printf("%ld %s\n", sb->st_size, fpath);
        total_size += sb->st_size;
    }
    return 0;
}

void bytes_rec(char* dirPath){
    if (ftw(dirPath, process_file, 10) == -1) {
        printf("ftw error");
        return;
    }
    printf("Total size: %lld\n", total_size);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments [%d], [1] - directory path expected\n", argc - 1);
        return -1;
    }

    bytes_rec(argv[1]);
    return 0;

}