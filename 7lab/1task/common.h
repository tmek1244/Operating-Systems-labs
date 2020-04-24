#ifndef SYSOPS_COMMON_H
#define SYSOPS_COMMON_H

#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>

#define PROJECT_PATH "/home/tmek"
#define PREPARER_ID 1
#define PACKER_ID 2

#define MAX_SIZE 15

void modify_sem(int sem_id, short int value)
{
    struct sembuf sem_operations = {0, value, 0};

    if (semop(sem_id, &sem_operations, 1))
        printf("Could not update semaphore\n");
}

void increase_sem(int sem_id)
{
    modify_sem(sem_id, 1);
}

void decrease_sem(int sem_id)
{
    modify_sem(sem_id, -1);
}

void printf_info(int pid, char* activity, int current_order, int order_to_prepare, int order_to_send)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    printf("PID: %d TIME: %lu : %lu. %s %d. Liczba zamówień do przygotowania: %d. Liczba zamówień do wysłania: %d \n"
            ,pid, current_time.tv_sec % 100, current_time.tv_usec,
            activity, current_order, order_to_prepare, order_to_send);
}


#endif //SYSOPS_COMMON_H
