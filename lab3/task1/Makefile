CC = gcc -Wall

compile:
	$(CC) myLs.c -o myLs


all:
	make clean
	make compile

test:
	make all
	./myLs ../

clean:
	rm -f *.o
	rm -f *.a
	rm -f myLs
	rm -f a.out