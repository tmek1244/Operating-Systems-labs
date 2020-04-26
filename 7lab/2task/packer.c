#include "common.h"


char* activity = "Przygotowałem zamówienie o wielkości ";
struct info* memory;

void clear_memory()
{
    close_semaphores();
    munmap(memory, sizeof(struct info));
    printf("[PACKER] END\n");

}

int get_order()
{
    decrease_sem(number_of_not_packed_orders);
    int* order = &memory->buf[(memory->begin_order +
                              get_sem_value(number_of_not_sent_orders)) % MAX_SIZE];
    (*order) *= 2;
    increase_sem(number_of_not_sent_orders);
    return *order;
}

void take_and_pack_order()
{
    sleep(1);
    if(get_sem_value(number_of_not_packed_orders) == 0)
    {
        printf("[PACKER] Nie ma zamówienia do zapakowania!\n");
    } else{
        int order = get_order();
        printf_info(getpid(), activity, order);

    }
}

void get_access_to_array()
{
    decrease_sem(access_sem);
    take_and_pack_order();
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