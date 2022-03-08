// l3.c

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
    
    int messageLength = l2_read(buffer, maxlength);
    if ( (messageLength == -1)|(messageLength > maxlength) ){
        return -1;
    } 

    char checksumOfMessage = checksum(buffer, messageLength) &= 0x7F ;

    if (checksumOfMessage == 0){
        return messageLength;
    } else {
        return -1;
    }
}


int l3_write(char* buffer, int length)
{
    unsigned char checksumOutput = checksum(buffer, length);
    checksumOutput = (char) ((-checksumOutput) & 0x7F);

    char header[1];
    header[0] = checksumOutput;

    char* newBuffer;

    newBuffer = malloc(strlen(header)+1+length);
    strcpy(newBuffer, header);
    strcat(newBuffer, buffer);

    if(l2_write(newBuffer, length+1) == -1){
        return -1;
    }

    return length;
}
