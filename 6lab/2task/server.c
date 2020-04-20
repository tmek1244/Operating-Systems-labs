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
    char* clients_list = malloc(1000);
    memset(clients_list, '\0', strlen(clients_list));
    char* next_client = malloc(100);
    for (int i = 0; i < MAX_CLIENT_COUNT; i++)
        if (clients[i].descriptor != 0 && i != client_id) {
            char* available = "not available";
            if(clients[i].peer == NULL)
                available = "available";

            memset(clients_list, '\0', strlen(clients_list));
            sprintf(next_client, "client_id: %d is %s\n", i, available);
            strcat(clients_list, next_client);
        }
    printf("list: %s %lu\n", clients_list, strlen(clients_list));
    mq_send(clients[client_id].descriptor, clients_list, strlen(clients_list), 0);
}

void add_client(char* client_queue_name)
{
    int new_id = pop();
//    struct message id_message = {Init, -1, new_id};
    char message[12];
    int client_descriptor = mq_open(client_queue_name, O_WRONLY, 0666, NULL);
    memset(message, '\0', sizeof(message));
    sprintf(message, "%d", new_id);
    if(new_id == -1)
    {
        push(-1);
    } else
    {
        clients[new_id].descriptor = client_descriptor;
        clients[new_id].peer = NULL;
        printf("[SERVER] add_client with id: %d and descriptor: %d\n", new_id, client_descriptor);
    }
    mq_send(client_descriptor, message, sizeof message, 0);

}

void connect_client(int client_id, int other_client_id)
{
    char message[MAX_SIZE];
    sprintf(message, "-1");
    if(is_on_stack(other_client_id) || other_client_id >= MAX_CLIENT_COUNT || client_id == other_client_id)
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
    mq_close(clients[client_id].descriptor);
    clients[client_id].descriptor = 0;
    clients[client_id].peer = NULL;
    push(client_id);
}

char* split_message(const char* message, int* type, int* id)
{
    (*type) = message[0] - '0';
    int end_of_id = 1;
    while(message[end_of_id] != ' ')
    {
        end_of_id++;
    }
    char id_str[5];
    memset(id_str, '\0', sizeof(id_str));
    strncpy(id_str, message+1, end_of_id - 1);
    printf("id: %s", id_str);
    (*id) = atoi(id_str);
    char* data = malloc(100);
    memset(data, '\0', sizeof(id_str));
    if(strlen(message) > end_of_id + 1)
    {
        strcpy(data, &message[end_of_id+1]);
    }
    return data;
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

    char* msg = malloc(MAX_SIZE);
    bool running = true;

    while(running)
    {
        if(mq_receive(server_descriptor, msg, MAX_SIZE, NULL) == -1)
        {
            perror("mq receive\n");
        }
//        printf("mq_receive: %zd\n", );
//        perror("");
        printf("[SERVER] message received %s\n", msg);
        int type = 0;
        int client_id = 0;
        char* data;
        data = split_message(msg, &type, &client_id);
        printf("[SERVER] client id: %d, data: %s\n", client_id, data);
        switch(type)
        {
            case Init:
                add_client(data);
                break;
            case List:
                send_list(client_id);
                break;
            case Connect:
                connect_client(client_id, atoi(data));
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