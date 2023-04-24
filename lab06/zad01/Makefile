client:
	gcc -o client client.c

server:
	gcc -o server server.c

all:
	make client
	make server

clean:
	rm -f client server log.txt

recompile:
	make clean
	make all