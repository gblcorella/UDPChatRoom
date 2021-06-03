/*
	Gabriel Corella 
	Bonus Programming Asssignment 
*/
#include <stdio.h> 
#include <sys/socket.h> 
#include <string.h>
#include <arpa/inet.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
 #include "simulate.h"

#define LENGTH 256	// Message max
#define USERMAX 16 	// Usernames are usually max 16 characters
#define BUFFSIZE 273
#define USERSIZE 3
#define LOCALHOST "127.0.0.1"

// Client socket to listen on 
int clientSocket; 

struct sockaddr_in serverAddr; 
struct sockaddr_in fromAddr; 
struct timeval timestamp;

unsigned short serverPort;	
unsigned int serverSize;
unsigned int localSize;

unsigned int usernameStringLen;
unsigned int chatLength;	
	
	

int recvMsgSize;
int userCount;
int selectVal;

char *recvChatMsg;
int main(int argc, char *argv[])
{
	char *serverIPAddr;	
	char *sendUser;	
			
	char userResponse[3*USERMAX +1]; /* Buffer to hold all user returned by server */
	// Hold message contents
	char msgBuff[BUFFSIZE];	/* Buffer to hold message contents sent by server */
	char *username; 
	
	
	// Users to join
	char *user[USERSIZE]; /*array of pointers to all user */
	char USERNAMES[USERSIZE][USERMAX]; /*array of all user */
	 	
	// User Response Len
	int userResLength; 
	fd_set rfds; 
	 

	// Error Checking for correct arguments
	if((argc<3) || (argc>4)){
		printf("Error: Invalid Amount of Arguments... Please see README \n");
		exit(1);
	}
	serverIPAddr = argv[1];
	username = argv[2];

	if ((usernameStringLen = strlen(username)) > USERMAX){
		printf("Invalid User \n");
		exit(1);
	}

	if (argc == 4){
		serverPort = atoi(argv[3]);
	} else {
		serverPort = 7;
	}
	printf("Looking on Port: %d\n", serverPort);

	// Socket 
	clientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(clientSocket <0)
		printf("Error: Failed to create socket \n");
		exit(1);
	
	user[0] = NULL;
	user[1] = NULL;
	user[2] = NULL;

	// Initialize Server Address like previous assignments
	memset(&serverAddr, 0, sizeof(serverAddr)); 
	serverAddr.sin_addr.s_addr = inet_addr(serverIPAddr);
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_port = htons(serverPort); 
	
	// Notify username 
	if (sendto(clientSocket, username, usernameStringLen, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != usernameStringLen){
		printf("Send-to Invalid, terminating program \n");
		exit(1);
	}
	localSize = sizeof(serverAddr);
	userResLength = recvfrom(clientSocket, userResponse, 3*USERMAX+1, 0, (struct sockaddr *)&fromAddr, &localSize);
	if (userResLength < usernameStringLen){
		exit(1);
	}
	
	// EOT packet similar, nullify the end of the packet 
	userResponse[userResLength] = '\0';
	userCount= 0;
	username = strtok(userResponse, "\n");
	while(username != NULL){
		memset(USERNAMES[userCount], '\0', USERMAX);
		strcpy(USERNAMES[userCount], username);
		user[userCount] = USERNAMES[userCount];
		userCount++;
		if (userCount== 3)
			break;
		username = strtok(NULL, "\n");
	}
	userCount=0;
	printf("Username: %s\n", user[0]); 
	if(!user[1])
		printf("No one else is in chatroom.\n");
	else {
		printf("Chatroom includes: %s", user[1]);
		if(user[2]){
			printf(", %s\n", user[2]);
		} else {
			printf("\n");
		}
	}
	printf("In order to chat, say user name and then message, or re read the README\n");
	printf("To send a file, include 'fileTransfer' at the beginning of the message.\n\n");
	// timestamp
	timestamp.tv_usec = 0;
	timestamp.tv_sec = 0; 
	FD_ZERO(&rfds);

	for(;;){
		FD_SET(0, &rfds); 
		FD_SET(clientSocket, &rfds); 
		selectVal = select(clientSocket+1, &rfds, NULL, NULL, &timestamp);
		
		if(selectVal == -1) {
			// close and return 
			close(clientSocket);
			exit(1);
		}
		else if (selectVal) {
			memset( msgBuff, '\0', sizeof(msgBuff) );
			if( FD_ISSET(0, &rfds) ) { 
				
				read(0, msgBuff, BUFFSIZE);
				// NULL Termination 
				msgBuff[BUFFSIZE] = '\0'; 
				chatLength = strlen(msgBuff);
				msgBuff[chatLength] ='\0';
				
				if( sendto(clientSocket, msgBuff, chatLength, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr))!= chatLength)
					printf("Error: Could not be sent to server.. \n");
					
				printf("%s: %s", user[0], msgBuff); 
			}
			else if ( FD_ISSET(clientSocket, &rfds) ) { /* we have received message from server */
				if((recvMsgSize = recvfrom(clientSocket, msgBuff, USERMAX+LENGTH+1, 0, (struct sockaddr *)&fromAddr, &localSize)) <0)
					exit(1);
				else if( serverAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr && recvMsgSize > 0){
					fprintf(stderr, "Error: received a packet from unknown source.\n");
					exit(1);
				} else {					
					// Null EOT 
					msgBuff[BUFFSIZE] = '\0';
					sendUser = strtok(msgBuff, "\n");
					recvChatMsg = strtok(NULL, "\n"); 
					/* print username of sender and received chat message */
					printf("%s: %s\n", sendUser, recvChatMsg);
				}
			}
		}
		FD_ZERO(&rfds);
	}
	close(clientSocket);
	exit(1);
}
