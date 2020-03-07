#ifndef UNTITLED1_LIBRARY_H
#define UNTITLED1_LIBRARY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

struct MainArray
{
    struct Block** blocks;
    int lastElementIndex;
    int size;
};

struct Block
{
    char** arrayOfOperations;
    int operationCounter;
};

void creatArray(int size);
void findDifferencesInFilesAndSaveResults(char* resultFile, int numberOfPairs, ...);
int creatNewBlockFrom(struct MainArray, char* fileName);
int howManyOperationsInBlockNr(struct MainArray, int blockNumber);
void deleteOperationBlock(struct MainArray, int blockNumber);
void deleteOperationInBlockNr(struct MainArray, int blockNumber, int operationNumber);
//void findDifferences(char* outputFileName, struct MainArray);


#endif //UNTITLED1_LIBRARY_H
