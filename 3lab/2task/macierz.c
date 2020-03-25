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

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

char* option;
int sizeOfNumber = 9;


char* intToString(int number, int size)
{
    char* result = malloc(size * sizeof(char));
    for(int i = 0; i < size; i++)
        result[i] = ' ';
    char tmp[10] = "";
    sprintf(tmp, "%d", number);
    for(int i = 0; i < size; i++)
        result[i] = tmp[i];
    return result;
}


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
    fclose(file);
}

int getNumber(char* fileName, int lineNumber, int columnNumber)
{
    FILE* file = fopen(fileName, "r");
    char line[256] = "";
    int i = 0;
    fseek(file, 0, 0);
    while (fgets(line, sizeof(line), file)) {
        i++;
        if(i == lineNumber)
            break;
    }

    int howManyNumbers = 1;
    int j;
    for(j = 0; j < sizeof(line); j++)
    {
        if(line[j] == ' ')
            howManyNumbers++;
        else if(howManyNumbers == columnNumber)
        {
            break;
        }
    }
    int k = j;
    int number = 0;
    while(k < strlen(line) && line[k] != ' ')
    {
        if(line[k] >= '0' && line[k] <= '9')
        {
            number = number*10 + (line[k] - '0');
        }
        k++;
    }
    if(line[j] == '-')
        number *= (-1);
    fclose(file);
    return number;
}

bool timePassed(int maxTimeInSecond, clock_t t)
{
    return (double)(clock() - t)/CLOCKS_PER_SEC >= maxTimeInSecond;
}

int childProcess(int maxTimeInSecond, char* matrixAName, char* matrixBName, int colBBegin, int colBEnd, int sizeAy,
        int sizeBy, char* outputMatrix, int colsB)
{
    clock_t t;
    t = clock();
    int howManyMultiplications = 0;
    int result = 0;
    int oneMultiply;
    FILE* file = NULL;

    if(strcmp(option, "2") == 0) //osobne pliki
    {
        char outputFile[20] = "";
        sprintf(outputFile, "%d.txt", getpid());
        file = fopen(outputFile, "w");
    }


//    printf("begin col b %i, end col b %i\n", colBBegin, colBEnd);
    for (int i = 1; i <= sizeAy; i++)
    {
//        printf("------------------------------\n");
        for(int k = colBBegin; k < colBEnd; k++)
        {
            result = 0;
            for (int j = 1; j <= sizeBy; j++) {
                oneMultiply = getNumber(matrixAName, i, j) * getNumber(matrixBName, j, k);
//                printf("oneMultiply: %i, (i,j) = (%i, %i)  (j, k) = (%i, %i)\n", oneMultiply, i, j, j, k);
                if (timePassed(maxTimeInSecond, t))
                    break;
                result += oneMultiply;
            }
            if (timePassed(maxTimeInSecond, t)) {
//                printf("koniec\n");
                break;

            }
//            printf("value: %i to pos %i %i\n", result, i, k);
            if(strcmp(option, "1") == 0)
            {
                file = fopen(outputMatrix, "r+");
                flockfile(file);
                fseek(file, sizeOfNumber * (colsB * (i - 1) + (k - 1)) * sizeof(char) + (i - 1), 0);
                fprintf(file, "%i", result);
                funlockfile(file);
            }
//            printf("");
            if(strcmp(option, "2") == 0)
            {
                fprintf(file, "%s ", intToString(result, sizeOfNumber));
            }
            howManyMultiplications++;
        }
        if(strcmp(option, "2") == 0) fprintf(file, "\n");
    }
    if(strcmp(option, "2") == 0) fclose(file);
    return howManyMultiplications;
}


void getMatrixSize(char* fileName, int* rows, int* cols)
{
    FILE* file = fopen(fileName, "r");

    char line[256] = "";
    int howManyRows = 1;
    if(!fgets(line, sizeof(line), file))
    {
        printf("file is empty");
        (*rows) = 0;
        (*cols) = 0;
        fclose(file);
        return;
    }
    int howManyNumbers = 1;
//    printf("");
    bool wasNumber = true;
    for(int j = 0; j < sizeof(line); j++)
    {
        if(line[j] == ' ' && wasNumber)
        {
            howManyNumbers++;
            wasNumber = false;
        }
        else if(line[j] != ' ')
            wasNumber = true;

    }
    (*cols) = howManyNumbers;

    while (fgets(line, sizeof(line), file)) {
        howManyRows++;
    }
    (*rows) = howManyRows;
    fclose(file);
}


void multiplyMatrices(char* matrixA, char* matrixB, char* outputMatrix, int numberOfWorkerProcess, int timeForProcess)
{
    pid_t PIDs[numberOfWorkerProcess];
    for(int i = 0; i < numberOfWorkerProcess; i++)
        PIDs[i] = -1;

    FILE* fMatrixA = fopen(matrixA, "r");
    FILE* fMatrixB = fopen(matrixB, "r");

    if(fMatrixA == NULL || fMatrixB == NULL)
    {
        printf("Couldn't open file with input matrices");
        return;
    }
    int rowsA, rowsB, colsA, colsB;
    getMatrixSize(matrixA, &rowsA, &colsA);
    getMatrixSize(matrixB, &rowsB, &colsB);
//    printf("Matrix A %i %i, Matrix B %i %i\n", rowsA, colsA, rowsB, colsB);
    if(colsA != rowsB)
    {
        printf("Wrong size of matrices %i %i", colsA, rowsB);
        fclose(fMatrixA);
        fclose(fMatrixB);
        return;
    }

    if(strcmp(option, "1") == 0)
    {
        FILE* file = fopen(outputMatrix, "w");
        char* line = calloc(sizeOfNumber * colsB + 1, sizeof(char));
        for(int i = 0; i < (sizeOfNumber * colsB); i++)
        {
            line[i] = ' ';
        }
        line[sizeOfNumber * colsB] = '\n';
        for(int i = 0; i < rowsA; i++)
        {
            fprintf(file, "%s", line);
        }
        free(line);
        fclose(file);
    }

    int colsPerProcess = (int)((colsB + 0.0)/(numberOfWorkerProcess + 0.0) + 0.999);
//    printf("cols per process: %i", colsPerProcess);
//    printf("colsB %i", colsB);

    for(int i = 0; i < numberOfWorkerProcess; i++)
    {
        if(colsPerProcess * i + 1 > colsB)
        {
            break;
        }
        pid_t pid = fork();
        if(pid < 0)
        {
            printf("error fork");
            return;
        }

        if(pid == 0) {
//            printf("%i\n", getpid());
            int result = childProcess(timeForProcess, matrixA, matrixB, colsPerProcess * i + 1,
                         min(colsPerProcess * (i + 1) + 1, colsB + 1), rowsA, rowsB, outputMatrix, colsB);
            exit(result);
        } else
        {
            PIDs[i] = pid;
        }
    }
    sleep(1);
    char cmd[1000] = "";

    for (int i = 0; i < numberOfWorkerProcess; ++i){
        pid_t pid = PIDs[i];
        wait(&PIDs[i]);
        if (WIFEXITED(PIDs[i])){
            pid_t ret = WEXITSTATUS(PIDs[i]);
            char outputFile[20] = "";
            sprintf(outputFile, "%i.txt ", pid);
            if(strcmp(option, "2") == 0) strcat(cmd, outputFile);
            printf("Proces %d wykonal %d mnozen.\n", pid, ret);
//            printf("cmd: %s\n", cmd);
        }
    }
    if(strcmp(option, "2") == 0) {
        char cmd1[1000] = "";
        strcat(cmd1, "paste -d ' ' ");
        strcat(cmd1, cmd);
        strcat(cmd1, " > ");
        strcat(cmd1, outputMatrix);
        system(cmd1);
        char cmd2[1000] = "";
        strcat(cmd2, "rm ");
        strcat(cmd2, cmd);
        system(cmd2);
    }
    fclose(fMatrixA);
    fclose(fMatrixB);
}


int main(int argc, char* argv[])
{
    if(argc != 5) {
        printf("Wrong number of arguments");
        return 0;
    }
    char* matrixA = malloc(50 * sizeof(char));
    char* matrixB = malloc(50 * sizeof(char));
    char* outputMatrix = malloc(50 * sizeof(char));

    int numberOfWorkerProcess = atoi(argv[2]);
    if(numberOfWorkerProcess <= 0)
    {
        printf("Number of worker process must be positive integer");
        return 0;
    }

    int timeForProcess = atoi(argv[3]);
    if(timeForProcess <= 0)
    {
        printf("Time for one process must be positive integer");
        return 0;
    }
    option = argv[4];



    readMatricesNames(argv[1], matrixA, matrixB, outputMatrix);
    multiplyMatrices(matrixA, matrixB, outputMatrix, numberOfWorkerProcess, timeForProcess);

    free(matrixA);
    free(matrixB);
    free(outputMatrix);

    return 0;
}