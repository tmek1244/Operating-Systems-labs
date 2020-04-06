CC = gcc -Wall

compile:
	make clean
	$(CC) producer.c -o producer
	$(CC) consumer.c -o consumer

clean:
	rm -f *.a
	rm -f *.out
	rm -f *.o
	rm -f producer
	rm -f consumer
	rm -f pipe
	rm -f main

producer:
	rm -f producer
	$(CC) producer.c -o producer
	./producer pipe data/2.txt 10

consumer:
	rm -f consumer
	$(CC) consumer.c -o consumer
	./consumer pipe out.txt 10

both:
	make clean
	make main

main:
	$(CC) consumer.c -o consumer
	$(CC) producer.c -o producer
	$(CC) main.c -o main
	./main