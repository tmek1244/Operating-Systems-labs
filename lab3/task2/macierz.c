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
    char line[256];
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

int childProcess(int maxTimeInSecond, char* matrixAName, char* matrixBName, int colBBegin, int colBEnd, int sizeAy, int sizeBx, int sizeBy)
{
    clock_t t;
    t = clock();
    int howManyMultiplications = 0;
    int result = 0;
    int oneMultiply;
    FILE* file = NULL;
    if(strcmp(option, "1") == 0) // wspolny plik
    {

    }
    else if(strcmp(option, "2") == 0) //osobne pliki
    {
        char outputFile[20];
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
            printf("value: %i to pos %i %i\n", result, i, k);
            if(strcmp(option, "2") == 0)
            {
                fprintf(file, "%s ", intToString(result, 6));
            }
            howManyMultiplications++;
        }
        fprintf(file, "\n");
    }
//    fclose(matrixA);
//    fclose(matrixB);
//    printf("how many multipications: %i\n", howManyMultiplications);
    if(strcmp(option, "2") == 0) fclose(file);
    return howManyMultiplications;
}


void getMatrixSize(char* fileName, int* rows, int* cols)
{
    FILE* file = fopen(fileName, "r");

    char line[256];
    int howManyRows = 1;
    fseek(file, 0, 0);
    if(!fgets(line, sizeof(line), file))
    {
        printf("file is empty");
        (*rows) = 0;
        (*cols) = 0;
        fclose(file);
        return;
    }

    int howManyNumbers = 1;
    printf("");
    for(int j = 0; j < sizeof(line); j++)
    {
        if(line[j] == ' ')
            howManyNumbers++;
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

    int status;

    FILE* fMatrixA = fopen(matrixA, "r");
    FILE* fMatrixB = fopen(matrixB, "r");

    if(fMatrixA == NULL || fMatrixB == NULL)
    {
        printf("Couldn't open file");
        return;
    }
    int rowsA, rowsB, colsA, colsB;
    getMatrixSize(matrixA, &rowsA, &colsA);
    getMatrixSize(matrixB, &rowsB, &colsB);
    if(colsA != rowsB)
    {
        printf("Wrong size of matrices %i %i", colsA, rowsB);
        fclose(fMatrixA);
        fclose(fMatrixB);
        return;
    }

    int colB = 0;
    int colsPerProcess = (int)((colsB + 0.0)/(numberOfWorkerProcess + 0.0) + 0.999);
//    printf("cols per process: %i", colsPerProcess);
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
                         min(colsPerProcess * (i + 1) + 1, colsB + 1), rowsA, colsB, rowsB);
            exit(result);
        } else
        {
            PIDs[i] = pid;
        }
    }
    sleep(timeForProcess);
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
            printf("cmd: %s\n", cmd);
        }
    }
    if(strcmp(option, "2") == 0) {
        char cmd1[1000] = "";
        strcat(cmd1, "paste ");
        strcat(cmd1, cmd);
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

//    int rows = 0, cols = 0;
//    FILE* file = fopen("../lab3/task2/macierzA.txt", "r");
//    if(file == NULL)
//        printf("nie udalo sie otworzyc");
//    else{
//        getMatrixSize(file, &rows, &cols);
//        printf("rows: %i, cols: %i\n", rows, cols);
//        fclose(file);
//    }

    return 0;
}