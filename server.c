#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "utils.h"
#define PORT 8080


int main(int argc, char const* argv[]){
    //good resource: https://man7.org/linux/man-pages/man7/ip.7.html
    int server_fd;
    ssize_t valread;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    
    // create Socket stream socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    v_err(server_fd, "socket creation err: ", 1);

    // configure socket
    v_err(setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)),"setsocketopt", 1);
        
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    int bind_result = bind(server_fd, (struct sockaddr*)&address,sizeof(address));
    v_err(bind_result,"binding socket",1);
        
    //set server_fd to listen and set max number of waiting connections to 3
    int listen_result = listen(server_fd, 3);
    v_err(listen_result,"listen",1); 

    int new_socket;
    new_socket = accept(server_fd, (struct sockaddr*)&address,&addrlen); //block until a client tries to connect
    v_err(new_socket,"accept",1);
    
    char buffer[1024];
    valread = read(new_socket, buffer,
                   1024); // subtract 1 for the null
                              // terminator at the end
    printf("%s\n", buffer);
    char * hello = "<html><body><p>test</p></body></html>";
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");



    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);
    return 0;
}
