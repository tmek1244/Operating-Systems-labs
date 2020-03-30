#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>


//char* convertToSI(int nr)
//{
//    switch (nr)
//    {
//        case SI_USER:
//            return "SI_USER";
//        case SI_KERNEL:
//            return "SI_KERNEL";
//        case SI_QUEUE:
//            return "SI_QUEUE";
//        case SI_TIMER:
//            return "SI_TIMER";
//        default:
//            return "unknown";
//    }
//}


void handler(int sig, siginfo_t *info, void *context) {
    printf("signal nr: %i\n", sig);
    printf("si_code: %i\n", info->si_code);
    printf("si_status: %i\n", info->si_status);
    printf("si_errno: %i\n", info->si_errno);
    printf("si_pid: %i\n", info->si_pid);
    printf("si_address: %i\n", info->si_addr);
    exit(0);
//    exit(0);
}

int main(int argc, char * argv[]){
    struct sigaction act;
    act.sa_sigaction = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_SIGINFO;

    if(argc == 1)
    {
        printf("wrong number of args\n");
        return -1;
    }
    if(strcmp(argv[1], "sigabrt") == 0)
    {
        sigaction(SIGABRT, &act, NULL);
        abort();
    }
    else if(strcmp(argv[1], "sigsegv") == 0)
    {
        sigaction(SIGSEGV, &act, NULL);
        char* str = "GfG";
        *(str+1) = 'n';
    }
    else if(strcmp(argv[1], "sigfpe") == 0)
    {
        sigaction(SIGFPE, &act, NULL);
        int a = 0;
        int b = 4/a;
    }
    else if(strcmp(argv[1], "sigchld") == 0)
    {
        sigaction(SIGCHLD, &act, NULL);
        pid_t childPID = fork();
        if(childPID == 0)
        {
            return 10;
        } else
        {
            wait(&childPID);
        }
    }
    else if(strcmp(argv[1], "sigquit") == 0)
    {
        sigaction(SIGQUIT, &act, NULL);
        exit(9);
    } else
    {
        printf("Wrong argument\n");
    }
    return 0;
}