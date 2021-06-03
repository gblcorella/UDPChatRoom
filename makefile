CC = gcc

all: client server

client: client.o 

server: server.o 

client.o: client.c
	gcc -c client.c

server.o: server.c
	gcc -c server.c

clean:
	rm -f client.o server.o client server


