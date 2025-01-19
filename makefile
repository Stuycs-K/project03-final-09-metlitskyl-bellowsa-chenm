compile: dit server download commit clone add push client build

utils.o: utils.c
	gcc -c -g utils.c

sound.o: sound.c
	gcc -c -g sound.c

file_transfer.o: file_transfer.c
	gcc -c file_transfer.c

networking.o: networking.c networking.h utils.h 
	gcc -c networking.c

user.o: user.c
	gcc -c user.c

client: client.o utils.o file_transfer.o networking.o user.o sound.o
	gcc -o client -g client.o utils.o file_transfer.o networking.o user.o sound.o

client.o: client.c utils.h file_transfer.h networking.h user.h sound.h
	gcc -c -g client.c

# dit
dit: dit.o utils.o client_module.o file_transfer.o networking.o user.o sound.o build.o add.o commit.o diff.o patch.o download.o push.o init.o
	gcc -o dit dit.o utils.o client_module.o file_transfer.o networking.o user.o sound.o build.o add.o commit.o diff.o patch.o download.o push.o init.o

dit.o: dit.c utils.h client_module.h file_transfer.h networking.h user.h sound.h build.h add.h commit.h diff.h patch.h download.h push.h init.h
	gcc -c -g dit.c 

download.o: utils.h file_transfer.h networking.h user.h sound.h download.c
	gcc -c -g download.c

init.o: utils.h file_transfer.h networking.h user.h sound.h init.c
	gcc -c -g init.c
	
push.o: utils.h file_transfer.h networking.h user.h sound.h push.c
	gcc -c -g push.c

client_module.o: client_module.c utils.h file_transfer.h networking.h user.h sound.h
	gcc -g -c client_module.c 


# Server
server: server.o utils.o file_transfer.o networking.o user.o
	gcc -o server -g server.o utils.o file_transfer.o networking.o user.o

server.o: server.c utils.h file_transfer.h networking.h user.h
	gcc -c -g server.c

# clone
clone: clone.o utils.o
	gcc -o clone -g clone.o utils.o

clone.o: clone.c utils.h
	gcc -c -g clone.c

# patch
patch: patch.o utils.o
	gcc -o patch -g patch.o utils.o

patch.o: patch.c utils.h
	gcc -c -g patch.c
# add
add.o: add.c utils.h patch.h diff.h build.h add.h
	@gcc -c -g add.c

add: patch.o utils.o diff.o add.o build.o 
	@gcc -o add -g add.o utils.o patch.o diff.o build.o
#commit
commit.o: commit.c utils.h patch.h diff.h commit.h
	@gcc -c -g commit.c

commit: patch.o utils.o diff.o commit.o
	@gcc -o commit -g commit.o utils.o patch.o diff.o
#build
build.o: build.c utils.h patch.h diff.h build.h
	@gcc -c -g build.c

build: patch.o utils.o diff.o build.o
	@gcc -o build -g build.o utils.o patch.o diff.o
# push
push: push.o utils.o
	gcc -o push -g push.o utils.o

push.o: push.c utils.h
	gcc -c -g push.c

clean_p:
	rm -f dit server download commit clone add push client
clean_o:
	rm -f *.o
clean: clean_p clean_o