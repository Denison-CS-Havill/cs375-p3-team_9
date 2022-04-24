// l2.c
#include <math.h>
#include <arpa/inet.h>
// Interface to Layer 1 
extern int l1_read(char* b);
extern int l1_write(char b);


int l2_read(char* buffer, int maxLength)
{
    int messageLength = 0;
    char header[2];
    uint16_t messageLengthPreConvert;
    
    for (int i = 0; i < 2; i++){
        if (l1_read(&header[i]) == 0){
            return -1;
        }
    }
    
    memcpy(&messageLengthPreConvert, header, sizeof(uint16_t));
    messageLength = (int)ntohs(messageLengthPreConvert);
    
    
    if (messageLength > maxLength){
        return -1;
    }

    for (int i = 0; i<messageLength; i++){
        
        if (l1_read(&buffer[i]) == 0){
            return -1;
        }

    }
    
    return messageLength;
}

int l2_write(char* buffer, int length)
{
    if (length > 1024){
        return -1;
    }

    char header_1,header_2;
    short int converting;
    short int convert;
    convert = (short) length;
    converting = htons(convert);
    converting = converting>>8;
    header_1 = converting+'0';
    if (l1_write(header_1) == 0){
            return -1;
    }
    
    converting = htons(convert);
    header_2 = converting+'0';
    
    if (l1_write(header_2) == 0){
            return -1;
    }
    
    for (int i = 0; i<length; i++){
        if (l1_write(buffer[i]) == 0){
            return -1;
        }
    }

    return length;
}
