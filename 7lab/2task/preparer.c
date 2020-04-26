#include "common.h"

#include <time.h>

char* activity = "Dodałem liczbę :";
struct info* memory;
int order;


void clear_memory()
{
    close_semaphores();
    munmap(memory, sizeof(struct info));
    printf("[PREPARER] END\n");

}

int get_random_number()
{
    return (int)(rand() / (RAND_MAX + 1.0) * 10.0);
}

int place_order(int order)
{
    increase_sem(number_of_not_packed_orders);
    int amount = MAX_SIZE - get_sem_value(space_for_new_orders);
    memory->buf[(memory->begin_order + amount) % MAX_SIZE] = order;
    decrease_sem(space_for_new_orders);
    return 0;
}

void place_order_in_array()
{
    order = get_random_number();
    sleep(1);
    if(get_sem_value(space_for_new_orders) == 0)
        printf("[PREPARER] Nie ma miejsca na nowe zamowienie!\n");
    else
    {
        place_order(order);
        printf_info(getpid(), activity, order);
    }

}

void get_access_to_array()
{
    decrease_sem(access_sem);
    place_order_in_array();
    increase_sem(access_sem);
    sleep(1);
}



int main(int argc, char* argv[])
{
    struct timeval seed;
    gettimeofday(&seed, NULL);
    srand(seed.tv_usec);

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