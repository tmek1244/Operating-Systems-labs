#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>


FILE* openFile(char* name, char* mode)
{
    FILE* file = fopen(name, mode);
    if(file == NULL)
    {
        printf("Cannot open %s", name);
        exit(-1);
    }
    return file;
}


int consume(char* pipeName, char* outputFileName, int n)
{
    mkfifo(pipeName, 0666);
    FILE* outputFile = openFile(outputFileName, "w");
    FILE* pipe = openFile(pipeName, "r");

    char string[n];
    char* result;

    while((result = fgets(string, n, pipe)) != NULL)
    {
//        printf("%s\n", result);
        while((++result)[0] != '#');
        printf("%s\n", result);

        fputs(result, outputFile);
    }

    fclose(outputFile);
    fclose(pipe);
    if(result)
        free(string);
    return 0;
}


int main(int argc, char** argv)
{
    if(argc != 4)
    {
        printf("Wrong number of arguments\n");
        return -1;
    }
    int n = atoi(argv[3]);
    if(n == 0)
    {
        printf("N must be positive integer\n");
        return -2;
    }
    consume(argv[1], argv[2], n);
    return 0;
}