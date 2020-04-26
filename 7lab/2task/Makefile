CC=gcc -Wall
flags=-lrt -pthread

clean:
	rm -f preparer
	rm -f packer
	rm -f sender
	rm -f main
	rm -f *.a
	rm -f *.o

compile:
	make clean
	make preparer
	make packer
	make sender
	make main

packer:
	$(CC) packer.c -o packer $(flags)

preparer:
	$(CC) preparer.c -o preparer $(flags)

sender:
	$(CC) sender.c -o sender $(flags)

main:
	$(CC) main.c -o main $(flags)