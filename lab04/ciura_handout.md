sygnał - informacja dla procesu, że coś się wydarzyło

- może zostać wysłany w dowolnym czasie:
  - przez jądro systemu
  - przez inny proces
- sygnały mają numery — liczby całkowite bez znaku
- nie trzeba na niego reagować
  - wyjątek: SIGKILL (sygnał unicestwienia procesu)
    funkcja obsługi sygnału przerywa działanie programu; po zakończeniu jej
    działania program wykonuje się dalej od miejsca, którym został przerwany

---

kill, signal, sigqueue, sigaction

```c
#include <signal.h>

int kill(int pid, int sig);
```

Funkcja kill przesyła do procesu o identyfikatorze pid sygnał sig.
Więcej: man 2 kill

```c
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
```

Funkcja signal określa, jak ma być obsługiwany dany sygnał.
signum - numer sygnału
handler - wskaźnik do funkcji obsługującej sygnał:

- SIG_DFL
- SIG_IGN
- funkcja zdefiniowana w programie
  Zwraca wskaźnik do poprzedniej funkcji obsługującej sygnał.

```c
union sigval {
  int   sival_int;
  void *sival_ptr;
};
```

```c
int sigqueue(int pid, int sig, const union sigval value);
```

Funkcja sigqueue przesyła do procesu o identyfikatorze pid
sygnał sig wraz z dodatkowymi danymi.

```c
int sigaction(int signum, const struct sigaction *act,
              struct sigaction *oldact);
```

Funkcja sigaction jest rozszerzeniem funkcji signal.
Więcej: man sigaction, materiały pomocnicze do laboratorium

---

blokowanie sygnałów (co innego niż SIG_IGN)

```c
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
int sigismember(const sigset_t *set, int signum);
```

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```

Funkcja sigprocmask zwraca lub ustawia maskę zablokowanych sygnałów

```c
int sigpending(sigset_t *set);
```

Funkcja sigpending zapisuje w argumencie set zbiór sygnałów
oczekujących na odblokowanie, czyli sygnałów otrzymanych, ale
zablokowanych

```c
int sigsuspend(const sigset_t *mask);
```

Funkcja sigsuspend tymczasowo ustawia maskę zablokowanych sygnałów na
mask i czeka na jakiś sygnał, a po obsłużeniu tego sygnału przywraca
poprzednią maskę zablokowanych sygnałów.

---

pozostałe funkcje

```c
int raise(int sig) { return kill(getpid(), sig); }
void pause();  /* Czeka na sygnał */
unsigned sleep(unsigned seconds);  /* Usypia proces */
/* Ustawia czas pobudki (wysłania sygnału SIGALRM) */
unsigned alarm(unsigned seconds);
```

---

man 7 signal

signal(SIGCHLD, SIG_IGN) - nie zamieniaj zakończonych procesów
potomnych na zombie

Proces potomny utworzony funkcją fork dziedziczy z procesu macierzystego
ustalenia dotyczące obsługi sygnałów

Nowy program uruchomiony funkcją exec\* ma te same ustalenia dotyczące
obsługi sygnałów, co stary program. Wyjątek: funkcje obsługi sygnałów
są zamieniane na SIG_DFL, bo wskaźnik do funkcji obsługi sygnału wskazuje
teraz na co innego.
