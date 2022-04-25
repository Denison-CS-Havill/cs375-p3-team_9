// l3.c
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

// Interface to Layer 2
extern int l2_read(char* buffer, int maxlength);
extern int l2_write(char* buffer, int length);

unsigned short checksum(char* buffer, int length){
   unsigned short *p;
   unsigned int sum = 0;
   p = (unsigned short *) buffer;
   for(int i=0; i < length/2; i++){
      sum = sum + htons(p[i]);
      if (sum&0x10000) sum = (sum&0xFFFF)+1;
   }
   return (unsigned short)0xFFFF-sum;
}


int l3_read(char* buffer, int maxlength)
{
    char *messageWithHeader;
    int messageLength = l2_read(messageWithHeader, maxlength);
   
    if ( (messageLength == -1) || (messageLength > maxlength) ){
        return -1;
    } 

    unsigned short checksumOfMessage = checksum(messageWithHeader, messageLength);

    if (checksumOfMessage == 0){
        for (int i = 1; i < messageLength; i++){
            *(buffer+i-1) = messageWithHeader[i];
        }
        free(messageWithHeader);
        return messageLength;
    } else {
        return -1;
    }
}



int l3_write(char* buffer, int length)
{
    
    unsigned short checksumOutput = checksum(buffer, length);
    char header[2];
    uint16_t messageLengthPreConvert = htons((uint16_t)checksumOutput);
    memcpy(header, &messageLengthPreConvert, sizeof(uint16_t));
    int newLength = length + sizeof(header);
    char newBuffer[newLength];
    
    memcpy(newBuffer, header, sizeof(header));
    memcpy(newBuffer+sizeof(header), buffer, length);
    

    if(l2_write(newBuffer, newLength) == -1){
        return -1;
    }

    return length;
}


