#include "library.h"
#include "time.h"
#include "stdbool.h"
#include <sys/times.h>

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        printf("First argument is size of the array");
        return 0;
    }
    if(atoi(argv[1]) == 0)
    {
        printf("Size must be positive integer!");
        return 0;
    }

    struct MainArray mainArray = creatMainArray(atoi(argv[1]));
    
    int i = 2;
    while(i < argc)
    {
        if(strcmp(argv[i], "compare_pairs") == 0)
        {
            char** arrayWithFilesNames = calloc(0, sizeof(char*));
            i++;
            bool timerOn = false;
            struct timespec time;
            struct tms timeV2;
            if(strcmp(argv[i], "-t") == 0)
            {
                i++;
                timerOn = true;
                clock_gettime(1, &time);
                times(&timeV2);
            }
            int howManyFiles = 0;

            while(i < argc && strcmp(argv[i], "compare_pairs") != 0 && strcmp(argv[i], "remove_block") != 0 &&
                    strcmp(argv[i], "remove_operation") != 0)
            {
                howManyFiles++;
                arrayWithFilesNames = realloc(arrayWithFilesNames, sizeof(arrayWithFilesNames) * howManyFiles);
                arrayWithFilesNames[howManyFiles - 1] = argv[i];
                i++;
            }
            findDifferencesInFilesAndSaveResults("../labs1/result.txt", howManyFiles, arrayWithFilesNames);
            creatNewBlockFrom(&mainArray, "../labs1/result.txt");
            if(timerOn)
            {
                struct timespec time2;
                struct tms time2V2;
                times(&time2V2);
                clock_gettime(1, &time2);
                printf("real time: %ld\n", time2.tv_sec - time.tv_sec);
                printf("user time: %ld\n", time2V2.tms_cutime - timeV2.tms_cutime);
                printf("cpu time: %ld\n", time2V2.tms_cstime - timeV2.tms_cstime);
            }
            continue;
        } else if (strcmp(argv[i], "remove_block") == 0)
        {
            i++;
            bool timerOn = false;
            struct timespec time;
            struct tms timeV2;
            if(strcmp(argv[i], "-t") == 0)
            {
                i++;
                timerOn = true;
                clock_gettime(1, &time);
                times(&timeV2);
            }
            int index = atoi(argv[i]);
            if(index == 0 && strcmp(argv[i], "0") != 0)
            {
                printf("After remove_block has to be not negative integer!");
                i++;
                continue;
            }
            deleteOperationBlock(mainArray, index);
            if(timerOn)
            {
                struct timespec time2;
                struct tms time2V2;
                times(&time2V2);
                clock_gettime(1, &time2);
                printf("real time: %ld\n", time2.tv_sec - time.tv_sec);
                printf("user time: %ld\n", time2V2.tms_cutime - timeV2.tms_cutime);
                printf("cpu time: %ld\n", time2V2.tms_cstime - timeV2.tms_cstime);
            }
        } else if (strcmp(argv[i], "remove_operation") == 0)
        {
            i++;
            bool timerOn = false;
            struct timespec time;
            struct tms timeV2;
            if(strcmp(argv[i], "-t") == 0)
            {
                i++;
                timerOn = true;
                clock_gettime(1, &time);
                times(&timeV2);
            }
            int blockIndex = atoi(argv[i]);
            if(blockIndex == 0 && strcmp(argv[i], "0") != 0)
            {
                printf("After remove_operation has to be two not negative integers!");
                i++;
                continue;
            }
            i++;
            int operationIndex= atoi(argv[i]);
            if(operationIndex == 0 && strcmp(argv[i], "0") != 0)
            {
                printf("After remove_block has to be two not negative integers!");
                i++;
                continue;
            }
            deleteOperationInBlockNr(mainArray, blockIndex, operationIndex);
            if(timerOn)
            {
                struct timespec time2;
                struct tms time2V2;
                times(&time2V2);
                clock_gettime(1, &time2);
                printf("real time: %ld\n", time2.tv_sec - time.tv_sec);
                printf("user time: %ld\n", time2V2.tms_cutime - timeV2.tms_cutime);
                printf("cpu time: %ld\n", time2V2.tms_cstime - timeV2.tms_cstime);
            }
        }
        i++;
    }

//    findDifferencesInFilesAndSaveResults("../labs1/result.txt", 4,
//            ["../labs1/file1.txt", "../labs1/file2.txt", "../labs1/file3.txt", "../labs1/file4.txt"]);

//    creatNewBlockFrom(array, "../labs1/result.txt");
////    printf("%s", array.blocks[0]->arrayOfOperations[0]);
////    deleteOperationBlock(array, 1);
//    deleteOperationInBlockNr(array, 1, 0);
////    printf("%d", howManyOperationsInBlockNr(array, 1));
//    printf("%d", howManyOperationsInBlockNr(array, 0));
////    printf("%s", array.blocks[1]->arrayOfOperations[0]);

    return 0;
}