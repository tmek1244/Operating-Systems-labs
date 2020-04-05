#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

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
    char string[n];
    char* result = calloc(n + 15, sizeof(char));

    while(read(inputFile, string, n) > 0)
    {
        sleep(1);
        sprintf(result, "#%d#%s", getpid(), string);
        printf("%s\n", result);
        write(pipe, result, strlen(string) + 1);
    }


    free(result);
    exit(0);
}

void startProduction(char* pipeName, char* inputFileName, int n)
{
    mkfifo(pipeName, 0666);
    int inputFile = openFile(inputFileName, O_RDONLY);
    int pipe = openFile(pipeName, O_WRONLY);

    for(int i = 0; i < 5; i++)
    {
        pid_t pid = fork();

        if(pid == 0)
        {
            produce(pipe, inputFile, n);
//            exit(0);
        }
        else if(pid < 0)
        {
            printf("ERROR fork");
            close(inputFile);
            close(pipe);
            return;
        }
    }
    int status = 0;
//    pid_t childPid;
    while ((wait(&status)) > 0);
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
    if(n == 0)
    {
        printf("N must be positive integer\n");
        return -2;
    }
    startProduction(argv[1], argv[2], n);
    return 0;
}