#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

int chair_number, client_number;
int is_barber_sleeping = 0;
pthread_t actually_on_chair;
int number_of_free_chairs;
int next_free_chair = 0;
int next_chair = 0;
int amount_of_done_clients;

pthread_t* chairs;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void* barber()
{
    while(amount_of_done_clients < client_number) {
        pthread_mutex_lock(&mutex);
        if(number_of_free_chairs == chair_number) {
            printf("Golibroda: Ide spac\n");
            is_barber_sleeping = 1;
            pthread_cond_wait(&cond, &mutex);
            is_barber_sleeping = 0;
        } else {
            number_of_free_chairs++;
            actually_on_chair = chairs[next_chair];
            next_chair = (next_chair + 1) % chair_number;
        }
        printf("Golibroda: Czeka %d klientow, gole klienta %lu\n",
                chair_number - number_of_free_chairs, actually_on_chair);

        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        sleep(random() % 5 + 2);

        pthread_mutex_lock(&mutex);
        pthread_cancel(actually_on_chair);
        amount_of_done_clients++;
        actually_on_chair = 0;
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* client()
{
    pthread_t id = pthread_self();
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(is_barber_sleeping)
        {
            actually_on_chair = id;
            pthread_cond_broadcast(&cond);
            printf("Klient: Budze golibrode; ID: %ld\n", id);
            pthread_mutex_unlock(&mutex);
            break;
        } else if(number_of_free_chairs > 0)
        {
            number_of_free_chairs--;
            chairs[next_free_chair] = id;
            next_free_chair = (next_free_chair + 1) % chair_number;
            printf("Klient: Poczekalnia, wolne miejsca %d; ID: %ld\n", number_of_free_chairs, id);
            pthread_mutex_unlock(&mutex);
            break;
        }
        printf("Klient: Zajete; %ld\n", id);
        pthread_mutex_unlock(&mutex);
        sleep(random() % 5);
    }

    return NULL;
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    if(argc < 3)
    {
        printf("Zla liczba argumentow! Nalezy podac liczbe krzesel oraz liczbe klientow!\n");
        return -1;
    }

    chair_number = atoi(argv[1]);
    client_number = atoi(argv[2]);
    if(pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("Error with mutex init!\n");
        return -2;
    }
    number_of_free_chairs = chair_number;
    chairs = calloc(chair_number, sizeof(pthread_t));
    pthread_t* thread_ids = calloc(client_number + 1, sizeof(pthread_t));
    pthread_create(&thread_ids[0], NULL, barber, NULL);

    for(int i = 0; i < chair_number; i++)
        chairs[i] = 0;
    for(int i = 1; i <= client_number; i++)
    {
        sleep(random() % 5);
        pthread_create(&thread_ids[i], NULL, client, NULL);
    }

    for(int i = 0; i < client_number + 1; i++) {
        if(pthread_join(thread_ids[i], NULL) > 0)
        {
            printf("Error with pthread join!\n");
        }
    }

    free(thread_ids);
    free(chairs);
    return 0;
}