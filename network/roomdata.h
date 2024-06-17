#ifdef ROOMDATA_H
#else
#define ROOMDATA_H
#include "messagedata.h"
#define MAX_MESSAGE_COUNT 30
#define MAX_ROOM_NAME_SIZE 30
typedef struct 
{
    char name[MAX_ROOM_NAME_SIZE];
    MessageData messages[MAX_MESSAGE_COUNT];
}FileData;

#endif