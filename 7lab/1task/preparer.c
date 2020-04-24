#include "common.h"

#include <time.h>

char* activity = "Dodałem liczbę :";


int get_random_number()
{
    int i, seed;
    time_t tt;
    seed = time(&tt);
    srand(seed);

    return (int)(rand() / (RAND_MAX + 1.0) * 100.0);
}

int main(int argc, char* argv[])
{
    key_t preparer_key = ftok(PROJECT_PATH, PREPARER_ID);
    int sem_id = semget(preparer_key, 0, 0);

    int order = get_random_number();
    decrease_sem(sem_id, PREPARER);
    printf_info(getpid(), activity, order, 4, 7);
    return 0;
}