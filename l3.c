// l3.c

// Interface to Layer 2
extern int l2_read(char* buffer, int maxlength);
extern int l2_write(char* buffer, int length);

// unsigned char checksum(char* buffer, int length){
//     unsigned char sum;
//     sum = *buffer;
//     for (int i = 1; i<length; i++){
//         sum = (char) (sum + *(buffer+i));
//     }
//     return sum;
// }

unsigned short checksum(char* buffer, int count){
    register unsigned long sum = 0;

    while(count--){
        sum += *buffer++;
        if (sum & 0xFFFF0000){
            sum &= 0xFFFF;
            sum++;
        }
    }
    return ~(sum & 0xFFFF);
}


// int l3_read(char* buffer, int maxlength)
// {
//     char *messageWithHeader;
//     int messageLength = l2_read(messageWithHeader, maxlength);
//     if ( (messageLength == -1)|(messageLength > maxlength) ){
//         return -1;
//     } 

//     char checksumOfMessage = checksum(messageWithHeader, messageLength) &= 0x7F ;

//     if (checksumOfMessage == 0){
//         for (int i = 1; i < messageLength; i++){
//             *(buffer+i-1) = messageWithHeader[i];
//         }
//         free(messageWithHeader);
//         return messageLength;
//     } else {
//         return -1;
//     }
// }

int l3_read(char* buffer, int maxlength){
    char *messageWithHeader;
    int messageLength = l2_read(messageWithHeader, maxlength);
    if ( (messageLength == -1)|(messageLength > maxlength) ){
        return -1;
    }

    unsigned short checksum = checksum(messageWithHeader, messageLength);
    if (checksum == 0){
        for (int i = 1; i < messageLength; i++){
            *(buffer+i-1) = messageWithHeader[i];
        }
        return messageLength;
    } else {
        return -1;
    }
}

// int l3_write(char* buffer, int length)
// {
    
//     unsigned char checksumOutput = checksum(buffer, length);
//     checksumOutput = (char) ((-checksumOutput) & 0x7F);

//     char header[1];
//     header[0] = checksumOutput;

//     char* newBuffer;

//     newBuffer = malloc(strlen(header)+1+length);
//     strcpy(newBuffer, header);
//     strcat(newBuffer, buffer);

//     if(l2_write(newBuffer, length+1) == -1){
//         return -1;
//     }

//     return length;
// }

int l3_write(char* buffer, int length){
    unsigned short checksum = htons(checksum(buffer, length));
    char header[2];

    memcpy(&header, &checksum, sizeof(header));

    char* newBuffer;

    newBuffer = malloc(strlen(header)+1+length);
    strcpy(newBuffer, header);
    strcat(newBuffer, buffer);

    if(l2_write(newBuffer, length+1) == -1){
        return -1;
    }

    return length;
}
