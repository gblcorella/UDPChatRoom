#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>    

// Helper Functions

int simulateLoss(double packetLoss);
int simulateACKLoss(float loss);

/*
    If this function returns 0, thepacket is transmitted normally, but if it returns 1, 
    the packet is not transmitted and the server simply continues toits next action just 
    as if the packet had been transmitted.
*/
int simulateLoss(double packetLoss){
  double random = (double)rand() / (double)RAND_MAX;
  if(random < packetLoss){
    return 1;
  }
  return 0;
}
/*
     If this functionreturns 0, the ACK is transmitted normally, but if it returns 1, 
     the ACK is not transmitted and the client simplycontinues to its next action just 
     as if the ACK had been transmitted.
*/
int simulateACKLoss(float loss){
  double ratioLoss = ((double)rand() / (double)RAND_MAX);
  if(ratioLoss < loss){
    return 1;
  }
  return 0;
}