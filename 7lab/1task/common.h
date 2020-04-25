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
#define ID 1

#define MAX_SIZE 15

enum type
{
    ACCESS = 0,
    SPACE_FOR_NEW_ORDERS = 1, // zmniejsza sie gdy dodajemy nowe zamowienie, zwieksza sie gdy wysylamy to zamowienie
    NUMBER_OF_NOT_PACKED_ORDERS = 2, // zmniejszamy gdy zapakujemy, dodajemy gdy zostanie dodane zamowienie
    NUMBER_OF_NOT_SENT_ORDERS = 3 // zmniejszamy gdy wyslemy, zwiekszamy gdy zapakujemy
};

struct info
{
    int begin_order;
    int buf[MAX_SIZE];
};


struct info* get_struct_info(int key, int* sem_id)
{
    (*sem_id) = semget(key, 0, 0);
    int tmp = shmget(key, 0, 0666);
    return (struct info*) shmat(tmp, NULL, 0);
}

void modify_sem(int sem_id, short int value, int type, short flag)
{
    struct sembuf sem_operations = {type, value, flag};

    if (semop(sem_id, &sem_operations, 1))
        printf("Could not update semaphore\n");
}

void increase_sem(int sem_id, int type, short flag)
{
    modify_sem(sem_id, 1, type, flag);
}

void decrease_sem(int sem_id, int type, short flag)
{
    modify_sem(sem_id, -1, type, flag);
}

int get_sem_value(int sem_id, int type)
{
    return semctl(sem_id, type, GETVAL);
}

void printf_info(int pid, char* activity, int order, int semaphores)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    printf("PID: %d TIME: %lu : %lu. %s %d. Liczba zamówień do przygotowania: %d. Liczba zamówień do wysłania: %d \n"
            ,pid, current_time.tv_sec % 100, current_time.tv_usec,
            activity, order, get_sem_value(semaphores, NUMBER_OF_NOT_PACKED_ORDERS),
            get_sem_value(semaphores, NUMBER_OF_NOT_SENT_ORDERS));
}


#endif //SYSOPS_COMMON_H
