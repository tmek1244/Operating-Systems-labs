CC = gcc -Wall

all:
	@make prepare
	./main ignore
	./main mask
	./main pending
	./main handler

clean:
	@rm -f main
	@rm -f operationsWithSignals
	@rm -f *.out
	@rm -f *.o
	@rm -f *.a

prepare:
	@make clean
	@$(CC) operationsWithSignals.c -o operationsWithSignals
	@$(CC) main.c -o main