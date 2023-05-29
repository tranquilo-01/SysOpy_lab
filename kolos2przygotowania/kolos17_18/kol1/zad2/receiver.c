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
    sleep(1);
    int val = 0;
    /*******************************************
    Utworz/otworz posixowy obszar pamieci wspolnej "reprezentowany" przez SHM_NAME
    odczytaj zapisana tam wartosc i przypisz ja do zmiennej val
    posprzataj
    *********************************************/
    //   otwarcie/utworzenie obszaru
    const char* shm_path = SHM_NAME;
    int fd = shm_open(shm_path, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    // podpiecie do pamieci adresowej procesu
    int* shm_ptr = mmap(NULL, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    printf("Proba odczytu\n");
    // odczyt wartosci
    memcpy(&val, shm_ptr, sizeof(int));

    // odpiecie pamieci wspolnej
    munmap(shm_ptr, MAX_SIZE);

    // oznaczenie obszaru do usuniecia
    shm_unlink(SHM_NAME);

    printf("%d square is: %d \n", val, val * val);
    return 0;
}
