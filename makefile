compile: client server

client: basic_client.o pipe_networking.o
	gcc -o client -g basic_client.o pipe_networking.o

server: forking_server.o pipe_networking.o
	gcc -o server -g forking_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	gcc -c -g basic_client.c

forking_server.o: forking_server.c pipe_networking.h
	gcc -c -g forking_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c -g pipe_networking.c

clean:
	rm *.o
	rm client server
