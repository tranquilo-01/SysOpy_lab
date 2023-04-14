CFLAGS = -Wall
CC = gcc

main: zad01.o

clean:
	rm -f zad01.o zad01

test:  zad01
	@echo -e "\n\nTESTING\n\n"
	./zad01 100 11
	./zad01 100 10
	./zad01 100 9

all: zad01 test
