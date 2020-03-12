CC = gcc -Wall
option = sys
rSize = 1
rCount = 10
file1 = plik1.txt
file2 = plik2.txt

compile:
	$(CC) InputOutputTest.c -o main

clean:
	rm -f *.o
	rm -f *.a
	rm -f main
	rm -f $(file1), $(file2)

testCopy:
	make clean
	make compile
	./main generate $(file1) $(rCount) $(rSize)
	@eval printf "\ncopy number of records : $(rCount), size of one record : $(rSize), with $(option) \n" > wyniki.txt
	@eval time ./main copy $(file1) $(file2) $(rCount) $(rSize) $(option) > wyniki.txt


