#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>


void run()
{
    mkfifo("pipe", 0666);

    char inputFile[10] = "data/1.txt";
    pid_t consumerPID = fork();
    if(consumerPID == 0)
    {
        execl("consumer", "consumer", "pipe", "out.txt", "10", NULL);
        exit(0);
    }

    char c = '1';
    while(c != '6')
    {
        pid_t producerPID = fork();
        if(producerPID == 0)
        {
            inputFile[5] = c;
            execl("producer", "producer", "pipe", inputFile, "10", NULL);
            exit(0);
        }
        c += 1;
    }
    int status = 0;
    pid_t childPid;
    while ((childPid = wait(&status)) > 0);
}


int main(int argc, char** argv)
{
    run();
    return 0;
}