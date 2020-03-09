CC = gcc -Wall

static:
	make clean
	$(CC) -c library.c
	ar crs liblibrary.a library.o
	$(CC) testingProgram.c -o testingProgram -L. -llibrary

shared:
	make clean
	$(CC) -fPIC -c library.c
	$(CC) -shared -fPIC -o liblibrary.so library.o
	$(CC) testingProgram.c -o testingProgram -L. -llibrary -Wl,-rpath=`pwd`
clean:
	rm -f *.o
	rm -f *.a
	rm -f testingProgram
	rm -f *.so

small_test:
	./testingProgram 4 compare_pairs -t small1.txt small2.txt small1.txt small2.txt small1.txt small2.txt small1.txt small2.txt

medium_test:
	./testingProgram 7 compare_pairs -t medium1.txt medium2.txt medium1.txt medium2.txt medium1.txt medium2.txt\
		medium1.txt medium2.txt  medium1.txt medium2.txt medium1.txt medium2.txt medium1.txt medium2.txt

big_test:
	./testingProgram 100 compare_pairs -t big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt

save_small_test:
	./testingProgram 4 compare_pairs small1.txt small2.txt small1.txt small2.txt small1.txt small2.txt small1.txt small2.txt \
		save -t

save_medium_test:
	./testingProgram 7 compare_pairs medium1.txt medium2.txt medium1.txt medium2.txt medium1.txt medium2.txt\
		medium1.txt medium2.txt  medium1.txt medium2.txt medium1.txt medium2.txt medium1.txt medium2.txt save -t

save_big_test:
	./testingProgram 100 compare_pairs big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
 			big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt big1.txt big2.txt\
			save -t

delete_small_test:
	./testingProgram 4 compare_pairs small1.txt small2.txt save remove_block -t 0

delete_medium_test:
	./testingProgram 2 compare_pairs medium1.txt medium2.txt save remove_block -t 0

delete_big_test:
	./testingProgram 2 compare_pairs big1.txt big2.txt save remove_block -t 0

