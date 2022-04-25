// l4.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Interface to Layer 3
extern int l3_read(char* buffer, int maxlength);
extern int l3_write(char* buffer, int length);


int l4_read(char* name, int* nameLength, char* value, int* valueLength)
{
    int maxLength = 2 * sizeof(uint16_t) + *nameLength + *valueLength;
    char buff[maxLength];
    int length = l3_read(buff, maxLength);
    
    
    if (length == -1){
        return 0;
    }
    
    uint16_t nameLengthPreConverting;
    uint16_t valueLengthPreConverting;
    
    memcpy(&nameLengthPreConverting, buff, sizeof(uint16_t));
    memcpy(&valueLengthPreConverting, buff + sizeof(uint16_t), sizeof(uint16_t));

    *nameLength = (int)  ntohs(nameLengthPreConverting);
    *valueLength = (int) ntohs(valueLengthPreConverting);

    memcpy(name, buffer + 2 * sizeof(uint16_t), *nameLength);
    memcpy(value, buffer + 2 * sizeof(uint16_t) + *nameLength, *valueLength);
    
    
    return 1;
}

int l4_write(char* name, int nameLength, char* value, int valueLength)
{
    
    
    char buff[2 * sizeof(uint16_t) + nameLength + valueLength];
    int buffLength = 0;
    
    uint16_t nameLengthPostConverting = htons((uint16_t)nameLength);
    uint16_t valueLengthPostConverting = htons((uint16_t)valueLength);

    memcpy(buff, &nameLengthPostConverting, sizeof(uint16_t));
    memcpy(buff + sizeof(uint16_t), &valueLengthPostConverting, sizeof(uint16_t));
    memcpy(buff + 2 * sizeof(uint16_t), name, nameLength);
    memcpy(buff + 2 * sizeof(uint16_t) + nameLength, value, valueLength);

    buffLength = 2 * sizeof(uint16_t) + nameLength + valueLength;
    
    if (l3_write(buff, buffLength) == -1){
        return 0;
    }

    return 1;
}
