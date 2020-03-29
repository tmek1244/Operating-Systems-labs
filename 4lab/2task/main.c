#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>


void printMessage(int sigNumber)
{
    printf("   Signal received: %i\n", sigNumber);
}

void ignore()
{
    if(signal(SIGUSR1, SIG_IGN) == SIG_ERR)
    {
        printf("Error with SIG_IGN");
        return;
    }
}

void handler()
{
    if(signal(SIGUSR1, printMessage) == SIG_ERR)
    {
        printf("Error with SIG_IGN");
        return;
    }
}

void mask()
{
    sigset_t newMask;
    sigset_t oldMask;

    sigemptyset(&newMask);
    sigaddset(&newMask, SIGUSR1);

    if(sigprocmask(SIG_BLOCK, &newMask, &oldMask) < 0)
    {
        printf("Error, cannot block signal");
        return;
    }
}

void checkIfSignalPending()
{
    sigset_t pendingSignals;
    sigpending(&pendingSignals);

    if(sigismember(&pendingSignals, SIGUSR1))
    {
        printf("   SIGUSR1 is pending\n");
    } else
        printf("   SIGUSR1 is not pending\n");
}

void forkAndExec(char* argS)
{
    int arg = atoi(argS);
    printf("MAIN:\n");
    raise(SIGUSR1);
    if(arg % 3 == 0)
        checkIfSignalPending();
    pid_t child;
    if ((child = fork()) == -1)
    {
        perror("Error during fork\n");
        exit(1);
    }
    else if (child == 0)
    {
        printf("FORK:\n");
        if(arg % 2 == 0)
            raise(SIGUSR1);
        if(arg % 3 == 0)
            checkIfSignalPending();
        exit(EXIT_SUCCESS);
    } else{
        usleep(300);
        printf("EXEC:\n");
        execl("./operationsWithSignals", "operationWithSignals", argS, NULL);
    }
}

void ignoreProcedure()
{
    ignore();
    forkAndExec("2");
}

void handlerProcedure()
{
    handler();
    forkAndExec("2");
}

void maskProcedure()
{
    mask();
    forkAndExec("6");
}

void pendingProcedure()
{
    mask();
    forkAndExec("3");
}

void checkInheritanceFork(char* option)
{
    if(strcmp(option, "ignore") == 0)
        ignoreProcedure();
    else if(strcmp(option, "handler") == 0)
        handlerProcedure();
    else if(strcmp(option, "mask") == 0)
        maskProcedure();
    else if(strcmp(option, "pending") == 0)
        pendingProcedure();
    else
    {
        printf("Wrong option\n");
        return;
    }
}

int main(int argc, char * argv[]){
    if(argc != 2)
    {
        printf("Wrong number of arguments!\n");
        return -1;
    }
    checkInheritanceFork(argv[1]);
    return 0;
}