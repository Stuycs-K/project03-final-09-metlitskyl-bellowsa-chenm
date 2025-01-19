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
#include "networking.h"
#include "user.h"
#define PORT "9845"
#define ADDRESS "127.0.0.1"


//init client and 
int setup_client(char * ip){
    struct addrinfo * results;//results is allocated in getaddrinfo
    struct addrinfo hints; 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; //TCP socket

    printf("IP: %s\n", ip);
    int addr_return = getaddrinfo(ip, PORT, &hints, &results);  //Server sets node to NULL
    
    v_err(addr_return, "getaddrinfo", 1);


    //create socket
    int client_fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    v_err(client_fd, "socket creation err", EXIT);

    //attatch client_fd to server
    int status = connect(client_fd, results->ai_addr, results->ai_addrlen); 
    v_err(status, "connection err", EXIT);

    freeaddrinfo(results);

    return client_fd;
}


/*================SETUP SERVER===============
This method sets up the socket to be a SOCK_STREAM socket
It then binds it to port 8000 and sets ups the required address
and sets the socket to listen. It returns the server socket fd.*/
int setup_server(){
    int server_fd; //server_fd is analogous to WKP

    struct addrinfo * results;//results is allocated in getaddrinfo
    struct addrinfo hints; 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; //TCP socket
    hints.ai_flags = AI_PASSIVE; //only needed on server
    int addr_return = getaddrinfo("0.0.0.0", PORT, &hints, &results); 
    v_err(addr_return, "getaddrinfo", 1);

    server_fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    v_err(server_fd, "socket creation err: ", 1);

    int opt=1;
    v_err(setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)),"setsocketopt", 1);

    int bind_result = bind(server_fd, results->ai_addr, results->ai_addrlen);
    v_err(bind_result,"binding socket",1);
        
    //set server_fd to listen and set max number of waiting connections to 3
    int listen_result = listen(server_fd, 3);
    v_err(listen_result,"listen",1); 
    //end server_fd setup

    freeaddrinfo(results);

    v_err(server_fd,"accept",1);

    return server_fd;
}