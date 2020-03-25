#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

bool isWaiting = 0;

void sigintFunction(int signum){
    printf("Odebrano sygnal SIGINT\n");
    exit(EXIT_SUCCESS);
}

void sigtstpFunction(int signum){
    if (isWaiting){
        isWaiting = false;
    }
    else if (!isWaiting){
        printf("Oczekuje na CTRL+Z - kontynuacja albo CTR+C - zakończenie programu\n");
        isWaiting = true;
        sigset_t maskWithSignals;
        sigfillset(&maskWithSignals);
        sigdelset(&maskWithSignals, SIGTSTP);
        sigdelset(&maskWithSignals, SIGINT);
        sigsuspend(&maskWithSignals);
    }
}

void waitForSignal() {
    if (signal(SIGINT, sigintFunction) == SIG_ERR){
        printf("Error with signal sigint");
        exit(EXIT_FAILURE);
    }

    struct sigaction act;
    act.sa_handler = sigtstpFunction;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGTSTP, &act, NULL) != 0){
        printf("Error with signal sigtstp");
        exit(EXIT_FAILURE);
    }

    while(1) {
        system("ls .");
        sleep(1);
    }
}

int main(int argc, char * argv[]){
    waitForSignal();
}