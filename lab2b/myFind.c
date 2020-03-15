#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <stdbool.h>


bool mtimeG = false, atimeG = false;
char* mtimeValueG;
char* atimeValueG;

char *typeOfFile(unsigned char type) {
    switch(type) {
        case 8: return "file"; //DT_REG      8
        case 4: return "dir"; //DT_DIR       4
        case 2: return "char dev"; //DT_CHR  2
        case 6: return "block dev"; //DT_BLK 6
        case 1: return "fifo"; //DT_FIFO     1
        case 10: return "slink"; //DT_LNK    10
        case 12: return "sock"; //DT_SOCK    12
        default: return "";
    };
}

int dayDifferenceBetween(struct tm *data1, struct tm *data2)
{

//    printf("w dayDifferenceBetween %i/%i/%i\n", data1->tm_mday, data1->tm_mon, data1->tm_year);
//    printf("w dayDifferenceBetween %i/%i/%i\n", data2->tm_mday, data2->tm_mon, data2->tm_year);

    int day1, day2;

    day1 = data1->tm_mday + 31*(data1->tm_mon + 1) + 365*data1->tm_year;
    day2 = data2->tm_mday + 31*(data2->tm_mon + 1) + 365*data2->tm_year;
//    printf("data1: %i/%i/%i", data1->tm_mday, data1->tm_mon, data1->tm_year);
//    printf("data2: %i/%i/%i", data2->tm_mday, data2->tm_mon, data2->tm_year);

//    printf("ile dni: %i, %i", day1, day2);
    return day2 - day1;
}

void getValueWithChar(const char* input, char* prefix, int* value)
{
    if(input[0] == '+')
        (*prefix) = '+';
    else if (input[0] == '-')
        (*prefix) = '-';
    else {
        (*prefix) = '0';
        (*value) = atoi(input);
        return;
    }
    (*value) = atoi(input+1);
}

bool checkTime(char* timeDiff, struct tm *actionTime)
{
    int value;
    char prefix;
    getValueWithChar(timeDiff, &prefix, &value);
    struct tm *todayDate;
    time_t tas;
    time(&tas);
    todayDate = localtime(&tas);
//    printf("%i/%i/%i\n", actionTime->tm_mday, actionTime->tm_mon, actionTime->tm_year);
//    printf("dzisiejsza data: %i/%i/%i\n", todayDate->tm_mday, todayDate->tm_mon, todayDate->tm_year);
    int dayDifference = dayDifferenceBetween(actionTime, todayDate);
//    printf("roznica dni: %i\n", dayDifference);
    if(prefix == '+' && dayDifference >= value)
        return true;
    if(prefix == '-' && dayDifference <= value)
        return true;
    if(prefix == '0' && dayDifference == value)
        return true;
    return false;
}


void findStat(char* path, bool mtime, char* mtimeValue, bool atime, char* atimeValue, bool maxDepth, int maxDepthValue)
{
    DIR* directory = opendir(path);

    if(maxDepth && maxDepthValue == 0)
        return;

    if(directory == NULL) {
        printf("Cannot open directory!");
        return;
    }

    struct dirent *file;
    struct tm *modTime;
    struct tm *accTime;
    char buffer[50];
    char *absPath = (char*) calloc(100, sizeof(char));
    char *nextPath = (char*) calloc(100, sizeof(char));
    struct stat stats;

    while((file = readdir(directory)))
    {
        strcpy(nextPath, path);
        strcat(nextPath, "/");
        strcat(nextPath, file->d_name);
        realpath(nextPath, absPath);

        stat(nextPath, &stats);

        if(strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0)
        {
            modTime = localtime(&stats.st_mtime);
            accTime = localtime(&stats.st_atime);
//            printf("przd wejsciem do funkcji %i/%i/%i\n", modTime->tm_mday, modTime->tm_mon, modTime->tm_year);
//            printf("%i\n", checkTime(mtimeValue, modTime));
            if( (!mtime || checkTime(mtimeValue, modTime)) && (!atime || checkTime(atimeValue, accTime)) )
            {
                printf("\nPath:                %s\n"
                       "Number of hardlink:  %lu\n"
                       "Type:                %s\n"
                       "Size:                %ld\n",
                       absPath,
                       stats.st_nlink,
                       typeOfFile(file->d_type),
                       stats.st_size);
                strftime (buffer, 50 ,"Last access:         %d.%m.%Y\n", accTime);
                printf("%s", buffer);
                strftime (buffer, 50 ,"Last mod.:           %d.%m.%Y\n", modTime);
                printf("%s", buffer);
            }

            if(file->d_type == 4)
                findStat(nextPath, mtime, mtimeValue, atime, atimeValue, maxDepth, maxDepthValue - 1);
        }
    }

    free(nextPath);
    free(absPath);
    free(file);
    closedir(directory);
}

char *typeOfFileNftw(int type){
    switch(type) {
        case FTW_F: return "file";
        case FTW_D: return "dir";
        case FTW_SL: return "link";
        default: return "";
    };
}

int displayInfo(const char* path, const struct stat infoAboutFile, int flagType, struct FTW* twBuff)
{
    printf("%d", flagType);
    struct tm *modTime;
    struct tm *accTime;

    char buffer[50];
    char *absPath = (char*) calloc(100, sizeof(char));

    realpath(path, absPath);

    modTime = localtime(&infoAboutFile.st_mtime);
    accTime = localtime(&infoAboutFile.st_atime);
//            printf("przd wejsciem do funkcji %i/%i/%i\n", modTime->tm_mday, modTime->tm_mon, modTime->tm_year);
//            printf("%i\n", checkTime(mtimeValue, modTime));
    if( (!mtimeG || checkTime(mtimeValueG, modTime)) && (!atimeG || checkTime(atimeValueG, accTime)) )
    {
        printf("\nPath:                %s\n"
               "Number of hardlink:  %lu\n"
               "Type:                %s\n"
               "Size:                %ld\n",
               absPath,
               infoAboutFile.st_nlink,
               typeOfFileNftw(flagType),
               infoAboutFile.st_size);
        strftime (buffer, 50 ,"Last access:         %d.%m.%Y\n", accTime);
        printf("%s", buffer);
        strftime (buffer, 50 ,"Last mod.:           %d.%m.%Y\n", modTime);
        printf("%s", buffer);
    }
    free(absPath);
    return 0;
}


void findNFTW(char* path)
{
    nftw(path, (__nftw_func_t) displayInfo, 10, FTW_PHYS);
}

int main(int argc, char* argv[])
{
    bool mtime = false, atime = false, maxDepth = false;
    char* mtimeValue = NULL;
    char* atimeValue = NULL;
    int maxDepthValue = 0;

    for(int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-mtime") == 0) {
            mtime = true;
            mtimeG = true;
            mtimeValue = argv[++i];
            mtimeValueG = argv[i];
        } else if (strcmp(argv[i], "-atime") == 0) {
            atime = true;
            atimeG = true;
            atimeValue = argv[++i];
            atimeValueG = argv[i];
        } else if (strcmp(argv[i], "-maxDepth") == 0) {
            maxDepth = true;
            maxDepthValue = atoi(argv[++i]);
        }
    }

    findStat(argv[1], mtime, mtimeValue, atime, atimeValue, maxDepth, maxDepthValue);
    printf("\n--------------------------------------------------\n");
    findNFTW(argv[1]);

    return 0;
}