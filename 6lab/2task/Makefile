CC=gcc -Wall

clean:
	rm -f client
	rm -f server
	rm -f *.a
	rm -f *.o

compile:
	make clean
	make server
	make client

server:
	$(CC) server.c -o server -lrt

client:
	$(CC) client.c -o client -lrt