#include <signal.h>
#include "common.h"


int available_ID[MAX_CLIENT_COUNT+1];
int last_element = -1;
int pop()
{
    int result = available_ID[last_element];
    last_element--;
    return result;
}

void push(int new_value)
{
    if(last_element > MAX_CLIENT_COUNT)
    {
        printf("Stack overflow\n");
        return;
    }
    last_element++;
    available_ID[last_element] = new_value;
}

int server_qid;
struct client
{
    int qid;
    struct client* peer;
}clients[MAX_CLIENT_COUNT];
typedef struct client client;

void close_server()
{
    printf("Server will be close...\n");
    struct message message = {Stop, -1};
    for(int i = 0; i < MAX_CLIENT_COUNT; i++)
    {
        if(clients[i].qid != 0)
        {
            msgsnd(clients[i].qid, &message, sizeof message, 0);
        }
    }
    msgctl(server_qid, IPC_RMID, NULL);
}

void send_list(int client_id)
{
    struct message message = { List };
    int size = 0;

    for (int i = 0; i < MAX_CLIENT_COUNT; i++)
        if (clients[i].qid != 0 && i != client_id) {
            message.List.clients[size].id = i;
            message.List.clients[size].available = clients[i].peer == NULL;
            size++;
        }

    message.List.size = size;
    msgsnd(clients[client_id].qid, &message, sizeof message, 0);
}

void add_client(int client_qid)
{
    int new_id = pop();
    struct message id_message = {Id, -1, new_id};
    if(new_id == -1)
    {
        push(-1);
    } else
    {
        client* new_client = &clients[new_id];
        new_client->qid = client_qid;
        new_client->peer = NULL;
        printf("add_client with id: %d\n", new_id);
    }
    msgsnd(client_qid, &id_message, sizeof id_message, 0);

}

void connect_client(int client_id, int other_client_id)
{
    struct message message = {Connect, -1, clients[client_id].qid};
    if(clients[other_client_id].peer == NULL)
    {
        msgsnd(clients[other_client_id].qid, &message, sizeof message, 0);
        message.data = clients[other_client_id].qid;
        clients[other_client_id].peer = &clients[client_id];
        clients[client_id].peer = &clients[other_client_id];
        return;
    }
    message.data = -1;
    msgsnd(clients[client_id].qid, &message, sizeof message, 0);
}

void disconnect_client(int client_id)
{
    clients[client_id].peer = NULL;
}

void remove_client(int client_id)
{
    clients[client_id].qid = 0;
    clients[client_id].peer = NULL;
    push(client_id);
}

int main()
{
    printf("Starting server...\n");
    key_t server_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);
    if(server_key == -1)
    {
        printf("Error with key - server\n");
        return -1;
    }

    if((server_qid = msgget(server_key, IPC_CREAT | IPC_EXCL | 0666)) == -1)
    {
        printf("Error with server_qid\n");
        return -2;
    }
    atexit(close_server);
    signal(SIGINT, exit);
    signal(SIGTERM, exit);
    signal(SIGSEGV, exit);
    for(int i = -1; i < MAX_CLIENT_COUNT; i++)
        push(i);

    struct message msg;
    bool running = true;

    while(running)
    {
        msgrcv(server_qid, &msg, sizeof msg, -100, 0);
        printf("[SERVER] message received %ld\n", msg.mtype);
        switch(msg.mtype)
        {
            case Init:
                add_client(msg.data);
                break;
            case List:
                send_list(msg.id);
                break;
            case Connect:
                connect_client(msg.id, msg.data);
                break;
            case Disconnect:
                disconnect_client(msg.id);
                break;
            case Stop:
                remove_client(msg.id);
                break;
        }
        for(int i = 0; i <= last_element; i++)
            printf("%d ", available_ID[i]);
    }
}