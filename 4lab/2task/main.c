#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>


void handler(int sigNumber)
{
    printf("Signal received %i\n", sigNumber);
}

void checkInheritanceFork(char* option)
{
    struct sigaction act;
    if(strcmp(option, "ignore") == 0)
    {
        if(signal(SIGUSR1, SIG_IGN) == SIG_ERR)
        {
            printf("Error with SIG_IGN");
            return;
        }
    }
    else if(strcmp(option, "handler") == 0)
    {
        if(signal(SIGUSR1, handler) == SIG_ERR)
        {
            printf("Error with SIG_IGN");
            return;
        }
    }
    else if(strcmp(option, "mask") == 0)
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

        sigset_t pendingSignals;
        sigpending(&pendingSignals);

        if(sigismember(&pendingSignals, SIGUSR1))
        {
            printf("SIGUSR1 is pending\n");
        } else
            printf("SIGUSR1 is not pending\n");
    }
    else
    {
        printf("Wrong option");
        return;
    }


    raise(SIGUSR1);
    sigset_t pendingSignals;
    sigpending(&pendingSignals);

    if(sigismember(&pendingSignals, SIGUSR1))
    {
        printf("SIGUSR1 is pending\n");
    } else
        printf("SIGUSR1 is not pending\n");


    pid_t child;
    if ((child = fork()) == -1)
    {
        perror("Nie powołano procesu potomnego");
        exit(1);
    }
    else if (child == 0)
    {
        raise(SIGUSR1);
        if(sigismember(&pendingSignals, SIGUSR1))
        {
            printf("SIGUSR1 is pending even at child\n");
        } else
            printf("SIGUSR1 is not pending at child\n");
        exit(EXIT_SUCCESS);
    } else
    {
        execl("./sendSignal", NULL);
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