#include "message.h"

int main(){
    char server_message[256] = "You have reached the server!";
    //create the server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    struct sockaddr_un unix_address;
    unix_address.sun_family = AF_UNIX;
    strcpy(unix_address.sun_path, UNIX_PATH);
    //bind the socket to our specified IP and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    bind(unix_socket, (struct sockaddr*) &unix_address, sizeof(unix_address));
    listen(server_socket, 10);
    listen(unix_socket, 10);
    int epoll=epoll_create1(0);
    int client_socket=accept(server_socket, NULL, NULL);
    //send the message
    int send_status=send(client_socket, server_message, sizeof(server_message), 0);
    //close the socket
    close(server_socket);
    return 0;
}