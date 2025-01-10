// Client side C program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//socket stuff
#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h>

#include <fcntl.h>
//for mkdir
#include <sys/stat.h>
#include "utils.h"
#include "file_transfer.h"

#define PORT "9845"

int main(int argc, char const* argv[]){
    struct addrinfo * results;//results is allocated in getaddrinfo
    struct addrinfo hints; 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; //TCP socket

    int addr_return = getaddrinfo("127.0.0.1", PORT, &hints, &results);  //Server sets node to NULL
    
    v_err(addr_return, "getaddrinfo", 1);


    //create socket
    int client_fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    v_err(client_fd, "socket creation err", EXIT);

    //attatch client_fd to server
    int status = connect(client_fd, results->ai_addr, results->ai_addrlen); 
    v_err(status, "connection err", EXIT);
        
    printf("connected...\n");
    
    while(1){
        struct file_transfer ft;
        int bytes = read(client_fd, &ft,sizeof(ft));
        v_err(bytes, "read err", 1);
        
        printf("    clinet line 49 recieves %d bytes...\n", bytes);
        // for (int i = 0; i<bytes; i++){
        //     printf("%c", ((char*)&ft)[i]);
        // }
        // printf("\n\n");

        if(bytes == 0){
            printf("no more bytes to read...\n");
            break;
        }
        if(ft.mode == TR_END){
            printf("recv exit signal...\n");
            break;
        }
        recv_file(client_fd, &ft);
    }
        
    
    // closing the connected socket
    close(client_fd);
    freeaddrinfo(results);
    return 0;
}
