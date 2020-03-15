#include "library.h"
#include "time.h"
#include "stdbool.h"
#include <sys/times.h>

void printTime(struct timespec time, struct tms timeV2)
{
    struct timespec time2;
    struct tms time2V2;
    times(&time2V2);
    clock_gettime(1, &time2);
    printf("real time: %ld:%ld\n", time2.tv_sec - time.tv_sec, time2.tv_nsec - time.tv_nsec);
    printf("user time: %ld\n", time2V2.tms_cutime - timeV2.tms_cutime);
    printf("cpu time: %ld\n", time2V2.tms_cstime - timeV2.tms_cstime);

    FILE *file = fopen("../lab1/report.txt", "r+");
    if(file != NULL)
    {
        fprintf(file,"real time: %ld:%ld\n", time2.tv_sec - time.tv_sec,  time2.tv_nsec - time.tv_nsec);
        fprintf(file,"user time: %ld\n", time2V2.tms_cutime - timeV2.tms_cutime);
        fprintf(file,"cpu time: %ld\n", time2V2.tms_cstime - timeV2.tms_cstime);
        fclose(file);
    }
}


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
        if(strcmp(argv[i], "compare_pairs") == 0) {
            char **arrayWithFilesNames = calloc(0, sizeof(char *));
            i++;
            bool timerOn = false;
            struct timespec time;
            struct tms timeV2;
            if (strcmp(argv[i], "-t") == 0) {
                i++;
                timerOn = true;
                clock_gettime(1, &time);
                times(&timeV2);
            }
            int howManyFiles = 0;

            while (i < argc && strcmp(argv[i], "compare_pairs") != 0 && strcmp(argv[i], "remove_block") != 0 &&
                   strcmp(argv[i], "remove_operation") != 0 && strcmp(argv[i], "save") != 0) {
                howManyFiles++;
                arrayWithFilesNames = realloc(arrayWithFilesNames, sizeof(arrayWithFilesNames) * howManyFiles);
                arrayWithFilesNames[howManyFiles - 1] = argv[i];
                i++;
            }
            findDifferencesInFilesAndSaveResults("../lab1/result.txt", howManyFiles, arrayWithFilesNames);
//            creatNewBlockFrom(&mainArray, "../lab1/result.txt");
            if (timerOn)
                printTime(time, timeV2);
            continue;
        } else if (strcmp(argv[i], "save") == 0)
        {
            i++;
            bool timerOn = false;
            struct timespec time;
            struct tms timeV2;
            if (strcmp(argv[i], "-t") == 0) {
                timerOn = true;
                clock_gettime(1, &time);
                times(&timeV2);
            } else
                i--;
            creatNewBlockFrom(&mainArray, "../lab1/result.txt");
            if (timerOn)
                printTime(time, timeV2);

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
                printTime(time, timeV2);

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
                printTime(time, timeV2);
        }
        i++;
    }

    return 0;
}