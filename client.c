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
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; //TCP socket

    int addr_return = getaddrinfo("127.0.0.1", "9845", &hints, &results);  //Server sets node to NULL
    
    v_err(addr_return, "getaddrinfo", 1);


    //create socket
    int client_fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    v_err(client_fd, "socket creation err", EXIT);

    //attatch client_fd to server
    int status = connect(client_fd, results->ai_addr, results->ai_addrlen); 
    v_err(status, "connection err", EXIT);
        
    printf("connected...\n");
    char buffer[1024];
   
    while(read(client_fd, buffer,sizeof(buffer))){
        printf("%s\n", buffer);
    }
        
    
    // closing the connected socket
    close(client_fd);
    freeaddrinfo(results);
    return 0;
}
