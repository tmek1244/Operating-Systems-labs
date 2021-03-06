#include "common.h"
#include <signal.h>

int server_qid;
int client_qid;
int id;

void send_stop()
{
    struct message message = {Stop, id};
    msgsnd(server_qid, &message, sizeof message, 0);
}

void close_client()
{
    send_stop();
    msgctl(client_qid, IPC_RMID, NULL);
}

void print_list(struct List list)
{
    for(int i = 0; i < list.size; i++)
    {
        char* available = list.clients[i].available ? "available" : "not available";
        printf("id: %d is %s\n", list.clients[i].id, available);
    }
}

void get_list()
{
    struct message message;
    message.mtype = List;
    message.id = id;
    message.data = 0;
    msgsnd(server_qid, &message, sizeof message, 0);
    msgrcv(client_qid, &message, sizeof message, -100, 0);

    print_list(message.List);
}

void connect(int client_id)
{
//    printf("client_ID: %d", client_id);
    struct message message;
    message.mtype = Connect;
    message.id = id;
    message.data = client_id;
    msgsnd(server_qid, &message, sizeof message, 0);
    msgrcv(client_qid, &message, sizeof message, -100, 0);
    if(message.data == -1)
    {
        printf("There is no user with this id\n");
    } else{
        printf("Other client qid: %d\n", message.data);
    }
}

void disconnect()
{
    struct message message;
    message.mtype = Disconnect;
    message.id = id;
    msgsnd(server_qid, &message, sizeof message, 0);
}

void check_for_message()
{
//    printf("Looking for messages\n");
    struct message message;
    msgrcv(client_qid, &message, sizeof message, -100, IPC_NOWAIT);
    if(message.mtype == Connect)
    {
        printf("Connect with other client with qid: %d", message.data);
    }
    else if(message.mtype == Stop)
    {
        printf("Server will be closed...\n");
        exit(0);
    }
}

void get_id()
{
    struct message message = {Init, 0, client_qid};
    msgsnd(server_qid, &message, sizeof message, 0);
    msgrcv(client_qid, &message, sizeof message, -100, 0);
    if(message.mtype == Init && message.data == -1)
    {
        printf("Server is full!\n");
        exit(-1);
    }
    id = message.data;
    printf("received id: %d\n", message.data);
}

void set_server_qid()
{
    key_t server_key = ftok(SERVER_KEY_PATHNAME, PROJECT_ID);
    if(server_key == -1)
    {
        printf("Error with key - server\n");
        exit(-2);
    }

    if((server_qid = msgget(server_key, 0666)) == -1)
    {
        printf("Error with server_qid - client\n");
        exit(-3);
    }
}

void set_client_qid()
{
    if((client_qid = msgget(IPC_PRIVATE, 0666)) == -1)
    {
        printf("Error with client_qid\n");
        exit(-4);
    }

}

int main()
{
    set_server_qid();
    set_client_qid();

    get_id();
    atexit(close_client);
    signal(SIGINT, exit);


    char* command = malloc(100);
    int option = 0;
    while(strcmp(command, "stop") != 0)
    {
        printf(">> ");
        scanf("%s", command);
        if(strcmp(command, "list") == 0)
        {
            get_list();
        } else if(strcmp(command, "connect") == 0)
        {
            printf("  id: ");
            scanf("%d", &option);
            connect(option);
        } else if(strcmp(command, "disconnect") == 0)
        {
            disconnect();
        }
        check_for_message();
    }
    return 0;
}