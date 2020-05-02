#include "common.h"


char* activity = "Przygotowałem zamówienie o wielkości ";
struct info* memory;
int semaphores;

void clear_memory()
{
    shmdt(memory);
    printf("[PACKER] END\n");
}

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
        printf("[PACKER] Nie ma zamówienia do zapakowania!\n");
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
    memory = get_struct_info(ftok(PROJECT_PATH, ID), &semaphores);
    for(int i = 0; i < 10; i++)
    {
        get_access_to_array();
    }
    atexit(clear_memory);
    signal(SIGINT, exit);

    return 0;
}