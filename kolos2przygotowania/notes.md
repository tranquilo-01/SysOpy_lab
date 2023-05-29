# Notatki kolos 2

## 6. Kolejki komunikatow

### System V

### POSIX

---

## 7. Shared memory, semafory

### POSIX

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
