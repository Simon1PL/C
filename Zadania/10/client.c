#include "message.h"

char* name;
char *connectionType;
char *serverIp;
uint16_t port;
void webConnect();
void localConnect();
void makeTask();
void sendMessage();
void registerOnServer();
int network_socket;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv) {
    if (!((argc == 4 && argv[2]=="local") || (argv[2]=="web" && argc == 5))) {
        printf("use: name \"local\" socketName OR\n name \"web\" serverIP port");
    }
    name=argv[1];
    connectionType = argv[2];
    serverIp = argv[3];
    if (argc == 5) {
        port = (uint16_t) atoi(argv[4]);
        webConnect();
    }
    else {
        localConnect();
    }
    registerOnServer();
    atexit(disconnect);
    makeTask();
    return 0;
}

void makeTask(){
    int messageType;
    pthread_t thread;
    while(1){
        read(network_socket, &messageType, sizeof(int));
        switch(messageType){
            case REQUEST:
                printf("got task");
                Request request;
                read(network_socket, &request, sizeof(Request));
                pthread_create(&thread, NULL, DoTask, &request);
                pthread_detach(thread);
                break;
            case PING:
                pthread_mutex_lock(&requestMutex);
                sendMessage(PONG, NULL);
                pthread_mutex_unlock(&requestMutex); 
                break; 
        }
    }
}

void sendMessage(int messageType, char *text) {
    Message msg;
    msg.messageType = messageType;
    snprintf(msg.name, 64, "%s", name);
    msg.connectionType = connectionType;
    if (text) {
        snprintf(msg.value, strlen(text), "%s", text);
    }
    pthread_mutex_lock(&mutex);
    write(network_socket, &msg, sizeof(Message));
    pthread_mutex_unlock(&mutex);
}

void registerOnServer() {
    sendMessage(REGISTER, NULL);
    int messageType;
    read(network_socket, &messageType, 1);
    switch (messageType) {
        case WRONGNAME:
            printf("Unavilable name");
            exit(1);
        case FAILSIZE:
            printf("Too many clients");
            exit(1);
        case SUCCESS:
            printf("SUCCESS!!\n");
            break;
        default:
            printError("Unavailable message");
    }
}

void webConnect(){
    //create a socket
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    //connect
    int connection_status=connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connection_status) printf("connection failed\n");
}
void localConnect(){
    //create a socket
    network_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    //specify an address for the socket
    struct sockaddr_un server_address_name;
    server_address_name.sun_family = AF_UNIX;
    snprintf(server_address_name.sun_path, 100, "%s", name);
    bind(network_socket, (const struct sockaddr *) &server_address_name, sizeof(server_address_name));
    //connect
    struct sockaddr_un server_address;
    server_address.sun_family = AF_UNIX;
    snprintf(server_address.sun_path, 100, "%s", serverIp);
    int connection_status=connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (connection_status) printf("connection failed\n");
}

void *DoTask(void *arg) {
    printf("task start\n");
    struct Request *arguments = arg;
    struct Request currentRequest;
    strcpy(currentRequest.text, arguments->text);
    currentRequest.ID = arguments->ID;
    char *buffer = malloc(100 + 2 * strlen(currentRequest.text));
    char *text = malloc(100 + 2 * strlen(currentRequest.text));
    sprintf(buffer, "echo '%s' | awk '{for(x=1;$x;++x)print $x}' | sort | uniq -c", (char *) currentRequest.text);
    FILE *result = popen(buffer, "r");
    int n = fread(buffer, 1, 99 + 2 * strlen(currentRequest.text), result);
    buffer[n] = '\0';
    int words_count = 1;
    char *res = strtok(currentRequest.text, " ");
    while (strtok(NULL, " ") && res) {
        words_count++;
    }
    sprintf(text, "ID: %d\tSUM: %d\tWORDS: \n%s", currentRequest.ID, words_count, buffer);
    pthread_mutex_lock(&requestMutex);
    sendMessage(RESULT, text);
    pthread_mutex_unlock(&requestMutex);
    free(buffer);
    free(text);
    return NULL;
}

void disconnect(){
    sendMessage(UNREGISTER, NULL);
    //clear the socket file
    unlink(name);
    //close the socket
    close(network_socket);
}