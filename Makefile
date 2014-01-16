CC=clang
CFLAGS=-I.

outprogname: db.o dict.o server.o
	$(CC) -o bin/server dict.o db.o server.o

dict.o: dict.c
	$(CC) -c dict.c

db.o: db.c
	$(CC) -c db.c

server.o: server.c
	$(CC) -c server.c
