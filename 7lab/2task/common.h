#ifndef SYSOPS_COMMON_H
#define SYSOPS_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>

#define ACCESS "/access"
#define NEW_ORDERS "/new_orders"
#define PACKED "/packed"
#define NOT_SENT "/not_sent"

#define MEMORY "/memory"

#define MAX_SIZE 15

sem_t* access_sem;
sem_t* space_for_new_orders;
sem_t* number_of_not_packed_orders;
sem_t* number_of_not_sent_orders;
int shared_memory;


struct info
{
    int begin_order;
    int buf[MAX_SIZE];
};


struct info* get_struct_info()
{
    shared_memory = shm_open(MEMORY, O_RDWR, 0666);

    return (struct info*) mmap(NULL, sizeof(struct info),
            PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
}


void open_semaphores()
{
    access_sem = sem_open(ACCESS, 0);
    space_for_new_orders = sem_open(NEW_ORDERS, 0);
    number_of_not_packed_orders = sem_open(PACKED, 0);
    number_of_not_sent_orders = sem_open(NOT_SENT, 0);
}

void close_semaphores()
{
    sem_close(access_sem);
    sem_close(space_for_new_orders);
    sem_close(number_of_not_packed_orders);
    sem_close(number_of_not_sent_orders);
}

void increase_sem(sem_t* sem)
{
    sem_post(sem);
}

void decrease_sem(sem_t* sem)
{
    sem_wait(sem);
}

int get_sem_value(sem_t* sem)
{
    int tmp;
    sem_getvalue(sem, &tmp);
    return tmp;
}

void printf_info(int pid, char* activity, int order)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    printf("PID: %d TIME: %lu : %lu. %s %d. Liczba zamówień do przygotowania: %d. Liczba zamówień do wysłania: %d \n"
            ,pid, current_time.tv_sec % 100, current_time.tv_usec,
            activity, order, get_sem_value(number_of_not_packed_orders), get_sem_value(number_of_not_sent_orders));
}


#endif //SYSOPS_COMMON_H
