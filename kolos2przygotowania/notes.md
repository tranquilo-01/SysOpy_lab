# Notatki kolos 2

## 6. Kolejki komunikatow

### System V

### POSIX

## 7. NFEOFWO

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
