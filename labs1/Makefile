CC = gcc -Wall

static:
	make clean
	$(CC) -c library.c
	ar crs liblibrary.a library.o
	$(CC) testingProgram.c -o testingProgram -L. -llibrary

shared:
	make clean
	$(CC) -fPIC -c library.c
	$(CC) -shared -soname liblibrary.so.1 -o liblibrary.so.1.0.1 library.o -lc
#    $(CC) -shared -fPIC -o liblibrary.so library.o
#    $(CC) testingProgram.c -o testingProgram -L. -llibrary

clean:
	rm -f *.o
	rm -f *.a
	rm -f testingProgram
