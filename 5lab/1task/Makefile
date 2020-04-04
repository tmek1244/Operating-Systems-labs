CC = gcc -Wall

all:
	make compile
	@printf "\n--------------------------------\n"
	./main commands.txt

clean:
	rm -f *.a
	rm -f *.out
	rm -f *.o
	rm -f main

compile:
	make clean
	$(CC) main.c -o main