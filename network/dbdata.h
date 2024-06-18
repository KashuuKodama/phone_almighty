#ifdef DBDATA_H
#else
#define DBDATA_H
#define MAX_ROOM_COUNT 30
#define MAX_USER_COUNT 10
#include "roomdata.h"
typedef struct 
{
    RoomData rooms[MAX_ROOM_COUNT];
    int statuses[MAX_USER_COUNT];
}DBData;

#endif