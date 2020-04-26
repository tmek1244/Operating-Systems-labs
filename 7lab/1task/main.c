#include <sys/wait.h>
#include <time.h>

#include "common.h"

int semaphores, shared_memory;


void clean_memory()
{
    semctl(semaphores, 0, IPC_RMID);
    shmctl(shared_memory, IPC_RMID, NULL);

    printf("Cleaning memory...\n");
}

void summon_workers(int amount, char* type)
{
    for(int i = 0; i < amount; i++)
    {
        int pid = fork();
        if(pid < 0)
        {
            printf("ERROR - fork\n");
            exit(-2);
        } else if(pid == 0)
        {
            execl(type, type, NULL);
            exit(0);
        }
    }
}

int main(int argc, char* argv[])
{
    int preparers = 5, packers = 3, senders = 3;
    if(argc == 4)
    {
        preparers = atoi(argv[1]);
        packers = atoi(argv[2]);
        senders = atoi(argv[3]);
    }
    key_t key = ftok(PROJECT_PATH, ID);
    semaphores = semget(key, 4, IPC_CREAT | 0666);
    shared_memory = shmget(key, sizeof(struct info), IPC_CREAT | 0666);

    printf("%d\n", semctl(semaphores, ACCESS, SETVAL, 1));
    semctl(semaphores, SPACE_FOR_NEW_ORDERS, SETVAL, MAX_SIZE);
    semctl(semaphores, NUMBER_OF_NOT_PACKED_ORDERS, SETVAL, 0);
    semctl(semaphores, NUMBER_OF_NOT_SENT_ORDERS, SETVAL, 0);
    printf("[MAIN] %d\n", get_sem_value(semaphores, ACCESS));
    printf("[MAIN] %d\n", get_sem_value(semaphores, SPACE_FOR_NEW_ORDERS));
    printf("[MAIN] %d\n", get_sem_value(semaphores, NUMBER_OF_NOT_PACKED_ORDERS));
    printf("[MAIN] %d\n", get_sem_value(semaphores, NUMBER_OF_NOT_SENT_ORDERS));

    atexit(clean_memory);
    signal(SIGINT, exit);

//
    struct info* info = (struct info*) shmat(shared_memory, NULL, 0);
    info->begin_order = 0;
    printf("zaczynam...\n");

    summon_workers(preparers, "preparer");
    summon_workers(packers, "packer");
    summon_workers(senders, "sender");

    int pid = -1;
    while(wait(&pid) > 0);


    return 0;
}