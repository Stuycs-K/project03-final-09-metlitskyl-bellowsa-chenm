#ifndef networking_h
#define networking_h

//init client and return fd of socket
int setup_client();


/*This method sets up the socket to be a SOCK_STREAM socket
It then binds it to port and sets ups the required address
and sets the socket to listen. It returns the server socket fd.*/
int setup_server();

#endif