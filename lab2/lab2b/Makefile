CC = gcc -Wall

all:
	make clean
	$(CC) myFind.c -o myFind

clean:
	rm -f *.o
	rm -f *.a
	rm -f myFind

test:
	./myFind ./ -atime -1
