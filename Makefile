CC=clang
CFLAGS=-I.

outprogname: db.o dict.o server.o client.o util.o
	$(CC) -o bin/dictum-server dict.o db.o server.o util.o
	$(CC) -o bin/dictum-client client.o

dict.o: dict.c
	$(CC) -c dict.c

util.o: util.c
	$(CC) -c util.c

db.o: db.c
	$(CC) -c db.c

server.o: server.c
	$(CC) -c server.c

client.o: client.c
	$(CC) -c client.c