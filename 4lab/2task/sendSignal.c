#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char * argv[]){
    raise(SIGUSR1);

    sigset_t pendingSignals;
    sigpending(&pendingSignals);

    if(sigismember(&pendingSignals, SIGUSR1))
    {
        printf("SIGUSR1 is pending at exec\n");
    } else
        printf("SIGUSR1 is not pending at exec\n");


    return 0;
}