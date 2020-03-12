#!/bin/bash

option="sys"
file1=plik1.txt
file2=plik2.txt

make clean
gcc -Wall InputOutputTest.c -o main
./main generate "$file1" "$1" "$2"
if(( "option" == "sys" ))
then
  printf "\ncopy number of records: %s, size of one record: %s, with %s option\n" "$1" "$2" "$option">> wyniki.txt
  { time ./main copy "$file1" "$file2" "$1" "$2" "$option" ;} 2>> wyniki.txt
#  printf "%s" "$tmp">> wyniki.txt
fi