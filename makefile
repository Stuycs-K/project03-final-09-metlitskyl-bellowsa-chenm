compile: dit server download commit clone add push 

utils.o: utils.c
	gcc -c -g utils.c

# dit
dit: dit.o utils.o
	gcc -o dit -g dit.o utils.o

dit.o: dit.c utils.h
	gcc -c -g dit.c

# Server
server: server.o utils.o
	gcc -o server -g server.o utils.o

server.o: server.c utils.h
	gcc -c -g server.c

# Download
commit: commit.o utils.o
	gcc -o commit -g commit.o utils.o

commit.o: commit.c utils.h
	gcc -c -g commit.c

# clone
clone: clone.o utils.o
	gcc -o clone -g clone.o utils.o

clone.o: clone.c utils.h
	gcc -c -g clone.c

# add
add: add.o utils.o
	gcc -o add -g add.o utils.o

add.o: add.c utils.h
	gcc -c -g add.c
# push
push: push.o utils.o
	gcc -o push -g push.o utils.o

push.o: push.c utils.h
	gcc -c -g push.c

clean_p:
	rm -f dit server download commit clone add push client
clean_o:
	rm -f *.o