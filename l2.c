// l2.c
#include <math.h>
// Interface to Layer 1 
extern int l1_read(char* b);
extern int l1_write(char b);


int l2_read(char* buffer, int maxlength)
{
    int messageLength = 0;
    char header[2];
    
    for (int i = 0; i<2; i++){
        if (l1_read(&header[i]) == 0){
            return -1;
        }
    }

   messageLength = ((int) header[0]) * pow(2,8);

    if (messageLength > maxlength){
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

    char header[2];
    header[0] = (char) (length/pow(2,8));
    header[1] = (char) (fmod(length, pow(2,8)));

    for (int i = 0; i<2; i++){
        if (l1_write(header[i]) == 0){
            return -1;
        }
    }

    for (int i = 0; i<length; i++){
        if (l1_write(buffer[i]) == 0){
            return -1;
        }
    }

    return length;
}
