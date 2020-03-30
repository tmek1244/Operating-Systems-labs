CC = gcc -Wall

compile:
	make clean
	$(CC) catcher.c -o catcher
	$(CC) sender.c -o sender

clean:
	rm -f *.a
	rm -f *.o
	rm -f catcher
	rm -f sender
	rm -f a.out