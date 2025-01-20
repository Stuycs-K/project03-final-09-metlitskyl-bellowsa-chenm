.PHONY: compile clean clean_f clean_o clean_p

compile: dit server 
# FINAL OBJECTS

#dit
dit: dit.o utils.o file_transfer.o networking.o user.o sound.o build.o status.o add.o commit.o diff.o patch.o download.o push.o init.o revert.o
	@gcc -o dit -g dit.o utils.o file_transfer.o networking.o user.o sound.o build.o status.o add.o commit.o diff.o patch.o download.o push.o init.o revert.o
# Server
server: server.o utils.o file_transfer.o networking.o user.o
	@gcc -o server -g server.o utils.o file_transfer.o networking.o user.o

#O FILES
utils.o: utils.c
	@gcc -c -g utils.c

sound.o: sound.c
	@gcc -c -g sound.c

file_transfer.o: file_transfer.c
	@gcc -c -g file_transfer.c

networking.o: networking.c networking.h utils.h 
	@gcc -c -g networking.c

user.o: user.c
	@gcc -c -g user.c

dit.o: dit.c utils.h file_transfer.h networking.h user.h sound.h build.h add.h commit.h diff.h patch.h download.h push.h init.h status.h
	@gcc -c -g dit.c 

download.o: utils.h file_transfer.h networking.h user.h sound.h download.c
	@gcc -c -g download.c

init.o: utils.h file_transfer.h networking.h user.h sound.h init.c
	@gcc -c -g init.c
	
push.o: utils.h file_transfer.h networking.h user.h sound.h push.c
	@gcc -c -g push.c

server.o: server.c utils.h file_transfer.h networking.h user.h
	@gcc -c -g server.c

clone.o: clone.c utils.h
	@gcc -c -g clone.c

diff.o: diff.c utils.h
	@gcc -c -g diff.c

patch.o: patch.c utils.h
	@gcc -c -g patch.c

add.o: add.c utils.h patch.h diff.h build.h add.h status.h
	@gcc -c -g add.c

commit.o: commit.c utils.h patch.h diff.h commit.h
	@gcc -c -g commit.c

revert.o: utils.h diff.h build.h revert.c
	@gcc -c -g revert.c

build.o: build.c utils.h patch.h diff.h build.h status.h
	@gcc -c -g build.c

status.o: status.c utils.h patch.h diff.h status.h
	@gcc -c -g status.c

clean_f:
	rm -rf .client_config
	rm -rf server_data/
clean_p:
	rm -f dit server download commit clone add push client
clean_o:
	rm -f *.o
clean: clean_p clean_o clean_f