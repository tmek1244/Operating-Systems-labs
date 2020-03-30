#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

pid_t pid;
bool wait = true;

int received = 0;


void onCommunicationSignal(int sig) { received++; }
void onBreakSignal(int sig) { wait = false; }

union sigval value = {.sival_ptr = NULL};


int main(int argc, char **argv)
{
    int communicationSignal = SIGUSR1;
    int signalToBreak = SIGUSR2;
    if(argc != 4 && argc != 5)
    {
        printf("Wrong number of arguments");
        return -1;
    }

    bool check = argc == 5 ? true : false;

    if(strcmp(argv[3], "sigrt") == 0)
    {
        signalToBreak = SIGTTIN;
        communicationSignal = SIGTTOU;
    }

    signal(signalToBreak, onBreakSignal);

    pid = atoi(argv[1]);
    int numberOfSignalsToSend = atoi(argv[2]);
    sigset_t sigset;

    signal(SIGUSR1, onCommunicationSignal);
    signal(communicationSignal, onCommunicationSignal);
    if (strcmp(argv[3], "sigrt") == 0 || strcmp(argv[3], "kill") == 0)
    {
        for (int i = 0; i < numberOfSignalsToSend; i++)
        {
            sigemptyset(&sigset);
            kill(pid, communicationSignal);
            if(check) {
                sigsuspend(&sigset);
            }
        }
        kill(pid, signalToBreak);
    }
    else if (strcmp(argv[3], "sigqueue") == 0)
    {
        for (int i = 0; i < numberOfSignalsToSend; i++) {
            sigqueue(pid, communicationSignal, value);
            if(check) {
                sigemptyset(&sigset);
                sigsuspend(&sigset);
            }
        }
        sigqueue(pid, signalToBreak, value);
    }
    received = 0;
    wait = true;
    signal(communicationSignal, onCommunicationSignal);
    while (wait)
        ;

    printf("SENDER send %i signals and received: %d\n", numberOfSignalsToSend, received);
}