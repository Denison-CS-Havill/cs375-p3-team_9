// l3.c

// Interface to Layer 2
extern int l2_read(char* buffer, int maxlength);
extern int l2_write(char* buffer, int length);

u_long checksum(char* buffer, int length){
    register u_long sum = 0;

    while (length--)
    {
        u_short bufferi = (u_short) *buffer++;
        sum += bufferi;
        if (sum & 0xFFFF0000)
        {
            /* carry occurred, so wrap around */
            sum &= 0xFFFF;
            sum++;
        }
    }
    return ~(sum & 0xFFFF);
}


int l3_read(char* buffer, int maxlength)
{
    char header[4];
    short convert;
    u_long checksum = 0;
    for(int i = 0; i<4; i++){
        if (l2_read(&header[i]) == -1){
            return -1;
        }
        convert = (short) header[i];
        checksum += convert << (8*4*(3-i));
    }
    checksum = ntohl(checksum);
    

}


int l3_write(char* buffer, int length)
{
    u_long checksumOutput = htonl(checksum(buffer, length));

    char header[4];
    u_short converting;
    u_long convert_l;
    for (int i = 0; i<length; i++){
        convert_l = fmod((checksumOutPut >> (8*4*i)) , pow(2,8*4) );
        converting = (short) convert_l;
        header[3-i] = (char) converting;
    }
    
    char* newBuffer;

    newBuffer = malloc(strlen(header)+1+length);
    strcpy(newBuffer, header);
    strcat(newBuffer, buffer);

    for (int i=0; i<strlen(newBuffer); i++){
        if(l2_write(newBuffer[i]) == -1){
            return -1;
        }
    }

    return length;
}
