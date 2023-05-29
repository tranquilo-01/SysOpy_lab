#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_NAME "/kol_shm"
#define MAX_SIZE 1024

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    int value = atoi(argv[1]);

    /***************************************
    Utworz/otworz posixowy obszar pamieci wspolnej "reprezentowany" przez SHM_NAME
    zapisz tam wartosc przekazana jako parametr wywolania programu
    posprzataj
    *****************************************/
    //   otwarcie/utworzenie obszaru
    const char* shm_path = SHM_NAME;
    int fd = shm_open(shm_path, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // ustawnienie rozmiaru
    if (ftruncate(fd, MAX_SIZE) == -1) {
        perror("ftruncate");
    }

    // podpiecie do pamieci adresowej procesu
    void* shm_ptr = mmap(NULL, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // zapis do obszaru
    memcpy(shm_ptr, &value, sizeof(int));

    // odpiecie pamieci wspolnej
    munmap(shm_ptr, MAX_SIZE);

    // oznaczenie obszaru do usuniecia - tutaj tego nie mozna zrobic
    // bo sender jak skonczy dzialac a receiver jeszcze nie zacznie to sie usunie shm
    // shm_unlink(SHM_NAME);

    return 0;
}
