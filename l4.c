// l4.c

// Interface to Layer 3
extern int l3_read(char* buffer, int maxlength);
extern int l3_write(char* buffer, int length);


int l4_read(char* name, int* nameLength, char* value, int* valueLength)
{
    char buff[1000];
    int length = l3_read(buff, 1000);
    int i = 1;
    while ( (buff[i]!=',') )
}

int l4_write(char* name, int nameLength, char* value, int valueLength)
{
    char buff[nameLength+valueLength+3];
    strcpy(buff,'(');
    strcat(buff, name);
    strcat(buff, ',');
    strcat(buff, value);
    strcat(buff,')');
    if (l3_write(buff, nameLength+valueLength+3) ==-1){
        return 0;
    }

    return 1;
}
