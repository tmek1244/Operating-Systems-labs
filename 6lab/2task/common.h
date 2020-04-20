#ifndef SYSOPS_COMMON_H
#define SYSOPS_COMMON_H

#include <stdbool.h>
//#include <sys/ipc.h>
//#include <sys/msg.h>
#include <sys/types.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SERVER_QUEUE_NAME "/server1"

#define MAX_CLIENT_COUNT 10
#define MAX_SIZE 8192
typedef enum msg_t {
    Stop = 1,
    Disconnect = 2,
    List = 3,
    Connect = 4,
    Init = 5,
} msg_t;

struct message {
    long mtype;
    int id;
    int data;
    struct List {int size; struct { int id; bool available; } clients[MAX_CLIENT_COUNT]; } List;
};



#endif //SYSOPS_COMMON_H
