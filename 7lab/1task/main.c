#include <sys/wait.h>
#include <time.h>

#include "common.h"

int semaphores, shared_memory;


void clean_memory()
{
    semctl(semaphores, 0, IPC_RMID);
    semctl(semaphores, 1, IPC_RMID);
    semctl(semaphores, 2, IPC_RMID);
    semctl(semaphores, 3, IPC_RMID);
    shmctl(shared_memory, IPC_RMID, NULL);

    printf("Cleaning memory...\n");
}

int main(int argc, char* argv[])
{
    int seed;
    time_t tt;
    seed = time(&tt);
    srand(seed);
    key_t key = ftok(PROJECT_PATH, ID);
    semaphores = semget(key, 4, IPC_CREAT | 0666);
    shared_memory = shmget(key, sizeof(struct info), IPC_CREAT | 0666);

//    union semun
//    {
//        int val;
//        struct semid_ds *buf;
//        ushort array [1];
//    } sem_attr;
//    sem_attr.val = 1;
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


    for(int i = 0; i < 5; i++)
    {
        int pid = fork();
        if(pid < 0)
        {
            printf("ERROR - fork\n");
            return 0;
        } else if(pid == 0)
        {
            execl("preparer", "preparer", NULL);
            return 0;
        }

    }
    for(int i = 0; i < 3; i++)
    {
        int pid = fork();
        if(pid < 0)
        {
            printf("ERROR - fork\n");
            return 0;
        } else if(pid == 0)
        {
            execl("packer", "packer", NULL);
            return 0;
        }

    }
    for(int i = 0; i < 1; i++)
    {
        int pid = fork();
        if(pid < 0)
        {
            printf("ERROR - fork\n");
            return 0;
        } else if(pid == 0)
        {
            execl("sender", "sender", NULL);
            return 0;
        }

    }
    int pid = -1;
    while(wait(&pid));


    return 0;
}