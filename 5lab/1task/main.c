#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


char** splitLine(char* line, char* delimiter, int* size)
{
    int numberOfCommands = 1;
    for(int i = 0; i < strlen(line); i++)
    {
        if(line[i] == delimiter[0])
            numberOfCommands++;
    }
    char** commands = calloc(numberOfCommands, sizeof(char*));
    char* commandsLine = malloc(100 * sizeof(char));
    commandsLine = strcpy(commandsLine, line);
    (*size) = numberOfCommands;
    int commandsCount = 0;
    char * token = strtok(commandsLine, delimiter);
    while( token != NULL ) {
        commands[commandsCount++] = token;
        token = strtok(NULL, delimiter);
    }
    return commands;
}

int executeLinePipe(char* line)
{
    int numberOfCommands = 0;
    char delimiters[2] = {'|', '\n'};
    char** commands = splitLine(line, delimiters, &numberOfCommands);
    int pipes[2][2];

    for(int i = 0; i < numberOfCommands; i++)
    {
        if (pipe(pipes[i % 2]) < 0){
            printf("pipe error");
            return -1;
        }
        pid_t pid = vfork();
        if(pid == 0)
        {
            int numberOfArgs = 0;
            char** args = splitLine(commands[i], " ", &numberOfArgs);
            if (i != 0) {
                close(pipes[(i+1) % 2][1]);
                if (dup2(pipes[(i+1) % 2][0], STDIN_FILENO) < 0){
                    printf("error dup2");
                    exit(-3);
                }
            }
            if (i != numberOfCommands - 1){
                close(pipes[i % 2][0]);
                if (dup2(pipes[i % 2][1], STDOUT_FILENO) < 0){
                    printf("error dup2");
                    exit(-3);
                }
            }

            if (execvp(args[0], args) < 0){
                printf("execvp error");
                exit(-2);
            }
            exit(-4);
        }
        else if(pid < 0)
        {
            printf("fork error");
            return -1;
        }

    }
    close(pipes[numberOfCommands % 2][0]);
    close(pipes[numberOfCommands % 2][1]);
    int status = 0;
    pid_t childPid;
    while ((childPid = wait(&status)) > 0);

    return 0;
}

int run(char* fileName)
{
    FILE* file = fopen(fileName, "r");
    if(file == NULL)
    {
        printf("Cannot open file");
        return -1;
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        printf("%s", line);
        executeLinePipe(line);
    }

    fclose(file);
    if(line)
        free(line);
    return 0;
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Wrong number of arguments");
        return -1;
    }
    run(argv[1]);

    return 0;
}