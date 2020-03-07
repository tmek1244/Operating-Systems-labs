#include "library.h"


char* stringsConcatenate(int numberOfStrings, ...)
{
    va_list arguments;
    va_start(arguments, numberOfStrings);

    char *result = NULL;

    for(int i = 0; i < numberOfStrings; i++)
    {
        result = strcat(result, va_arg(arguments, char*));
    }
    return result;
}

// ----------------------------- FIRST FUNCTION -------------------------------------
struct MainArray creatMainArray(int size)
{
    struct MainArray array;

    array.blocks = (struct Block **) calloc(size, sizeof(struct Block *));
    array.lastElementIndex = -1;
    array.size = size;

    return array;
}

//void addFilePairToMainArray(struct MainArray mainArray, char* file1, char* file2)
//{
//    if(mainArray.size > mainArray.lastElementIndex + 1)
//    {
//        mainArray.lastElementIndex++;
//        mainArray.blocks[mainArray.lastElementIndex].file1Name = file1;
//        mainArray.blocks[mainArray.lastElementIndex].file2Name = file2;
//    }
//}


// ----------------------------- SECOND FUNCTION -------------------------------------

void findDifferencesInFilesAndSaveResults(char* resultFile, int numberOfPairs, ...)
{
    if(numberOfPairs % 2 == 1)
    {
        printf("Wrong number of files!");
        return;
    }

    FILE* file;
    file = fopen(resultFile, "w");


    va_list arguments;
    va_start(arguments, numberOfPairs);
    for(int i = 0; i < numberOfPairs/2; i++)
    {
        system(stringsConcatenate(5, "diff ", va_arg(arguments, char*), " ",
                va_arg(arguments, char*), " >> ", resultFile));
        //addFilePairToMainArray(mainArray, va_arg(arguments, char*), va_arg(arguments, char*));
    }
    fclose(file);
}


// ----------------------------- THIRD FUNCTION -------------------------------------

int creatNewBlockFrom(struct MainArray mainArray, char* fileName)
{
    mainArray.blocks[0];
    return 0;
}

// ----------------------------- FOURTH FUNCTION -------------------------------------

int howManyOperationsInBlockNr(struct MainArray mainArray, int blockNumber)
{
    if(mainArray.size >= blockNumber || mainArray.blocks[blockNumber] == NULL)
    {
        printf("There is no block with this number!");
        return -1;
    }

    return mainArray.blocks[blockNumber]->operationCounter;
}


// ----------------------------- FIFTH FUNCTION -------------------------------------

void deleteOperationBlock(struct MainArray mainArray, int blockNumber)
{
    if(mainArray.size >= blockNumber)
    {
        printf("There is no block with this number!");
        return;
    }
    if(mainArray.blocks[blockNumber] == NULL)
    {
        printf("This block is already empty");
        return;
    }

    free(mainArray.blocks[blockNumber]);
    mainArray.blocks[blockNumber] = NULL;
}

// ----------------------------- SIXTH FUNCTION -------------------------------------

void deleteOperationInBlockNr(struct MainArray mainArray, int blockNumber, int operationNumber)
{
    if(mainArray.size >= blockNumber || mainArray.blocks[blockNumber] == NULL)
    {
        printf("There is no block with this number!");
        return;
    }
    if(mainArray.blocks[blockNumber]->operationCounter < operationNumber)
    {
        printf("There is no operation with this number");
        return;
    }
    free(mainArray.blocks[blockNumber]->arrayOfOperations[operationNumber]);
    mainArray.blocks[blockNumber]->arrayOfOperations[operationNumber] = NULL;
}


//
//void findDifferences(char* outputFileName, struct MainArray mainArray)
//{
////    FILE *outputFile = fopen(outputFileName, "w");
//    for(int i = 0; i < mainArray.size; i++)
//    {
//        system(stringsConcatenate(4,
//                                  "diff ", mainArray.blocks[i].file1Name, " ", mainArray.blocks[i].file2Name));
//    }
//}
