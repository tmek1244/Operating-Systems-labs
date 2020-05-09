CC=gcc -Wall

all: compile test clean

compile:
	$(CC) barber.c -o barber -lpthread -lm

test:
	./barber 3 8

clean:
	rm -f barber