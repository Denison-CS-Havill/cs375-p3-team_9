// l3.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Interface to Layer 2
extern int l2_read(char* buffer, int maxlength);
extern int l2_write(char* buffer, int length);

unsigned char checksum(char* buffer, int length){
    unsigned char sum;
    sum = *buffer;
    for (int i = 1; i<length; i++){
        sum = (char) (sum + *(buffer+i));
    }
    return sum;
}

int l3_read(char* buffer, int maxlength)
{
    char messageWithHeader[maxlength];
    int messageLength = l2_read(messageWithHeader, maxlength);
    if ( (messageLength == -1)|(messageLength > maxlength) ){
        return -1;
    } 

    char cheksum = checksum(messageWithHeader, messageLength);
    char checksumOfMessage = (cheksum & 0xFF) ;

    if (checksumOfMessage == 0){
        for (int i = 1; i < messageLength; i++){
            *(buffer+i-1) = messageWithHeader[i];
        }
        //free(messageWithHeader);
        return messageLength;
    } else {
        //printf("checksum: ");
        //printf("%s\n",messageWithHeader);
        return -1;
    }
}


int l3_write(char* buffer, int length)
{
    unsigned char checksumOutput = checksum(buffer, length);
    checksumOutput = (char) ((-checksumOutput) & 0xFF);

    char header;
    header = checksumOutput;

    char newBuffer[1+length];
    newBuffer[0] =  header;
    strcat(newBuffer, buffer);

    if(l2_write(newBuffer, length+1) == -1){
        return -1;
    }

    return length;
}
