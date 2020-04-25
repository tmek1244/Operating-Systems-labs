#include "common.h"

#include <time.h>

char* activity = "Przygotowałem zamówienie o wielkości ";
struct info* memory;
int semaphores;

int get_order()
{
    decrease_sem(semaphores, NUMBER_OF_NOT_PACKED_ORDERS, 0);
    int* order = &memory->buf[(memory->begin_order +
                              get_sem_value(semaphores, NUMBER_OF_NOT_SENT_ORDERS)) % MAX_SIZE];
    (*order) *= 2;
    increase_sem(semaphores, NUMBER_OF_NOT_SENT_ORDERS, 0);
    return *order;
}

void take_and_pack_order()
{
    sleep(1);
    if(get_sem_value(semaphores, NUMBER_OF_NOT_PACKED_ORDERS) == 0)
    {
        printf("There is no order to take!\n");
    } else{
        int order = get_order();
        printf_info(getpid(), activity, order, semaphores);

    }
}

void get_access_to_array()
{
    decrease_sem(semaphores, ACCESS, 0);
    take_and_pack_order();
    increase_sem(semaphores, ACCESS, 0);
}


int main(int argc, char* argv[])
{
//    key_t preparer_key = ftok(PROJECT_PATH, PREPARER_ID);
//    preparer_id = semget(preparer_key, 0, 0);
//    int memory_id = shmget(preparer_key, sizeof(struct info), 0666);
//    memory_preparer = (struct info*) shmat(memory_id, NULL, 0);
//    memory_preparer = get_struct_info(ftok(PROJECT_PATH, PREPARER_ID), &preparer_id);
//    memory_packer = get_struct_info(ftok(PROJECT_PATH, PACKER_ID), &packer_id);
    memory = get_struct_info(ftok(PROJECT_PATH, ID), &semaphores);
    for(int i = 0; i < 10; i++)
    {
        get_access_to_array();
    }

//    shmdt(memory_preparer);
    shmdt(memory);
    return 0;
}