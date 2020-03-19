#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <stdbool.h>


void readMatricesNames(char* fileName, char* matrixA, char* matrixB, char* outputMatrix)
{
    FILE* file;
    file = fopen(fileName, "r");

    if(file == NULL)
    {
        printf("Cannot open file");
        return;
    }

    fscanf(file, "%s %s %s", matrixA, matrixB, outputMatrix);
}


int main(int argc, char* argv[])
{
    if(argc != 3) {
        printf("Wrong number of arguments");
        return 0;
    }
    char* matrixA = malloc(50 * sizeof(char));
    char* matrixB = malloc(50 * sizeof(char));
    char* outputMatrix = malloc(50 * sizeof(char));
    int numberOfWorkerProcess = atoi(argv[2]);

    if(numberOfWorkerProcess <= 0)
    {
        printf("Number of worker process must be positive number");
        return 0;
    }

    readMatricesNames(argv[1], matrixA, matrixB, outputMatrix);



    return 0;
}