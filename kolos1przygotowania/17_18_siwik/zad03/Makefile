CFLAGS = -Wall
CC = gcc

main: zad03.o

clean:
	rm -f zad03.o zad03

test:  zad03
	@echo -e "\nTESTING\n"
	./zad03 5 
	./zad03 7
	./zad03 100 | wc -l
	./zad03 200 | wc -l
all: zad03 test
