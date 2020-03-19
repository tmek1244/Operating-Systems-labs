#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ftw.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


void findStat(char* path)
{
    DIR* directory = opendir(path);

    if(directory == NULL) {
        printf("Cannot open directory!");
        return;
    }

    pid_t pid = fork();
    if(pid < 0)
    {
        printf("fork() error");
        return;
    }

    if(pid == 0)
    {
        struct dirent *file;
        char *absPath = (char *) calloc(100, sizeof(char));
        char *nextPath = (char *) calloc(100, sizeof(char));
        struct stat stats;
        printf("------------------------------------------------------------\n");
        printf("Path: %s\n", path);
        printf("Pid: %i\n", pid);
        char command[80] = "ls -l ";
        strcat(command, path);
        system(command);
        printf("------------------------------------------------------------\n");

        while ((file = readdir(directory)))
        {
            strcpy(nextPath, path);
            strcat(nextPath, "/");
            strcat(nextPath, file->d_name);
            realpath(nextPath, absPath);

            stat(nextPath, &stats);

            if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0 && file->d_type == 4)
                findStat(nextPath);
        }
        free(nextPath);
        free(absPath);
        free(file);
        exit(0);
    } else
        wait(&pid);

    closedir(directory);
}

int main(int argc, char* argv[])
{

    if(argc != 2)
        findStat(".");
    else
        findStat(argv[1]);

    return 0;
}