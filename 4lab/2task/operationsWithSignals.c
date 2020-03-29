#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


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


int main(int argc, char * argv[]){
    int arg = atoi(argv[1]);

    if(arg % 2 == 0)
        raise(SIGUSR1);
    if(arg % 3 == 0){
        checkIfSignalPending();
    }

    return 0;
}