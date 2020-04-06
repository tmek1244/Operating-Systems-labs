#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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
    FILE* outputFile = openFile(outputFileName, "w");
    FILE* pipe = openFile(pipeName, "r");

    char* string = calloc(n, sizeof(char));
    size_t result;

    while((result = fread(string, sizeof(char), n, pipe)) > 0)
    {
        fwrite(string, sizeof(char), result, outputFile);
    }
    free(string);
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