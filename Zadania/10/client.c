#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //struck sockaddr_in

int main() {
    //create a socket
    int socket = socket(AF_INET, SOCK_STREAM, 0);
    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002); //0?
    server_address.sin_addr.s_addr = INADDR_ANY;
    //connect
    int connection_status=connect(socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (!connection_status) printf("connection ok\n");
    //recive data from the server
    char server_response[256];
    recv(socket, &server_response, sizeof(server_response), 0);
    //print out the server's response
    printf("The server sent the data: %s\n", server_response);
    //close the socket
    close(socket);
    return 0;
}