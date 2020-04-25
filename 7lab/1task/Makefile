CC=gcc -Wall

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
	$(CC) packer.c -o packer

preparer:
	$(CC) preparer.c -o preparer

sender:
	$(CC) sender.c -o sender

main:
	$(CC) main.c -o main