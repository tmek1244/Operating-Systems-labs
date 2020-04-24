#include "common.h"

#include <time.h>

char* activity = "Dodałem liczbę :";


int get_random_number()
{
    return (int)(rand() / (RAND_MAX + 1.0) * 100.0);
}

int main(int argc, char* argv[])
{
    int i, seed;
    time_t tt;
    seed = time(&tt);
    srand(seed);
    key_t preparer_key = ftok(PROJECT_PATH, PREPARER_ID);
    int sem_id = semget(preparer_key, 0, 0);
    int memory_id = shmget(preparer_key, MAX_SIZE *  sizeof(int), 0666);
    int* buf = (int*) shmat(memory_id, NULL, 0);
    for(int i = 0; i < 20; i++)
    {
        int order = get_random_number();
//    decrease_sem(sem_id);
        buf[i % MAX_SIZE] = order;
        printf_info(getpid(), activity, order, 4, 7);
        sleep(1);
    }
    return 0;
}