#ifndef SYSOPS_COMMON_H
#define SYSOPS_COMMON_H

#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVER_KEY_PATHNAME "/home/tmek/"
#define PROJECT_ID 'e'

#define MAX_CLIENT_COUNT 10
typedef enum msg_t {
    Stop = 1,
    Disconnect = 2,
    List = 3,
    Connect,
    Init,
} msg_t;


struct message {
    long mtype;
    int id;
    int data;
    struct List {int size; struct { int id; bool available; } clients[MAX_CLIENT_COUNT]; } List;
};



#endif //SYSOPS_COMMON_H
