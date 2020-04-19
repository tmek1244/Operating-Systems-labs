#include "common.h"
#include <signal.h>

int server_descriptor;
int client_descriptor;
char* client_queue_name;
char* id;

char* make_message(char* priority, char* data)
{
    char* message = malloc(2 + strlen(id) + strlen(data));
    strcat(message, priority);
    strcat(message, id);
    strcat(message, " ");
    strcat(message, data);
//    printf("make_message: %s\n", message);
    return message;
}

void send_stop()
{
    char* message = make_message("1", "");
    printf("send_stop message: %s\n", message);
    mq_send(server_descriptor, message, sizeof message, Stop);
}

void close_client()
{
    send_stop();
    mq_close(server_descriptor);
    mq_close(client_descriptor);
    mq_unlink(client_queue_name);
}

void print_list(char* list)
{
//    for(int i = 0; i < list.size; i++)
//    {
//        char* available = list.clients[i].available ? "available" : "not available";
//        printf("id: %d is %s\n", list.clients[i].id, available);
//    }
    printf("%s\n", list);
}

void get_list()
{
    char* message = make_message("3", "");
    mq_send(server_descriptor, message, sizeof message, List);
    char list[MAX_SIZE];
    mq_receive(client_descriptor, list, MAX_SIZE, NULL);

    print_list(list);
}

void connect(char* other_client_id)
{
//    printf("client_ID: %d", client_id);
    char* message = make_message("4", other_client_id);
//    struct message message;
//    message.mtype = Connect;
//    message.id = id;
//    message.data = client_id;
    mq_send(server_descriptor, message, sizeof message, Connect);
    char feedback[MAX_SIZE];
    mq_receive(client_descriptor, feedback, MAX_SIZE, NULL);
    if(strcmp(feedback,"-1") == 0)
    {
        printf("There is no user with this id\n");
    } else{
        printf("Other client qid: %s\n", feedback);
    }
}

void disconnect()
{
//    struct message message;
    char* message = make_message("2", "");
//    message.mtype = Disconnect;
//    message.id = id;
    mq_send(server_descriptor, message, sizeof message, Disconnect);
}

void check_for_message()
{
//    printf("Looking for messages\n");
//    struct message message;
    char message[MAX_SIZE];
    mq_receive(client_descriptor, message, MAX_SIZE, NULL);
    if(strcmp(message, "stop") != 0)
    {
        printf("Connect with other client with queue name: %s", message);
    }
    else if(strcmp(message, "stop") == 0)
    {
        printf("Server will be closed...\n");
        exit(0);
    }
}

void get_id()
{
//    struct message message = {Init, 0, client_descriptor};
    char str[12];
    sprintf(str, "%d", client_descriptor);
    char* message = make_message("5", str);
    mq_send(server_descriptor, message, sizeof message, Init);
    char feedback[MAX_SIZE];
    mq_receive(client_descriptor, feedback, MAX_SIZE, NULL);
    if(strcmp(feedback, "-1") == 0)
    {
        printf("Server is full!\n");
        exit(-1);
    }
    id = feedback;
    printf("received id: %s\n", id);
}

void set_server_descriptor()
{
    server_descriptor = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if(server_descriptor == -1)
    {
        printf("Error with server descriptor\n");
        exit(-1);
    }
}

void set_client_descriptor(char* queue_name)
{
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    if((client_descriptor = mq_open(queue_name, O_RDONLY | O_CREAT | O_EXCL,  0666, &attr)) == -1)
    {
        printf("Error with client_qid\n");
        exit(-4);
    }

}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Give a queue name!\n");
        exit(-3);
    }
    set_server_descriptor();
    set_client_descriptor(argv[1]);

    get_id();
    atexit(close_client);
    signal(SIGINT, exit);


    char* command = malloc(100);
    char* option = "";
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
            scanf("%s", option);
            connect(option);
        } else if(strcmp(command, "disconnect") == 0)
        {
            disconnect();
        }
        check_for_message();
    }
    return 0;
}