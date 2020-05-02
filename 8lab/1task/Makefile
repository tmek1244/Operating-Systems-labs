CC = gcc -Wall -o
image = saturn.ascii.pgm

clean:
	rm -f main
	rm -f *.o
	rm -f cos*

compile:
	make clean
	$(CC) main main.c -lpthread -lm

sign1:
	make compile
	./main 1 sign $(image) cos1.txt

sign2:
	make compile
	./main 2 sign $(image) cos2.txt

sign4:
	make compile
	./main 4 sign $(image) cos3.txt

sign8:
	make compile
	./main 8 sign $(image) cos4.txt

block1:
	make compile
	./main 1 block $(image) cos5.txt

block2:
	make compile
	./main 2 block $(image) cos6.txt

block4:
	make compile
	./main 4 block $(image) cos7.txt

block8:
	make compile
	./main 8 block $(image) cos8.txt

interleaved1:
	make compile
	./main 1 interleaved $(image) cos9.txt

interleaved2:
	make compile
	./main 2 interleaved $(image) cos10.txt

interleaved4:
	make compile
	./main 4 interleaved $(image) cos11.txt

interleaved8:
	make compile
	./main 8 interleaved $(image) cos12.txt


