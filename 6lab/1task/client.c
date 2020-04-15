#include "common.h"

int server_qid;
int client_qid;
int id;

void send_stop()
{
    struct message message = {Stop, client_qid, id};
    msgsnd(server_qid, &message, sizeof message, 0);
}

void close_client()
{
    send_stop();
    msgctl(client_qid, IPC_RMID, NULL);
}


int main()
{
    key_t server_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);
    if(server_key == -1)
    {
        printf("Error with key - server\n");
        return -1;
    }

    if((server_qid = msgget(server_key, 0666)) == -1)
    {
        printf("Error with server_qid - client\n");
        return -2;
    }

    if((client_qid = msgget(IPC_PRIVATE, 0666)) == -1)
    {
        printf("Error with client_qid\n");
        return -2;
    }
    atexit(close_client);
    signal(SIGINT, exit);
    struct message message = {Init, 0, client_qid};
    msgsnd(server_qid, &message, sizeof message, 0);
    struct message id_message;
    msgrcv(client_qid, &id_message, sizeof id_message, -100, 0);
    if(id_message.mtype == Init && id_message.data == -1)
    {
        printf("Server is full!\n");
        return 0;
    }
    id = id_message.data;
    printf("received id: %d", id_message.data);



    return 0;
}