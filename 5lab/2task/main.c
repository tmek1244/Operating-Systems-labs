#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

    FILE* sort = popen("sort", "w");
    while (getline(&line, &len, file) != -1) {
        fputs(line, sort);
    }
    fclose(sort);

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