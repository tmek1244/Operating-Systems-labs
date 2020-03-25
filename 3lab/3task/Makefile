CC = gcc -Wall

compile:
	$(CC) macierzLimits.c -o macierzLimits

all:
	make clean
	make compile

test:
	make all
	./macierzLimits ../lab3/task3/lista.txt 10 1 2 1 1

clean:
	rm -f *.o
	rm -f *.a
	rm -f macierzLimits
	rm -f a.out