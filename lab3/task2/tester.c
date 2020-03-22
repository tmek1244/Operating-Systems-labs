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


int ** createMatrix(char* fileName, int rows, int cols)
{
    FILE* file;
    file = fopen(fileName, "w");
    if(file == NULL)
    {
        printf("error during opening file");
        return NULL;
    }
    int** matrix = (int **)calloc(rows, sizeof(int *));
    for (int i = 0; i < rows; i++)
        matrix[i] = (int *)calloc(cols, sizeof(int));

    int randomValue;
    for(int i = 0; i < rows; i++)
    {
        randomValue = (int)(rand() / (RAND_MAX + 1.0) * 100.0);
        matrix[i][0] = randomValue;
        fprintf(file, "%i", randomValue);

        for(int j = 1; j < cols; j++)
        {
            randomValue = (int)(rand() / (RAND_MAX + 1.0) * 100.0);
            matrix[i][j] = randomValue;
            fprintf(file, " %i", randomValue);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return matrix;
}


char* intToString(int number, int size)
{
    char* result = malloc(size * sizeof(char));
    for(int i = 0; i < size; i++)
        result[i] = ' ';
    char tmp[12];
    sprintf(tmp, "%d", number);
    for(int i = 0; i < strlen(tmp); i++)
        result[i] = tmp[i];
    return result;
}

void freeMatrix(int** result, int size)
{
    for(int i = 0; i < size; i++)
        free(result[i]);
    free(result);
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
    int** matrixA = NULL;
    int** matrixB = NULL;

    rowsA =  (int)(rand() / (RAND_MAX + 1.0) * (max - min) + min);
    commonSide =  (int)(rand() / (RAND_MAX + 1.0) * (max - min) + min);
    colsB =  (int)(rand() / (RAND_MAX + 1.0) * (max - min) + min);


    matrixA = createMatrix("A.txt", rowsA, commonSide);
    matrixB = createMatrix("B.txt", commonSide, colsB);

    int** result = (int **)calloc(rowsA, sizeof(int *));
    for (int i = 0; i < rowsA; i++)
        result[i] = (int *)calloc(colsB, sizeof(int));
    int sum = 0;
    for (int c = 0; c < rowsA; c++)
    {
        for (int d = 0; d < colsB; d++)
        {
            for (int k = 0; k < commonSide; k++) {
                sum += matrixA[c][k]*matrixB[k][d];
            }

            result[c][d] = sum;
            sum = 0;
        }
    }
    FILE* file = fopen("out.txt", "w");
    if(file == NULL)
    {
        printf("cannot open file");
        freeMatrix(matrixA, rowsA);
        freeMatrix(matrixB, colsB);
        freeMatrix(result, rowsA);

        return -1;
    }
    for(int i = 0; i < rowsA; i++)
    {
        for(int j = 0; j < colsB; j++)
        {
            fprintf(file, "%s ", intToString(result[i][j], 6));
        }
        fprintf(file, "\n");
    }

    freeMatrix(matrixA, rowsA);
    freeMatrix(matrixB, colsB);
    freeMatrix(result, rowsA);
    return 0;
}