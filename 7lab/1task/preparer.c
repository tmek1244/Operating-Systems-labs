#include "common.h"

#include <time.h>

char* activity = "Dodałem liczbę :";
struct info* memory;
int semaphores;
int order;

int get_random_number()
{
    return (int)(rand() / (RAND_MAX + 1.0) * 100.0);
}

void clear_memory()
{
    shmdt(memory);
    printf("[PREPARER] END\n");
}

int place_order()
{
    increase_sem(semaphores, NUMBER_OF_NOT_PACKED_ORDERS, 0);
    int amount = MAX_SIZE - get_sem_value(semaphores, SPACE_FOR_NEW_ORDERS);
    memory->buf[(memory->begin_order + amount) % MAX_SIZE] = order;
    decrease_sem(semaphores, SPACE_FOR_NEW_ORDERS, 0);
    return 0;
}

void place_order_in_array()
{
    order = get_random_number();
    sleep(1);
    if(get_sem_value(semaphores, SPACE_FOR_NEW_ORDERS) == 0)
        printf("[PREPARER] Nie ma miejsca na nowe zamowienie!\n");
    else
    {
        place_order();
        printf_info(getpid(), activity, order, semaphores);
    }

}

void get_access_to_array()
{

    decrease_sem(semaphores, ACCESS, 0);
    place_order_in_array();
    increase_sem(semaphores, ACCESS, 0);
}



int main(int argc, char* argv[])
{
    struct timeval seed;
    gettimeofday(&seed, NULL);
    srand(seed.tv_usec);

    memory = get_struct_info(ftok(PROJECT_PATH, ID), &semaphores);
    for(int i = 0; i < 10; i++)
    {
        get_access_to_array();
    }

    atexit(clear_memory);
    signal(SIGINT, exit);

    return 0;
}