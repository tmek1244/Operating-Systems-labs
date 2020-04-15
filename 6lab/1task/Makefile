CC=gcc

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
	$(CC) server.c -o server

client:
	$(CC) client.c -o client