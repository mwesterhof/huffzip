.PHONY = clean

CC = gcc
CFLAGS = -g

all: huffzip
	./huffzip

debug: huffzip
	cgdb huffzip

valgrind: huffzip
	valgrind ./huffzip

huffzip: main.o huffzip.o
	$(CC) -o huffzip main.o huffzip.o

main.o: main.c huffzip.h
	$(CC) -c $(CFLAGS) main.c

huffzip.o: huffzip.c huffzip.h
	$(CC) -c $(CFLAGS) huffzip.c

clean:
	-rm huffzip *.o
