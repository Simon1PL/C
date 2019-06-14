#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //struck sockaddr_in

#define MAX_CLIENTS 10

typedef enum MessageType {
    REGISTER = 0,
    UNREGISTER = 1,
    SUCCESS = 2,
    FAILSIZE = 3,
    WRONGNAME = 4,
    REQUEST = 5,
    RESULT = 6,
    PING = 7,
    PONG = 8,
    END = 9
} MessageType;

typedef enum Message{
    char connectionType[10];
    char name[128];
    char text[16384];
    enum MessageType messageType;
} Message;

typedef struct Request{
    char text[10240];
    int ID;
} Request;

typedef struct Client {
    char *name;
    int activeCounter;
    char connectionType[10];
    struct sockaddr* sockaddr;
    int reserved;
    socklen_t socklen;
} Client;