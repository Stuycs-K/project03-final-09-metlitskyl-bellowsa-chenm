#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//socket stuff
#include <sys/socket.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netdb.h>

//signal stuff
#include <signal.h>

//reap children
#include <sys/wait.h>

#include "utils.h"
#define PORT 8080

void sighandler(int signo){
  switch(signo){ //child 
    case SIGCHLD:
      {
        printf("reaping child...\n");
        while(waitpid(-1,NULL,0));
      }
    // cleanup();
  }

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
    int addr_return = getaddrinfo("127.0.0.1", "9845", &hints, &results);  //Server sets node to NULL
    v_err(addr_return, "getaddrinfo", 1);

    // create Socket stream socket
    server_fd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    v_err(server_fd, "socket creation err: ", 1);

    int bind_result = bind(server_fd, results->ai_addr, results->ai_addrlen);
    v_err(bind_result,"binding socket",1);
        
    //set server_fd to listen and set max number of waiting connections to 3
    int listen_result = listen(server_fd, 3);
    v_err(listen_result,"listen",1); 
    //end server_fd setup

    freeaddrinfo(results);
    return server_fd;
}

int server_action(int new_socket){
    while(1){
        char buffer[1024];
        ssize_t valread;
        valread = read(new_socket, buffer,
                    1024); // subtract 1 for the null
                                // terminator at the end
        printf("recieved %s\n", buffer);
        char * hello = "<html><body><p>test</p></body></html>";
        write(new_socket, hello, strlen(hello));
        printf("Hello message sent\n");
        sleep(1);
    }
    return 0;
}

int main(int argc, char const* argv[]){
    signal(SIGCHLD, sighandler); //set SIGCHILD to reaper...

    //good resource for sockets: https://man7.org/linux/man-pages/man7/ip.7.html https://man7.org/linux/man-pages/man2/socket.2.html 
    //set up server listening ...
    int server_fd = setup_server();
    //server loop
    while(1){ 
        //main server loop
        printf("establishing connection to client...\n");
    
        int new_socket;
        new_socket = accept(server_fd, NULL,NULL); //block until a client tries to connect
        v_err(new_socket,"accept",1);
        
        printf("recived client...\n");
        printf("forking...\n");
        if(fork()==0){//if fork is child
            // do what the server should do
            server_action(new_socket); 
            // closing the connected socket
            close(new_socket);
            // closing the listening socket
            close(server_fd);
            exit(0);
        }

        close(new_socket);
        
    
  }

  return 0;
    
}
