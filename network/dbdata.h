#ifdef DBDATA_H
#else
#define DBDATA_H
#define MAX_ROOM_COUNT 10
#define MAX_USER_COUNT 10
#include "roomdata.h"
typedef struct 
{
    //全トークルーム
    RoomData rooms[MAX_ROOM_COUNT];
    //把握しているユーザー
    char registered_users[MAX_USER_COUNT][MAX_NAME_SIZE];
    int statuses[MAX_USER_COUNT];
    
}DBData;
RoomData* DB_Get_Room(DBData* self,int room_id){
    return self->rooms+room_id;
}
#endif