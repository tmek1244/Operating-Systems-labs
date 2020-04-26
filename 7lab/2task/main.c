#include <sys/wait.h>
#include <time.h>
#include "common.h"


void clean_memory()
{

    close_semaphores();
    sem_unlink(ACCESS);
    sem_unlink(NEW_ORDERS);
    sem_unlink(PACKED);
    sem_unlink(NOT_SENT);
    shm_unlink(MEMORY);

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

    access_sem = sem_open(ACCESS, O_CREAT, 0666, 1);
    space_for_new_orders = sem_open(NEW_ORDERS, O_CREAT, 0666, MAX_SIZE);
    number_of_not_packed_orders = sem_open(PACKED, O_CREAT, 0666, 0);
    number_of_not_sent_orders = sem_open(NOT_SENT, O_CREAT, 0666, 0);
    shared_memory = shm_open(MEMORY, O_CREAT | O_RDWR, 0666);
    ftruncate(shared_memory, sizeof(struct info));

    atexit(clean_memory);
    signal(SIGINT, exit);

//
    struct info* info = (struct info*) mmap(NULL, sizeof(struct info),
            PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    info->begin_order = 0;

    summon_workers(preparers, "preparer");
    summon_workers(packers, "packer");
    summon_workers(senders, "sender");

    int pid = -1;
    while(wait(&pid) > 0);


    return 0;
}