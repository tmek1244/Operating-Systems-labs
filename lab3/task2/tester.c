#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>


void createMatrix(char* fileName, int rows, int cols)
{
    FILE* file;
    file = fopen(fileName, "w");
    if(file == NULL)
    {
        printf("error during opening file");
        return;
    }

    for(int i = 0; i < rows; i++)
    {
        fprintf(file, "%i", (int)(rand() / (RAND_MAX + 1.0) * 100.0));
        for(int j = 1; j < cols; j++)
        {
            fprintf(file, " %i", (int)(rand() / (RAND_MAX + 1.0) * 100.0));
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main(int argc, char* argv[])
{
    int seed;
    time_t tt;
    seed = time(&tt);
    srand(seed);
    int min, max;
    min = atoi(argv[1]);
    max = atoi(argv[2]);
    int rowsA, commonSide, colsB;
    rowsA =  (int)(rand() / (RAND_MAX + 1.0) * (max - min) + min);
    commonSide =  (int)(rand() / (RAND_MAX + 1.0) * (max - min) + min);
    colsB =  (int)(rand() / (RAND_MAX + 1.0) * (max - min) + min);
    createMatrix("A.txt",  rowsA, commonSide);
    createMatrix("B.txt",  commonSide, colsB);
    return 0;
}