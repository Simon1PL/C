#include <stdio.h> 
#include<stdlib.h> 
#include<sys/types.h> 
#include<sys/socket.h>
 #include<netinet/in.h> 
 int main() { 
     char server_message[100] = "Message"; 
     int server_socket; 
     server_socket = socket(AF_INET, SOCK_STREAM, 0);
      struct sockaddr_in server_addr; 
      server_addr.sin_family = AF_INET; 
     server_addr.sin_port = htons(9999); 
      server_addr.sin_addr.s_addr = INADDR_ANY; 
      //bind the socket to the specified IP addr and port
       bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)); 
       listen(server_socket, 3); 
       int client_socket; 
       client_socket = accept(server_socket, NULL, NULL); 
       int send_status; 
       char server_message[100] = ""; 
       send_status=send(client_socket, server_message, sizeof(server_message), 0); 
       close(server_socket);
        return 0; 
       }