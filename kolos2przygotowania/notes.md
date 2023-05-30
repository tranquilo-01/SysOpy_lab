# Notatki kolos 2

## 6. Kolejki komunikatow

### System V

- utworz klucz (standardowo jak w systemie v):

```c
key_t key = ftok(KEY, 'A');
```

- stworz nowa / otworz istniejaca kolejke:

```c
int q = msgget(key, IPC_CREAT | 0666);
```

- struktura na wiadomosc(do edycji przez programiste) i przypisanie wartosci:

```c
struct msgbuf {
    long mtype; /* typ komunikatu   */
    int mint;   /* tresc komunikatu */
};

struct msgbuf message = {1, atoi(argv[1])};
```

- wyslanie wiadomosci:

```c
msgsnd(q, &message, sizeof(message.mint), IPC_NOWAIT);
```

- odczytanie wiadomosci:

```c
msgrcv(q, &message, sizeof(message.mint), 0, IPC_NOWAIT);
```

- usuniecie kolejki:

```c
msgctl(q, IPC_RMID, NULL);
```

### POSIX

---

## 7. Shared memory, semafory

### System V

#### Semafory

- utworz klucz i semafor

```c
key_t sem_key = ftok(getenv("HOME"), 'A');

int sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
```

- ustaw semafor na 1

```c
semctl(sem_id, 0, SETVAL, 1)
```

- zabezpieczenie sekcji semaforem przez jego dekrementacje z 1 do 0

```c
struct sembuf sem_op;
sem_op.sem_num = 0;  // semaphore index
sem_op.sem_op = -1;  // decrement semaphore value by 1
sem_op.sem_flg = 0;  // no special flags
```

- koniec sekcji, ponowna inkrementacja semafora

```c
sem_op.sem_op = 1;
```

- usuniecie semafora

```c
semctl(sem_id, 0, IPC_RMID)
```

### POSIX

#### Semafory

- utworz semafor o wartosci 1

```c
sem_t* sem_id = sem_open(SEM_NAME, O_CREAT, 0666, 1);
```

- dekrementacja semafora:

```c
sem_wait(sem_id);
```

- inkrementacja semafora:

```c
sem_post(sem_id);
```

- zamkniecie i usuniecie semafora:

```c
sem_close(sem_id);
sem_unlink(SEM_NAME);
```

#### Shared memory

- Utworz nowy lub otworz segment pamieci wspoldzielonej:

```c
int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);
```

Pierwszy argument to sciezka identyfikujaca, drugi to flagi, a trzeci to uprawnienia

- Nastepnie trzeba okreslic rozmiar dla utworzonego segmentu:

```c
ftruncate(fd, MAX_SIZE);
```

- A potem podpiac do pamieci adresowej procesu:

```c
void* shm_ptr = mmap(NULL, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
```

- zapis do obszaru pamieci wspolnej:

```c
sprintf(shm_ptr, argv[1]);
```

- odpiecie pamieci wspolnej od pamieci adresowej procesu:

```c
munmap(shm_ptr, MAX_SIZE);
```

- oznaczenie pamieci wspolnej do usuniecia, co stanie sie po jej odpieciu od wszystkich procesow

```c
shm_unlink(SHM_NAME);
```

---

## 8. Watki - podstawy

- sygnatura funkcji watkowej:

```c
void* function(void* arg){}
```

- tworzenie nowego watku:

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
```

- thread - wskaźnik na miejsce gdzie zapisany zostanie identyfikator utworzonego wątku
- attr - dodatkowe ustawienia (opcjonalne, NULL = ustawienia domyślne)
- start_routine - adres funkcji, która ma zostać wykonana w utworzonym wątku
- arg - argument, z którym ma zostać wywołana

* czekanie na zakonczenie watku i odbieranie zwroconej wartosci:

```c
pthread_join(threadIDs[i], void** return_value)
```

- ubijanie watku:

```c
pthread_cancel(threadIDs[i]);
```

Po cancelowaniu domyslnie watek kontynuuje swoje dzialanie az dotrze do canncellation pointa, np `read(), write(), pause()`, czy w szczegolnosci `pthread_testcancel()`

Reakcje na zakonczenie mozna ustawic za pomoca:

```c
int pthread_setcancelstate(int state, int *oldstate);
```

gdzie jako pierwszy argument podajemy

- PTHREAD_CANCEL_ENABLE
- PTHREAD_CANCEL_DISABLE
- PTHREAD_CANCEL_DEFFERED - domyslne czeka na cancellation point
- PTHREAD_CANCEL_ASYNCHRONOUS - ubija od razu
