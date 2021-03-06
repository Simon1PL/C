#define _XOPEN_SOURCE 500
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <qemu-common.h>

#include <string.h>
#include <pthread.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "common.h"
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>

size_t get_file_size(const char *file_name) {
    int fd;
    if ((fd = open(file_name, O_RDONLY)) == -1) {
        perror("Unable to open file for size");
        return (size_t) -1;
    }
    struct stat stats;
    fstat(fd, &stats);
    size_t size = (size_t) stats.st_size;
    close(fd);
    return size;
}

size_t read_whole_file(const char *file_name, char *buffer) {
    size_t size = get_file_size(file_name);
    if (size == -1) {
        return size;
    }
    FILE *file;
    if ((file = fopen(file_name, "r")) == NULL) {
        perror("Unable to open file");
        return (size_t) -1;
    }
    size_t read_size;
    if ((read_size = fread(buffer, sizeof(char), size, file)) != size) {
        perror("Unable to read file");
        return (size_t) -1;
    }
    fclose(file);
    return read_size;
}

void handle_signal(int);

void init(char *, char *);

void handle_message(int);

void register_client(char *, int);

void unregister_client(char *);

void clean();

void *ping_routine(void *);

void *hendle_terminal(void *);

void handle_connection(int);

void delete_client(int);

void delete_socket(int);

int in(void *const a, void *const pbase, size_t total_elems, size_t size, __compar_fn_t cmp) {
    char *base_ptr = (char *) pbase;
    if (total_elems > 0) {
        for (int i = 0; i < total_elems; ++i) {
            if ((*cmp)(a, (void *) (base_ptr + i * size)) == 0) return i;
        }
    }
    return -1;
}

int cmp_name(char *name, Client *client) {
    return strcmp(name, client->name);
}

int web_socket;
int local_socket;
int epoll;
char *local_path;
int id;

pthread_t ping;
pthread_t command;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Client clients[CLIENT_MAX];
int clients_amount = 0;


int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 3)
    {
        perror("Expexted 2 arguments");
        return -1;
    }

    atexit(clean);

    init(argv[1], argv[2]);

    struct epoll_event event;
    int x = 1;
    while (x) {
        if (epoll_wait(epoll, &event, 1, -1) == -1)
        {
            perror("epoll_wait failed");
            return -1;
        }

        if (event.data.fd < 0)
            handle_connection(-event.data.fd);
        else
            handle_message(event.data.fd);
    }
}

void *ping_routine(void *arg) {
    uint8_t message_type = PING;
    int x = 1;
    while (x) {
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < clients_amount; ++i) {
            if (clients[i].active_counter != 0) {
                printf("num: %d \n",clients[i].active_counter );
                printf("Client \"%s\" is not responding. Removing...\n", clients[i].name);
                delete_client(i--);
            } else {
                if (write(clients[i].fd, &message_type, 1) != 1)
                {
                   perror("Could not send ping to client");
                   exit(-1);
                }
                clients[i].active_counter++;
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(10);
    }
    return NULL;
}

void send_msg(int type, int len, request_t *req, int i) {
    if (write(clients[i].fd, &type, 1) != 1) {
        perror("cannot send");
        return;
    }
    if (write(clients[i].fd, &len, 2) != 2) {
        perror("cannot send");
        return;
    }
    if (write(clients[i].fd, req, len) != len) {
        perror("cannot send");
        return;
    }
}

void *hendle_terminal(void *arg) {
    char *file_buffer;
    request_t *req;
    int true = 1;
    while (true) {
        char buffer[256];
        printf("Enter command: \n");
        fgets( buffer,256,stdin);
        file_buffer = (char*) calloc(sizeof(char),10240);
        req = (request_t*) calloc(sizeof(request_t),1);
        memset( file_buffer, '\0', sizeof(char)*10240);
        int scan_res = sscanf(buffer, "%s", file_buffer);
        printf("State: %d \n", scan_res);

        if (scan_res == 1) {
            id++;
            printf("Request ID: %d \n", id);
            printf("%s \n", file_buffer);
            int status = read_whole_file(file_buffer, req->text);
            req->ID = id;
            if(strlen(file_buffer) <= 0){
                printf("Provided empty file \n");
                continue;
            }
            if (status < 0) {
                printf("Wrong file format \n");
                continue;
            }
            int i = 0;
            int sent = 0;
            for (i = 0; i < clients_amount; i++) {
                if (clients[i].reserved == 0) {
                    printf("Request has been sent to %s \n", clients[i].name);
                    clients[i].reserved = 1;
                    send_msg(REQUEST, sizeof(request_t), req, i);
                    sent = 1;
                    break;
                }
            }
            if (!sent) {
                i = 0;
                if (clients[i].reserved > -1){
                    printf("Request has been sent to %s \n", clients[i].name);
                    clients[i].reserved  ++;
                    send_msg(REQUEST, sizeof(request_t), req, i);
                }

            }
            printf("REQUEST SENT \n");

        }
        free(req);
        free(file_buffer);
    }
    return NULL;
}

void handle_connection(int socket) {
    int client = accept(socket, NULL, NULL);
    if (client == -1) {
      perror("Could not accept new client");
      return;
    }

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLPRI;
    event.data.fd = client;

    if (epoll_ctl(epoll, EPOLL_CTL_ADD, client, &event) == -1)
        perror("Could not add new client to epoll");

}

void handle_message(int socket) {
    uint8_t message_type;
    uint16_t message_size;

    if (read(socket, &message_type, TYPE_SIZE) != TYPE_SIZE) {
      perror("Could not read message type");
      return;
    }
    if (read(socket, &message_size, LEN_SIZE) != LEN_SIZE) {
      perror("Could not read message size");
      return;
    }
    char *client_name = malloc(message_size);

    switch (message_type) {
        case REGISTER: {
            if (read(socket, client_name, message_size) != message_size) {
                perror(" Could not read register message name");
                return;
            }
            register_client(client_name, socket);
            break;
        }
        case UNREGISTER: {
            if (read(socket, client_name, message_size) != message_size) {
                perror(" Could not read unregister message name");
                return;
            }
            unregister_client(client_name);
            break;
        }
        case RESULT: {
            printf("Received result... \n");
            char result[10240];
            if (read(socket, client_name, message_size) < 0){
                perror("read res name");
                return;
            }
            int size;
            if (read(socket, &size, sizeof(int)) < 0) {
                perror("size of res");
                return;
            }
            if (read(socket, result, size) < 0) {
                perror("result");
                return;
            }

            printf("Computations from: %s :\n%s \n",client_name, result);

            int i;
            for(i = 0; i < CLIENT_MAX; i++){
                if(clients[i].reserved > 0 && strcmp(client_name, clients[i].name) == 0){
                    clients[i].reserved --;
                    clients[i].active_counter = 0;
                    printf("Client %s finished all his tasks\n", client_name);
                }
            }
            break;
        }
        case PONG: {
            if (read(socket, client_name, message_size) != message_size){
                perror(" Could not read ping return message");
                return;
            }
            pthread_mutex_lock(&mutex);
            int i = in(client_name, clients, (size_t) clients_amount, sizeof(Client), (__compar_fn_t) cmp_name);
            if (i >= 0) clients[i].active_counter = clients[i].active_counter == 0 ? 0 : clients[i].active_counter-1;
            pthread_mutex_unlock(&mutex);
            break;
        }
        default:
            printf("Unknown message type\n");
            break;
    }
    free(client_name);
}

void register_client(char *client_name, int socket) {
    uint8_t message_type;
    pthread_mutex_lock(&mutex);
    if (clients_amount == CLIENT_MAX) {
        message_type = FAILSIZE;
        if (write(socket, &message_type, 1) != 1) {
            perror("Could not write FAILSIZE message to client");
            return;
        }
        delete_socket(socket);
    } else {
        int exists = in(client_name, clients, (size_t) clients_amount, sizeof(Client), (__compar_fn_t) cmp_name);
        if (exists != -1) {
            message_type = WRONGNAME;
            if (write(socket, &message_type, 1) != 1){
                perror("Could not write WRONGNAME message to client");
                return;
            }
            delete_socket(socket);
        } else {
            clients[clients_amount].fd = socket;
            clients[clients_amount].name = malloc(strlen(client_name) + 1);
            clients[clients_amount].active_counter = 0;
            clients[clients_amount].reserved = 0;
            strcpy(clients[clients_amount++].name, client_name);
            message_type = SUCCESS;
            if (write(socket, &message_type, 1) != 1){
                perror("Could not write SUCCESS message to client");
                return;
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

void unregister_client(char *client_name) {
    pthread_mutex_lock(&mutex);
    int i = in(client_name, clients, (size_t) clients_amount, sizeof(Client), (__compar_fn_t) cmp_name);
    if (i >= 0) {
        delete_client(i);
        printf("Client \"%s\" unregistered\n", client_name);
    }
    pthread_mutex_unlock(&mutex);
}

void delete_client(int i) {
    delete_socket(clients[i].fd);

    free(clients[i].name);

    clients_amount--;
    for (int j = i; j < clients_amount; ++j)
        clients[j] = clients[j + 1];

}

void delete_socket(int socket) {
    if (epoll_ctl(epoll, EPOLL_CTL_DEL, socket, NULL) == -1) {
        perror("Could not remove client's socket from epoll");
        return;
    }

    if (shutdown(socket, SHUT_RDWR) == -1){
      perror("Could not shutdown client's socket");
      return;
    }

    if (close(socket) == -1){
      perror("Could not close client's socket");
      return;
    }
}

void handle_signal(int signo) {
    printf("SIGINT\n");
    exit(1);
}

void init(char *port, char *path) {

    struct sigaction act;
    act.sa_handler = handle_signal;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

    int i;
    for (i = 0; i < CLIENT_MAX; i++) {
        clients[i].reserved = -1;
    }

    uint16_t port_num = (uint16_t) atoi(port);
    local_path = path;

    struct sockaddr_in web_address;
    memset(&web_address, 0, sizeof(struct sockaddr_in));
    web_address.sin_family = AF_INET;
    web_address.sin_addr.s_addr =htonl(INADDR_ANY); //inet_addr("path"); //inet_addr("192.168.0.66"); //htonl(INADDR_ANY);
    web_address.sin_port = htons(port_num);

    if ((web_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Could not create web socket");
        return;
    }

    if (bind(web_socket, (const struct sockaddr *) &web_address, sizeof(web_address))){
        perror("Could not bind web socket");
        return;
    }

    if (listen(web_socket, 64) == -1){
        perror("Could not listen to web socket");
        return;
    }

    struct sockaddr_un local_address;
    local_address.sun_family = AF_UNIX;

    sprintf(local_address.sun_path, "%s", local_path);

    if ((local_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
        perror("Could not create local socket");
        return;
    }
    if (bind(local_socket, (const struct sockaddr *) &local_address, sizeof(local_address))){
        perror(" Could not bind local socket");
        return;
    }
    if (listen(local_socket, 64) == -1){
        perror("Could not listen to local socket");
        return;
    }

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLPRI;
    if ((epoll = epoll_create1(0)) == -1){
        perror("Could not create epoll");
        return;
    }
    event.data.fd = -web_socket;
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, web_socket, &event) == -1){
        perror("Could not add Web Socket to epoll");
        return;
    }
    event.data.fd = -local_socket;
    if (epoll_ctl(epoll, EPOLL_CTL_ADD, local_socket, &event) == -1){
        perror("Could not add Local Socket to epoll");
        return;
    }

    if (pthread_create(&ping, NULL, ping_routine, NULL) != 0){
        perror("Could not create Pinger Thread");
        return;
    }
    if (pthread_create(&command, NULL, hendle_terminal, NULL) != 0){
        perror("Could not create Commander Thread");
        return;
    }
}

void clean() {
    printf("CLEANUP \n");
    pthread_cancel(ping);
    pthread_cancel(command);
    if (close(web_socket) == -1)
        perror("Could not close Web Socket");
    if (close(local_socket) == -1)
        perror("Could not close Local Socket");
    if (unlink(local_path) == -1)
        perror("Could not unlink Unix Path");
    if (close(epoll) == -1)
        perror("Could not close epoll");
}
