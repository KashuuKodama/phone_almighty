#ifdef DBREQUEST_H
#else
#define DBREQUEST_H
#include "roomdata.h"
#include "messagedata.h"
typedef struct 
{
    //変更操作
    char method[10];
    //変更者
    int user_id;
    //変更先トークルーム
    int room_id;
    //メッセージ
    char message[MAX_MESSAGE_SIZE];
}DBRequest;
//チャットメッセージの追加
void Create_Request_Add(DBRequest* self,int user_id,int room_id,char* message){
    self->user_id=user_id;
    self->room_id=room_id;
    strcpy(self->message,message);
    strcpy(self->method,"ADD");
}
//トークルームに参加
void Create_Request_Join(DBRequest* self,int user_id,int room_id){
    self->user_id=user_id;
    self->room_id=room_id;
    strcpy(self->method,"JOIN");
}
#endif