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

#include <fcntl.h>
//for mkdir
#include <sys/stat.h>
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
    char buffer[1024 + sizeof(int)];
   
    // int mode = -1;
    // read(client_fd, &mode, sizeof(int));
    while(read(client_fd, buffer,sizeof(buffer))){
        int mode = (int)(buffer[0]);
        if (mode == TR_FILE){
            printf("creating FILE: %s\n", buffer+4);
            int fd = open(buffer +4, O_CREAT, 0644);
            v_err(fd, "CREAT file failed", 0);
            close(fd);
        }
        else{
            printf("creating DIR: %s\n", buffer+4);
            int r = mkdir(buffer+4,0744);
            v_err(r, "mkdir failed", 0);

        }
    }
        
    
    // closing the connected socket
    close(client_fd);
    freeaddrinfo(results);
    return 0;
}
