// l4.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Interface to Layer 3
extern int l3_read(char* buffer, int maxlength);
extern int l3_write(char* buffer, int length);


int l4_read(char* name, int* nameLength, char* value, int* valueLength)
{
    char buff[250];
    int length = l3_read(buff, 250);
    if (length == -1){
        return 0;
    }
    //printf("length: ");
    //printf("%d\n",length);
    int i = 1;
    while(buff[i]!=','){
        if (i >= *nameLength){
            //printf("namelength");
            //printf("%d\n",i);
            return 0;
        }
        *(name+i-1) = buff[i];
        i++;
    }

    i++;
    int j = 0;
    while(buff[i]!=')'){
        if ( j >= *valueLength) 
        {
            // printf("valueLength");
            // printf("%d\n",j);
            return 0;
        }
        if (i>=length){
            // printf("length");
            // printf("%d\n",i);
            return 0;
        }
        value[j] = buff[i];
        i++;
        j++;
    }
    return 1;
}

int l4_write(char* name, int nameLength, char* value, int valueLength)
{
    char buff[nameLength+valueLength+3];
    strcpy(buff,"(");
    strcat(buff, name);
    strcat(buff, ",");
    strcat(buff, value);
    strcat(buff,")");
    if (l3_write(buff, nameLength+valueLength+3) ==-1){
        return 0;
    }

    return 1;
}
