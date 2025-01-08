// Client side C program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

#define PORT 8080

int main(int argc, char const* argv[]){
    

    //create socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    v_err(client_fd, "socket creation err", EXIT);

    //create sockaddr_in for serv_addr
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    int conversion = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    v_err(conversion, "conversion of ip addr err", EXIT);

    //attatch client_fd to server
    int status = connect(client_fd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)); 
    v_err(status, "connection err", EXIT);
        
    while(1){
        //what the client actually does
        char* hello = "Hello from client";
        char buffer[1024] = { 0 };

        write(client_fd, hello, strlen(hello));
        printf("Hello message sent\n");
        int valread;
        valread = read(client_fd, buffer,
                    1024 - 1); // subtract 1 for the null
                                // terminator at the end
        printf("%s\n", buffer);
    }
    // closing the connected socket
    close(client_fd);
    return 0;
}
