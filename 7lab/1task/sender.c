#include "common.h"

#include <time.h>

char* activity = "Wysłałem zamówienie o wielkości ";
struct info* memory;
int semaphores;

//int get_order()
//{
//    decrease_sem(packer_id);
//    int order = take_order(memory_packer);
//    increase_sem(packer_id);
//    return order;
//}

void send_order()
{
    sleep(1);
    if(get_sem_value(semaphores, NUMBER_OF_NOT_SENT_ORDERS) == 0)
    {
        printf("There is no packed order!\n");
    } else
    {
        int order = memory->buf[memory->begin_order];
        memory->begin_order = (memory->begin_order + 1) % MAX_SIZE;
        increase_sem(semaphores, SPACE_FOR_NEW_ORDERS, 0);
        decrease_sem(semaphores, NUMBER_OF_NOT_SENT_ORDERS, 0);
        sleep(1);
        order *= 3;

        printf_info(getpid(), activity, order, semaphores);
    }
}


void get_access_to_array()
{
    decrease_sem(semaphores, ACCESS, 0);
    send_order();
    increase_sem(semaphores, ACCESS, 0);
}



int main(int argc, char* argv[])
{
    memory = get_struct_info(ftok(PROJECT_PATH, ID), &semaphores);

    for(int i = 0; i < 10; i++)
    {
        get_access_to_array();
    }

    shmdt(memory);
    return 0;
}