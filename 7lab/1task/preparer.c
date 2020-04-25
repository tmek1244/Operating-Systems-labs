#include "common.h"


char* activity = "Dodałem liczbę :";
struct info* memory;
int semaphores;
int order;

int get_random_number()
{
    return (int)(rand() / (RAND_MAX + 1.0) * 10.0);
}

int place_order(int order)
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
    printf("random: %d\n", order);
    sleep(1);
    if(get_sem_value(semaphores, SPACE_FOR_NEW_ORDERS) == 0)
        printf("[PREPARER] Nie ma miejsca na nowe zamowienie!\n");
    else
    {
        place_order(order);
//            return;

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

//    key_t preparer_key = ftok(PROJECT_PATH, PREPARER_ID);
//    int sem_id = semget(preparer_key, 0, 0);
//    int memory_id = shmget(preparer_key, MAX_SIZE *  sizeof(int), 0666);
//    memory = (struct info*) shmat(memory_id, NULL, 0);
//    int key = ftok(PROJECT_PATH, ID);
//    semaphores = semget(key, 0, 0);
//    int tmp = shmget(key, 0, 0666);
//    memory = (struct info*) shmat(tmp, NULL, 0);
//
    memory = get_struct_info(ftok(PROJECT_PATH, ID), &semaphores);
    for(int i = 0; i < 10; i++)
    {
        get_access_to_array();
    }
    shmdt(memory);
    return 0;
}