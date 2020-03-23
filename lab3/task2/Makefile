CC = gcc -Wall

compile:
	$(CC) macierz.c -o macierze

all:
	make clean
	make compile

test1:
	make clean
	$(CC) tester.c -o tester
	./tester 30 50 1
	diff -w out.txt result.txt

test2:
	make clean
	$(CC) tester.c -o tester
	./tester 30 50 2
	diff -w out.txt result.txt


clean:
	rm -f *.o
	rm -f *.a
	rm -f macierze
	rm -f a.out
	rm -f tester