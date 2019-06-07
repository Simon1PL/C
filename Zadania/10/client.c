#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //struck sockaddr_in

int main() {
        printf("aaAAAAAAAAAAAAAAAAAAAAAAAAAaa");
    //create a socket
    int network_socket = socket(AF_INET, SOCK_STREAM, 0);
    //specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002); //0?
    server_address.sin_addr.s_addr = INADDR_ANY;
    //connect
    int connection_status=connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if (!connection_status) printf("connection ok\n");
    //recive data from the server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);
    //print out the server's response
    printf("The server sent the data: %s\n", server_response);
    //close the socket
    close(network_socket);
    return 0;
}