// napisz program w ktorym utworzysz watek ktory wypisze na kopnsole
// napis typu 'hello tu watek iteracja n' i sie zakonczy. program glowny powinien
// zaczekac na zakonczenie swojego watku, odebrac wartosc zworcona przez watek i
// wypisac po tym potwierdzenie typu watek zakonczony, konczymy, wartosc zwrocona przez watek to:

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ITERATION_NUMBER 10

void* thread_function(void* arg) {
    int sum = 0;
    for (int i = 0; i < ITERATION_NUMBER; i++) {
        sum += i;
        printf("Hello from the thread, iteration %d, sum %d\n", i, sum);
    }

    int* return_value = malloc(sizeof(int));
    *return_value = sum;
    pthread_exit(return_value);
}

int main() {
    pthread_t tid;

    pthread_create(&tid, NULL, thread_function, NULL);

    int* return_value;

    pthread_join(tid, (void**)&return_value);

    printf("Thread completed, value: %d\n", *return_value);

    return (0);
}
