#ifdef DBDATA_H
#else
#define DBDATA_H
#define MAX_ROOM_COUNT 30
#include "roomdata.h"
typedef struct 
{
    RoomData rooms[MAX_ROOM_COUNT];
}DBData;

#endif