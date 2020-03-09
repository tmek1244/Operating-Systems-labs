#include "library.h"


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

// ----------------------------- FIRST FUNCTION -------------------------------------
struct MainArray creatMainArray(int size)
{
    struct MainArray array;

    array.blocks = (struct Block **) calloc(size, sizeof(struct Block *));
    array.lastElementIndex = -1;
    array.size = size;

    return array;
}

// ----------------------------- SECOND FUNCTION -------------------------------------

void findDifferencesInFilesAndSaveResults(char* resultFile, int numberOfFiles, char** arrayOfFiles)
{
    if(numberOfFiles % 2 == 1)
    {
        printf("Wrong number of files!\n");
        return;
    }

    FILE* file;
    file = fopen(resultFile, "w");
    if(file == NULL)
    {
        printf("Cannot open result file\n");
        return;
    }

    for(int i = 0; i < numberOfFiles / 2; i++)
    {
        system(stringsConcatenate(8, "diff ", arrayOfFiles[i], " ",
                arrayOfFiles[i+1], " >> ", resultFile, "; printf \"$\n\" >> ", resultFile));
        //addFilePairToMainArray(mainArray, va_arg(arguments, char*), va_arg(arguments, char*));
    }
    system(stringsConcatenate(2, "printf \"?\" >> ", resultFile));
    fclose(file);
}


// ----------------------------- THIRD FUNCTION -------------------------------------

int creatNewBlockFrom(struct MainArray *mainArray, char* fileName)
{
    FILE* file;
    file = fopen(fileName, "r");

    if(file == NULL)
    {
        printf("Couldn't open file\n");
        return -1;
    }

    char *line_buf = NULL;
    size_t line_buf_size = 0;
    getline(&line_buf, &line_buf_size, file);
//    mainArray.lastElementIndex++;
    int currentOperationBlock = mainArray->lastElementIndex + 1;
    if(currentOperationBlock >= mainArray->size)
    {
        printf("There is no space for new operation block\n");
        return -2;
    }
    int operationNumber = -1;
//    mainArray.blocks[currentOperationBlock]->arrayOfOperations;
    struct Block* newBlock = calloc(1, sizeof(struct Block));
    mainArray->blocks[currentOperationBlock] = newBlock;
    mainArray->blocks[currentOperationBlock]->arrayOfOperations = (char **) calloc(1, sizeof(char*));
    while(line_buf_size > 0)
    {
        //printf("%s", line_buf);
        if(line_buf[0] == '?')
            break;
        if(line_buf[0] >= '0' && line_buf[0] <= '9')
        {
            operationNumber++;
            mainArray->blocks[currentOperationBlock]->arrayOfOperations =
                    realloc(mainArray->blocks[currentOperationBlock]->arrayOfOperations,
                            (operationNumber + 1) * sizeof(char*));
            mainArray->blocks[currentOperationBlock]->operationCounter = operationNumber + 1;
        }
        if(line_buf[0] == '$')
        {
            mainArray->lastElementIndex++;
            operationNumber = -1;
            getline(&line_buf, &line_buf_size, file);
            if(line_buf_size == 0)
            {
                break;
            } else
            {
                operationNumber = -1;
                currentOperationBlock++;
                if(currentOperationBlock >= mainArray->size)
                {
                    printf("There is no space for new operation block!\n");
                    return -2;
                }
                mainArray->blocks[currentOperationBlock] = calloc(1, sizeof(struct Block));;
                mainArray->blocks[currentOperationBlock]->arrayOfOperations = (char **) calloc(1, sizeof(char*));
            }
        } else
        {
            if(mainArray->blocks[currentOperationBlock]->arrayOfOperations[operationNumber] == NULL)
            {
                mainArray->blocks[currentOperationBlock]->arrayOfOperations[operationNumber] = calloc(1, sizeof(char));
            }
//            operationNumber++;
//            printf("%lu", strlen(mainArray.blocks[currentOperationBlock]->arrayOfOperations[operationNumber]));
            char* result = malloc(sizeof(result) * (strlen(mainArray->blocks[currentOperationBlock]->arrayOfOperations[operationNumber]) +
                    strlen(line_buf) + 1));
            strcpy(result, mainArray->blocks[currentOperationBlock]->arrayOfOperations[operationNumber]);
            strcat(result, line_buf);
            mainArray->blocks[currentOperationBlock]->arrayOfOperations[operationNumber] = result;
//            printf("%s, %d, %d", mainArray.blocks[currentOperationBlock]->arrayOfOperations[operationNumber], currentOperationBlock, operationNumber);
            getline(&line_buf, &line_buf_size, file);
        }
    }

    return mainArray->lastElementIndex;

}

// ----------------------------- FOURTH FUNCTION -------------------------------------

int howManyOperationsInBlockNr(struct MainArray mainArray, int blockNumber)
{
    if(mainArray.lastElementIndex >= blockNumber || mainArray.blocks[blockNumber] == NULL || blockNumber < 0)
    {
        printf("There is no block with this number!\n");
        return -1;
    }

    return mainArray.blocks[blockNumber]->operationCounter;
}


// ----------------------------- FIFTH FUNCTION -------------------------------------

void deleteOperationBlock(struct MainArray mainArray, int blockNumber)
{
    if(mainArray.lastElementIndex < blockNumber || blockNumber < 0)
    {
        printf("There is no block with this number!\n");
        return;
    }
    if(mainArray.blocks[blockNumber] == NULL)
    {
        printf("This block is already empty\n");
        return;
    }
    for(int i = 0; i < mainArray.blocks[blockNumber]->operationCounter; i++)
    {
        free(mainArray.blocks[blockNumber]->arrayOfOperations[i]);
    }
    free(mainArray.blocks[blockNumber]);
    mainArray.blocks[blockNumber] = NULL;
}

// ----------------------------- SIXTH FUNCTION -------------------------------------

void deleteOperationInBlockNr(struct MainArray mainArray, int blockNumber, int operationNumber)
{
    if(mainArray.lastElementIndex < blockNumber || mainArray.blocks[blockNumber] == NULL)
    {
        printf("%d", mainArray.lastElementIndex);
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
