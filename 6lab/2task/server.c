#include <signal.h>
#include <mqueue.h>
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

bool is_on_stack(int id)
{
    for(int i = 0; i < last_element; i++)
        if(available_ID[i] == id)
            return true;
    return false;
}

int server_descriptor;
struct client
{
    int descriptor;
    struct client* peer;
}clients[MAX_CLIENT_COUNT];
typedef struct client client;

void close_server()
{
    printf("Server will be close...\n");
    char* message = "stop";
    for(int i = 0; i < MAX_CLIENT_COUNT; i++)
    {
        if(clients[i].descriptor != 0)
        {
            mq_send(clients[i].descriptor, message, strlen(message), 0);
            mq_close(clients[i].descriptor);
        }
    }
    mq_close(server_descriptor);
    mq_unlink(SERVER_QUEUE_NAME);
}

//#TODO
void send_list(int client_id)
{
//    struct message message = { List };
//    int size = 0;
//
//    for (int i = 0; i < MAX_CLIENT_COUNT; i++)
//        if (clients[i].descriptor != 0 && i != client_id) {
//            message.List.clients[size].id = i;
//            message.List.clients[size].available = clients[i].peer == NULL;
//            size++;
//        }
    printf("TODO\n");
//    message.List.size = size;
//    mq_send(clients[client_id].descriptor, message, sizeof message, 0);
}

void add_client(int client_qid)
{
    int new_id = pop();
//    struct message id_message = {Init, -1, new_id};
    char message[12];
    sprintf(message, "%d", new_id);
    if(new_id == -1)
    {
        push(-1);
    } else
    {
        clients[new_id].descriptor = client_qid;
        clients[new_id].peer = NULL;
        printf("[SERVER] add_client with id: %d\n", new_id);
    }
    mq_send(client_qid, message, sizeof message, 0);

}

void connect_client(int client_id, int other_client_id)
{
    char message[MAX_SIZE];
    sprintf(message, "-1");
    if(is_on_stack(other_client_id) || other_client_id >= MAX_CLIENT_COUNT)
    {
        printf("[SERVER] There is no client with this id\n");
//        struct message message = {Connect, -1, -1};
        mq_send(clients[client_id].descriptor, message, strlen(message), 0);
        return;
    }

    if(clients[other_client_id].peer == NULL)
    {
        sprintf(message, "%d", clients[client_id].descriptor);
        mq_send(clients[other_client_id].descriptor, message, sizeof message, 0);
        sprintf(message, "%d", clients[other_client_id].descriptor);
        clients[other_client_id].peer = &clients[client_id];
        clients[client_id].peer = &clients[other_client_id];
    }

    mq_send(clients[client_id].descriptor, message, sizeof message, 0);
}

void disconnect_client(int client_id)
{
    clients[client_id].peer = NULL;
}

void remove_client(int client_id)
{
    printf("[SERVER] remove client with id %d\n", client_id);
    clients[client_id].descriptor = 0;
    clients[client_id].peer = NULL;
    push(client_id);
}

int main()
{
    printf("Starting server...\n");
//    key_t server_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);
//    if(server_key == -1)
//    {
//        printf("Error with key - server\n");
//        return -1;
//    }
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    server_descriptor = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, 0666, &attr);
    if(server_descriptor == -1)
    {
        perror("");
        printf("Error with server_descriptor\n");
        return -2;
    }
    atexit(close_server);
    signal(SIGINT, exit);
    signal(SIGTERM, exit);
    signal(SIGSEGV, exit);
    for(int i = -1; i < MAX_CLIENT_COUNT; i++)
        push(i);

    char* msg = "";
    bool running = true;

    while(running)
    {
        printf("%zd", mq_receive(server_descriptor, msg, MAX_SIZE, NULL));
        perror("");
        printf("[SERVER] message received %s\n", msg);
        int type = msg[0];
        char* client_id_str = strtok(msg, " ");
        int client_id = atoi(client_id_str);
        int data = atoi(msg);
        printf("[SERVER] client %d, data %d\n", client_id, data);
        switch(type)
        {
            case Init:
                add_client(client_id);
                break;
            case List:
                send_list(client_id);
                break;
            case Connect:
                connect_client(client_id, data);
                break;
            case Disconnect:
                disconnect_client(client_id);
                break;
            case Stop:
                remove_client(client_id);
                break;
            default:
                printf("Dontthere is no command with this type %d\n", type);
        }
//        for(int i = 0; i <= last_element; i++)
//            printf("%d ", available_ID[i]);
    }
}