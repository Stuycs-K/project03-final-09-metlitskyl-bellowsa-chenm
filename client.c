// Client side C program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


//socket stuff
#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h>

#include "utils.h"

#define PORT 8080

int main(int argc, char const* argv[]){
    
    
    struct addrinfo * results;//results is allocated in getaddrinfo
    struct addrinfo hints; 
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; //TCP socket
    hints.ai_flags = AI_PASSIVE; //only needed on server
    int addr_return = getaddrinfo(NULL, "9845", &hints, &results);  //Server sets node to NULL
    v_err(addr_return, "getaddrinfo", 1);


    //create socket
    int client_fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    v_err(client_fd, "socket creation err", EXIT);

    //attatch client_fd to server
    int status = connect(client_fd, results->ai_addr, results->ai_addrlen); 
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
    freeaddrinfo(results);
    return 0;
}
