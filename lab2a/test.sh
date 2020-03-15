#!/bin/bash

operation="sort"
option="lib"
file1=plik1.txt
file2=plik2.txt

#make clean
gcc -Wall InputOutputTest.c -o main
#./main generate "$file1" "$1" "$2"
#if(( operation == "copy" ))
#then
#  printf "\ncopy number of records: %s, size of one record: %s, with %s option\n" "$1" "$2" "$option">> wyniki.txt
#  { time ./main $operation "$file1" "$file2" "$1" "$2" "$3" ;} 2>> wyniki.txt
##  printf "%s" "$tmp">> wyniki.txt
#fi

if(( operation == "sort" ))
then
#    ./main generate "$file1" "$1" "$2"
#    ./main copy "$file1" "$file2" "$1" "$2" sys
    printf "\nsort number of records: %s, size of one record: %s, with lib option\n" "$1" "$2" >> wyniki.txt
    { time ./main $operation "$file1" "$1" "$2" "lib" ;} 2>> wyniki.txt
    printf "\nsort number of records: %s, size of one record: %s, with sys option\n" "$1" "$2" >> wyniki.txt
    { time ./main $operation "$file2" "$1" "$2" "sys" ;} 2>> wyniki.txt
fi