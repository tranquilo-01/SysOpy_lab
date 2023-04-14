CFLAGS = -Wall
CC = gcc

main: zad02.o

clean:
	rm -f zad02.o zad02

test:  zad02
	@echo -e "\n\nTESTING\n\n"
	./zad02 22
	./zad02 -9
	./zad02 0
	./zad02 cos

all: zad02 test
