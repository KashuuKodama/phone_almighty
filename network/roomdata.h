#ifdef ROOMDATA_H
#else
#define ROOMDATA_H
#include "messagedata.h"
#define MAX_MESSAGE_COUNT 30
#define MAX_ROOM_NAME_SIZE 15
//トークルームを保持する構造体
typedef struct 
{
    char name[MAX_ROOM_NAME_SIZE];
    char message_length;
    MessageData messages[MAX_MESSAGE_COUNT];

}RoomData;

RoomData Create_RoomData(char* name){
    RoomData room;
    strcpy(room.name,name);
    return room;
}
//トークルームにチャットメッセージを追加
void Room_Add_Message(RoomData* self,MessageData message){
    if(self->message_length==MAX_MESSAGE_COUNT){
        self->message_length=self->message_length-1;
        memmove(self->messages,self->messages+1,self->message_length*sizeof(MessageData));
    }
    self->message_length= self->message_length+1;
    self->messages[self->message_length-1]=message;
}
#endif