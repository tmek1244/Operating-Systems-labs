#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

FILE* openAndCheck(char* fileName, char* mode)
{
    FILE* file;
    file = fopen(fileName, mode);
    if(file == NULL)
    {
        printf("Cannot open file\n");
    }
    return file;
}

char* stringsConcatenate(int numberOfStrings, ...)
{
    va_list arguments;
    va_start(arguments, numberOfStrings);

    char* result = calloc(1000, sizeof(char));

    for(int i = 0; i < numberOfStrings; i++)
    {
        result = strcat(result, va_arg(arguments, char*));
    }
    return result;
}

void generateRandomFile(char* destinationFileName, int howManyLines, char* bytesPerLine)
{
    char* command =  "cat /dev/urandom | base64 | head -c ";
    system(stringsConcatenate(2, "printf '' > ", destinationFileName));
    for(int i = 0; i < howManyLines; i++) {
        system(stringsConcatenate(5, "( ", command, bytesPerLine, " && echo ) >> ", destinationFileName));
    }
}

void copyFilesLib(char* inputFile, char* outputFile, int recordCount, int buffSize)
{
    FILE* fileInput = fopen(inputFile, "r");
    FILE* fileOutput = fopen(outputFile, "w");
    if(fileInput == NULL || fileOutput == NULL)
    {
        printf("Couldn't open file");
        return;
    }
    char* tmp = calloc(buffSize + 1, sizeof(char));
    for(int i = 0; i < buffSize + 1; i++)
        tmp[i] = 0;
    int i = 0;
    while(i < recordCount)
    {
        i++;
        if( (fread(tmp, 1, buffSize + 1, fileInput)) != buffSize + 1 )
        {
            printf("Sth gone wrong");
            break;
//            return;
        }
        if ( (fwrite(tmp, sizeof(char), buffSize+1, fileOutput)) != buffSize+1 ){
            printf("Sth gone wrong");
            break;
        }
    }
    free(tmp);
    fclose(fileInput);
    fclose(fileOutput);
}

void copyFilesSys(char* inputFile, char* outputFile, int recordCount, int buffSize)
{
    int input = open(inputFile, O_RDONLY);
    int output = open(outputFile, O_WRONLY | O_CREAT,S_IRUSR | S_IWUSR);
    char* tmp = calloc(buffSize + 1, sizeof(char));
    for(int i = 0; i < recordCount; i++)
    {
        if(read(input, tmp, buffSize + 1) != buffSize + 1)
        {
            printf("Error during reading file");
            return;
        }
        write(output, tmp, buffSize + 1);
    }
    close(input);
    close(output);
    free(tmp);
}
//----------------------------QUICK SORT LIB---------------------------------------
char* readSeekLib(FILE* file, int pos, int buffSize, char* line, int sizeOfElement)
{
    fseek(file, pos * (buffSize + 1), 0);
    fread(line, sizeOfElement, buffSize + 1, file);
    return line;
}

void writeSeekLib(FILE* file, int pos, int buffSize, char* line, int sizeOfElement)
{
    fseek(file, pos * (buffSize + 1), 0);
    fwrite(line, sizeOfElement, buffSize + 1, file);
}

void swapLib(FILE* file, int a, int b, int sizeOfElement, int buffSize)
{
    char* lineA = calloc(buffSize + 1, sizeOfElement);
    char* lineB = calloc(buffSize + 1, sizeOfElement);
    readSeekLib(file, a, buffSize, lineA, sizeOfElement);
    readSeekLib(file, b, buffSize, lineB, sizeOfElement);
    writeSeekLib(file, a, buffSize, lineB, sizeOfElement);
    writeSeekLib(file, b, buffSize, lineA, sizeOfElement);
    free(lineA);
    free(lineB);
}

int makePivotLib(FILE* file, int left, int right, int buffSize, int sizeOfElement)
{
    int i = left - 1;
    int j = right + 1;
    char* pivot = calloc(buffSize + 1, sizeOfElement);
    readSeekLib(file, (i + j) / 2, buffSize, pivot, sizeOfElement);
    char* linaA = calloc(buffSize + 1, sizeOfElement);
    char* linaB = calloc(buffSize + 1, sizeOfElement);
    while(1)
    {
        while(strcmp(pivot, readSeekLib(file, ++i, buffSize, linaA, sizeOfElement)) < 0);
        while(strcmp(pivot, readSeekLib(file, --j, buffSize, linaB, sizeOfElement)) > 0);

        if(i < j)
            swapLib(file, i, j, sizeOfElement, buffSize);
        else
            break;
    }
    free(pivot);
    free(linaA);
    free(linaB);
    return i;
}

void sortOpenFileLib(FILE* file, int left, int right, int buffSize)
{
    int i = makePivotLib(file, left, right, buffSize, sizeof(char));
    if(i + 1 < right)
        sortOpenFileLib(file, i + 1, right, buffSize);
    if(i > left)
        sortOpenFileLib(file, left, i, buffSize);
}

void sortFileLib(char* inputFile, int recordCounter, int buffSize)
{
    FILE* file = fopen(inputFile, "r+");
    if(file == NULL)
    {
        printf("Cannot open file!");
        return;
    }
    sortOpenFileLib(file, 0, recordCounter, buffSize);
    fclose(file);
}

//----------------------------QUICK SORT SYS---------------------------------------
char* readSeekSys(int fileID, int pos, int buffSize, char* line, int sizeOfElement)
{
    lseek(fileID, pos * (buffSize + 1), 0);
    read(fileID, line, buffSize + 1);
    return line;
}

void writeSeekSys(int fileID, int pos, int buffSize, char* line, int sizeOfElement)
{
    lseek(fileID, pos * (buffSize + 1), 0);
    write(fileID, line, buffSize + 1);
}

void swapSys(int fileID, int a, int b, int sizeOfElement, int buffSize)
{
    char* lineA = calloc(buffSize + 1, sizeOfElement);
    char* lineB = calloc(buffSize + 1, sizeOfElement);
    readSeekSys(fileID, a, buffSize, lineA, sizeOfElement);
    readSeekSys(fileID, b, buffSize, lineB, sizeOfElement);
    writeSeekSys(fileID, a, buffSize, lineB, sizeOfElement);
    writeSeekSys(fileID, b, buffSize, lineA, sizeOfElement);
    free(lineA);
    free(lineB);
}

int makePivotSys(int fileID, int left, int right, int buffSize, int sizeOfElement)
{
    int i = left - 1;
    int j = right + 1;
    char* pivot = calloc(buffSize + 1, sizeOfElement);
    readSeekSys(fileID, (i + j) / 2, buffSize, pivot, sizeOfElement);
    char* linaA = calloc(buffSize + 1, sizeOfElement);
    char* linaB = calloc(buffSize + 1, sizeOfElement);
    while(1)
    {
        while(strcmp(pivot, readSeekSys(fileID, ++i, buffSize, linaA, sizeOfElement)) < 0);
        while(strcmp(pivot, readSeekSys(fileID, --j, buffSize, linaB, sizeOfElement)) > 0);

        if(i < j)
            swapSys(fileID, i, j, sizeOfElement, buffSize);
        else
            break;
    }
    free(pivot);
    free(linaA);
    free(linaB);
    return i;
}

void sortOpenFileSys(int fileID, int left, int right, int buffSize)
{
    int i = makePivotSys(fileID, left, right, buffSize, sizeof(char));
    if(i + 1 < right)
        sortOpenFileSys(fileID, i + 1, right, buffSize);
    if(i > left)
        sortOpenFileSys(fileID, left, i, buffSize);
}

void sortFileSys(char* inputFile, int recordCounter, int buffSize)
{
    int fileID = open(inputFile, O_RDWR);
    if(fileID < 0)
    {
        printf("Cannot open file!");
        return;
    }
    sortOpenFileSys(fileID, 0, recordCounter, buffSize);
    close(fileID);
}
//---------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    if(strcmp(argv[1], "generate") == 0)
    {
        generateRandomFile(argv[2], atoi(argv[3]), argv[4]);
    } else if (strcmp(argv[1], "copy") == 0)
    {
        if(strcmp(argv[6], "lib") == 0)
            copyFilesLib(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
        if(strcmp(argv[6], "sys") == 0)
            copyFilesSys(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
    } else if (strcmp(argv[1], "sort") == 0 && strcmp(argv[5], "sys") == 0)
    {
        sortFileSys(argv[2], atoi(argv[3]), atoi(argv[4]));
    } else if (strcmp(argv[1], "sort") == 0 && strcmp(argv[5], "lib") == 0)
    {
        sortFileLib(argv[2], atoi(argv[3]), atoi(argv[4]));
    }
    return 0;
}