Gabriel Corella

Protocol Design:
		•A user starts the chat program sending its username to the server.  
		The server then returns the list of theusernames of current users to the user, which should be displayed at the user side.
		
		•The user can then choose to send messages to either one designated user through unicast (i.e., private chat)or all the other users through broadcast.
		
		•The chat program needs to allow a user to type messages, which should be conveyed to the other end(s) anddisplayed at the other end(s).

List of relevant files:
	client.c
	server.c
	makefile
	loremipsum.txt
	simulate.h 
	
Compilation Instructions:
	- Use the makefile for this program with the commands:
			make all 
			make clean

Running Instructions:
	There are two executables generated: client and server
	- In order to run server, a port number must be specified. Use:
		./server <port number
		Example. ./server 49152

	- In order to run client use a host IP, username, and server port number must be specified. Use command:
		./client <IP address> <username> <port number>
		Example: ./client 127.0.0.1 gabe 49152
		In another terminal do:
			- ./client 127.0.0.1 <user> 49152 to join chatroom 

* Please note there is a bug when sending messages without username specified, I receive a segmentation fault * 
