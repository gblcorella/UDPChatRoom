/*
	Bonus Programming Assignment
*/
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "simulate.h"
#include <math.h> 


#define LENGTH 256 
#define SIZE 16	
#define BUFFSIZE 273
#define LOCALHOST "127.0.0.1"
#define USERSIZE 3

int serverSocket;	

struct UDP_DataPacket {
  unsigned short count;
  unsigned short seq;
  char data[SIZE];
};

// Recieve and server struct
struct sockaddr_in serverAdd; 
struct sockaddr_in recieveAddr; 

struct sockaddr_in client01; 
struct sockaddr_in client02; 
struct sockaddr_in client02; 

unsigned short serverPort; /* Server Port */
unsigned int clientLength; 

int failedSend;
int failedSend2;
int failedSend3;

// current chat server message pointer
char *msgPointer;
int recvMsgSize;

// Message Use to Notify
char *sendUser; 
char *msgPointer;

int main(int argc, char *argv[]){

	char msgBuff[BUFFSIZE]; /* Buffer for chat string */
	char sendBuffer[BUFFSIZE + 1]; 
	char userResponse[USERSIZE *(SIZE +1)]; 

	// Users to join
	char* user[USERSIZE]; 
	char TOTALUSER[USERSIZE][SIZE+1];		

	if(argc > 2 || argc < 2){
		printf("Error: Invalid number of arguments \n");
		exit(1);
	}
	
	// Socket Initialize 
	serverPort = atoi(argv[1]); 
	if ((serverSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) <0){
		printf("Error: Could not open Socket... \n Please Try Again on a New Port \n");
		exit(1);
	}
	
	
	// Clear Mem for 3 Users
	memset(&client01, 0, sizeof(client01)); 
	client01.sin_family = AF_INET;
	user[0] = NULL;
	
	memset(&client02, 0, sizeof(client02)); 
	client02.sin_family = AF_INET; 
	user[1] = NULL;
	
	memset(&client02, 0, sizeof(client02)); 
	client02.sin_family = AF_INET;
	user[2] = NULL;
	
		
	memset(&serverAdd, 0, sizeof(serverAdd)); 
	serverAdd.sin_addr.s_addr = htonl(INADDR_ANY); 
	serverAdd.sin_family = AF_INET; 
	// Local Port
	serverAdd.sin_port = htons(serverPort);

	// Binding 
	int bindConfirmation = bind(serverSocket, (struct sockaddr *)&serverAdd, sizeof(serverAdd));
	if (bindConfirmation < 0){
		perror("Error: Could Not Bind Socket \n");
		exit(1);
	}
	printf("\n*******************************\n");
	printf("- Welcome to ChatRoom Server - \n");
	

	printf("Listening on Port: %d\n", serverPort);
	for (;;){
	memset(msgBuff, '\0', sizeof(msgBuff));
	clientLength = sizeof(recieveAddr);
		
	recvMsgSize = recvfrom(serverSocket, msgBuff, BUFFSIZE, 0, (struct sockaddr *)&recieveAddr, &clientLength);
		if(recvMsgSize == -1){
			exit(1);
		} else if ((recieveAddr.sin_port == client01.sin_port) || (recieveAddr.sin_port == client02.sin_port) || (recieveAddr.sin_port == client02.sin_port)){ 
			if(recieveAddr.sin_port== client01.sin_port)
				sendUser = user[0];
			else if( recieveAddr.sin_port == client02.sin_port )
				sendUser = user[1];
			else if( recieveAddr.sin_port == client02.sin_port )
				sendUser = user[2];
			else{
				printf("Could not find user... please make sure username is the first word of message\n");
				// exit(1);
			}
			
			msgBuff[recvMsgSize] = '\0';
			printf("Message Recieved from %s\n", sendUser);
			memset(sendBuffer, '\0', sizeof(sendBuffer)); 
			strcpy(sendBuffer, sendUser); 
			strcat(sendBuffer, "\n");
			msgPointer = strtok(msgBuff, " ");
			
			// Compare the first word and users
			if(!strcmp(msgPointer,(char *) user[0] && (char *) user[0]) ){
				printf("Detected Message for User 1\n");
				failedSend2 = 1;
				failedSend = 0;
				failedSend3 = 1;
			} else if(!strcmp(msgPointer, user[1])&& user[1]){
				printf("Detected Message for User 2\n");
				failedSend = 1;
				failedSend3 = 1;
				failedSend2 = 0;
			} else if(!strcmp(msgPointer, user[2] && user[2])){
				printf("Detected Message for User 3\n");
				failedSend = 1;
				failedSend2 = 1;
				failedSend3 = 0;
			} else {
				// If not specific to a user, send to all chatroom 
				failedSend2 = 0;
				failedSend = 0;
				failedSend3 = 0;
				// Recieve a Segmentation fault fix 
				strcat(sendBuffer, msgPointer); 
				strcat(sendBuffer, " ");
			}
			msgPointer = strtok(NULL, ""); 

			// Copy Message to Chat 
			if (msgPointer) {
				strcat(sendBuffer, msgPointer);
			}

			// EOT, add NULL termination
			sendBuffer[BUFFSIZE + 1] = '\0';
			printf("Sending '%s' to client(s)....\n", sendBuffer);
			if (!strcmp(sendUser, user[0] && user[0])){
				// Find out route of msg
				if(user[1] && !failedSend2){
					if(sendto(serverSocket, sendBuffer, BUFFSIZE, 0, (struct sockaddr *)&client02, sizeof(client02)) <0)
						exit(1);
					printf("Message sent to %s\n", user[1]);
				}
				if(user[2] && !failedSend3){
					if( sendto(serverSocket, sendBuffer, BUFFSIZE, 0, (struct sockaddr *)&client02, sizeof(client02)) <0)
						exit(1);
					printf("Message sent to %s\n", user[2]);

				}
			}
			// Determine where message goes
			else if( user[1] && !strcmp(sendUser, user[1])){ 
				if(user[0] && !failedSend){ 
					if(sendto(serverSocket, sendBuffer, BUFFSIZE, 0, (struct sockaddr *)&client01, sizeof(client01)) < 0){
						exit(1);
					}
					printf("Message sent to %s\n", user[0]);
				}
				if(user[2] && !failedSend3){
					if( sendto(serverSocket, sendBuffer, BUFFSIZE, 0, (struct sockaddr *)&client02, sizeof(client02)) <0)
						exit(1);
					printf("Message sent to %s\nn", user[2]);
				}
			}
			else if( user[2] && !strcmp(sendUser, user[2])){
				if(user[0] && !failedSend){ /* Send to second user if valid */
					if( sendto(serverSocket, sendBuffer, BUFFSIZE, 0, (struct sockaddr *)&client01, sizeof(client01)) <0)
						printf("Error: Failed to send message ... \n");
						exit(1);
					printf("Message sent to %s\n", user[0]);
				}
				if(user[1] && !failedSend2){ 
					if(sendto(serverSocket, sendBuffer, BUFFSIZE, 0, (struct sockaddr *)&client02, sizeof(client02)) <0)
						printf("Error: Could not sent to other users... \n");
					printf("Message sent to %s\n", user[1]);
				}
			}
			else {
				printf("Noone else in chat, message dropped...\n");
			}
		}
		// If we have one user then
		else if (!user[0]||!user[1]||!user[2]){
			
			// Make new Messages
			memset(sendBuffer, '\0', sizeof(sendBuffer)); /* zero out structures */
			memset(userResponse, '\0', sizeof(userResponse));
			strcpy(sendBuffer, msgBuff); 
			strcpy(userResponse, msgBuff);

			strcat(sendBuffer, "\n has entered the chatroom");
			
			// if open, assign to contact
			if (!user[0]){

				memset(TOTALUSER[0], '\0', SIZE); 
				strcpy(TOTALUSER[0], msgBuff);

				client01 = recieveAddr; /*add address*/
				printf("\n %s Joined ChatRoom \n Port:%d \n Address:%d\n", TOTALUSER[0],client01.sin_port, client01.sin_addr.s_addr );
				user[0] = TOTALUSER[0]; /*point to username*/
				// Respond to client for user 0
				int sendTo = sendto(serverSocket, userResponse, 3*(SIZE+1), 0, (struct sockaddr *)&client01, sizeof(client01));
				if(sendTo < 0){
					exit(1);
				}	
			} else if (!user[1]) {

				// Initialize Structure
				strcpy(TOTALUSER[1], msgBuff); 
				memset(TOTALUSER[1], '\0', SIZE); 
			
				client02 = recieveAddr;
				printf("\n %s Joined ChatRoom \n Port:%d \n Address:%d\n", TOTALUSER[1], client02.sin_port, client02.sin_addr.s_addr );
				user[1] = TOTALUSER[1]; 

				// Send back to client
				strcat(userResponse, "\n");
				strcat(userResponse, TOTALUSER[0]);

				if(sendto(serverSocket, userResponse, 3*(SIZE+1), 0, (struct sockaddr *)&client02, sizeof(client02)) < 0)
					exit(1);
				if(sendto(serverSocket, sendBuffer, SIZE+22, 0, (struct sockaddr *)&client01, sizeof(client01)) < 0){
					printf("Failed to send to ... \n");
				}
				printf("User Notified of New User\n", user[0]);
			}
			else if (!user[2])
			{	
				memset(TOTALUSER[2], '\0', SIZE); 
				strcpy(TOTALUSER[2], msgBuff); 
				client02 = recieveAddr; 
				printf("\n %s Joined ChatRoom \n Port:%d \n Address:%d\n", TOTALUSER[2],client02.sin_port, client02.sin_addr.s_addr);
				user[2] = TOTALUSER[2]; 
				
				// Respond to Client 
				strcat(userResponse, "\n");
				strcat(userResponse, TOTALUSER[0]);
				strcat(userResponse, "\n");
				strcat(userResponse, TOTALUSER[1]);		
			} else {
				exit(1);
			}
		}
		else{
			printf("Cannot add more users... full chat. 3/3 \n");
			printf("Chatroom: %s, %s, %s.\n", user[0], user[1], user[2]);		
		}
	}
}
