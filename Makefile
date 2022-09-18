CC=gcc
CFLAGS=-c -Wall

all: transfer

transfer: transfer.o client_server.o
	$(CC) -pthread transfer.o client_server.o -o transfer

transfer.o: transfer.c
	$(CC) $(CFLAGS) transfer.c

client_server.o: client_server.c
	$(CC) $(CFLAGS) client_server.c

clean:
	rm -rf *.o transfer
