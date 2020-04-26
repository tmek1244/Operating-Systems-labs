#include "common.h"


char* activity = "Wysłałem zamówienie o wielkości ";
struct info* memory;


void clear_memory()
{
    close_semaphores();
    munmap(memory, sizeof(struct info));
    printf("[SENDER] END\n");
}

void send_order()
{
    sleep(1);
    if(get_sem_value(number_of_not_sent_orders) == 0)
    {
        printf("[SENDER] Nie ma zamówienia zapakowanego do wysłania!\n");
    } else
    {
        int order = memory->buf[memory->begin_order];
        memory->begin_order = (memory->begin_order + 1) % MAX_SIZE;
        increase_sem(space_for_new_orders);
        decrease_sem(number_of_not_sent_orders);
        sleep(1);
        order *= 3;

        printf_info(getpid(), activity, order);
    }
}


void get_access_to_array()
{
    decrease_sem(access_sem);
    send_order();
    increase_sem(access_sem);
    sleep(1);
}



int main(int argc, char* argv[])
{
    open_semaphores();
    memory = get_struct_info();
    for(int i = 0; i < 10; i++)
    {
        get_access_to_array();
    }

    atexit(clear_memory);
    signal(SIGINT, exit);


    return 0;
}