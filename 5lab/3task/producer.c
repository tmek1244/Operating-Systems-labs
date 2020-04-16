#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int openFile(char* name, int mode)
{
    int fileDescriptor = open(name, mode);
    if(fileDescriptor == -1)
    {
        printf("Cannot open %s", name);
        exit(-1);
    }
    return fileDescriptor;
}


int produce(int pipe, int inputFile, int n)
{

    char* string = calloc(n, sizeof(char));
    char* result = calloc(n + 15, sizeof(char));
    size_t amount;
    while((amount = read(inputFile, string, n)) > 0)
    {
        sleep(1);
        sprintf(result, "#%d#%s",getpid(), string);
        write(pipe, result, strlen(result));
        for(int i = 0; i < n; i++)
            string[i] = '\0';
    }

    free(string);
    free(result);
    exit(0);
}

void startProduction(char* pipeName, char* inputFileName, int n)
{
    mkfifo(pipeName, 0666);

    int inputFile = openFile(inputFileName, O_RDONLY);
    int pipe = openFile(pipeName, O_WRONLY);

    produce(pipe, inputFile, n);

    close(inputFile);
    close(pipe);
}

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        printf("Wrong number of arguments\n");
        return -1;
    }
    int n = atoi(argv[3]);
    printf("PID: %d, input file: %s\n", getpid(), argv[2]);
    if(n == 0)
    {
        printf("N must be positive integer\n");
        return -2;
    }
    startProduction(argv[1], argv[2], n);
    return 0;
}