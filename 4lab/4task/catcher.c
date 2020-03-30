#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

pid_t pid;
bool wait = true;
bool check = false;

int received = 0;

void onCommunicationSignal(int sig, siginfo_t* info, void* context) {
    received++;

    if(check){
        kill(info->si_pid, SIGUSR1);
    }
}
void onBreakSignal(int sig, siginfo_t* info, void* context)
{
    printf("CATCHER received: %d\n", received);
    pid = info->si_pid;
    wait = false;
}

union sigval value = {.sival_ptr = NULL};


int main(int argc, char **argv)
{
    int communicationSignal = SIGUSR1;
    int signalToBreak = SIGUSR2;

    if(argc != 2 && argc != 3)
    {
        printf("Wrong number of arguments");
        return -1;
    }
    check = (argc == 3 ? true : false);
    if(strcmp(argv[1], "sigrt") == 0)
    {
        signalToBreak = SIGTTIN;
        communicationSignal = SIGTTOU;

    }

    printf("PID: %d\n", getpid());
    struct sigaction actForBreak;
    actForBreak.sa_flags = SA_SIGINFO;
    actForBreak.sa_sigaction = onBreakSignal;
    sigaction(signalToBreak, &actForBreak, NULL);

    struct sigaction actForCommunication;
    actForCommunication.sa_flags = SA_SIGINFO;
    actForCommunication.sa_sigaction = onCommunicationSignal;
    sigaction(communicationSignal, &actForCommunication, NULL);

    while (wait)
        ;


    if (strcmp(argv[1], "sigrt") == 0 || strcmp(argv[1], "kill") == 0)
    {
        for (int i = 0; i < received; i++)
            kill(pid, communicationSignal);
        kill(pid, signalToBreak);
    }
    else if (strcmp(argv[1], "sigqueue") == 0)
    {
        for (int i = 0; i < received; i++)
            sigqueue(pid, communicationSignal, value);
        sigqueue(pid, signalToBreak, value);
    }

    return 0;
}