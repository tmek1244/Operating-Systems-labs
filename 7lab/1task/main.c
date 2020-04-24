#include "common.h"


int sem_preparer_id, shared_memory_preparer_id;
int sem_packer_id, shared_memory_packer_id;


void clean_memory()
{
    semctl(sem_preparer_id, 0, IPC_RMID);
    shmctl(shared_memory_preparer_id, IPC_RMID, NULL);

    semctl(sem_packer_id, 0, IPC_RMID);
    shmctl(shared_memory_packer_id, IPC_RMID, NULL);
    printf("Cleaning memory...\n");
}

int main(int argc, char* argv[])
{
    key_t preparer_key = ftok(PROJECT_PATH, PREPARER_ID);
    sem_preparer_id = semget(preparer_key, 1, IPC_CREAT | 0666);
    shared_memory_preparer_id = shmget(preparer_key, MAX_SIZE, IPC_CREAT | 0666);

    key_t packer_key = ftok(PROJECT_PATH, PACKER_ID);
    sem_packer_id = semget(packer_key, 1, IPC_CREAT | 0666);
    shared_memory_packer_id = shmget(packer_key, MAX_SIZE, IPC_CREAT | 0666);




    atexit(clean_memory);

    semctl(sem_preparer_id, 0, SETVAL, 1);
    semctl(sem_packer_id, 0, SETVAL, 1);


    return 0;
}