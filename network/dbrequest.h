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
    char user_id;
    //変更先トークルーム
    char room_id;
    //メッセージ
    char message[MAX_MESSAGE_SIZE];
}DBRequest;
//チャットメッセージの追加
void Create_Request_Chat(DBRequest* self,char user_id,char room_id,char* message){
    self->user_id=user_id;
    self->room_id=room_id;
    strcpy(self->message,message);
    strcpy(self->method,"CHAT");
}
//トークルームに参加
void Create_Request_Join(DBRequest* self,char user_id,char room_id){
    self->user_id=user_id;
    self->room_id=room_id;
    strcpy(self->method,"JOIN");
}
//ユーザーを追加
void Create_Request_Add_User(DBRequest* self,char* name){
    strcpy(self->message,name);
    strcpy(self->method,"ADD_USER");
}
#endif