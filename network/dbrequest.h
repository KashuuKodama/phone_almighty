#ifdef DBREQUEST_H
#else
#define DBREQUEST_H
#include "roomdata.h"
#include "messagedata.h"
#define MAX_REQUEST_MESSAGE_SIZE 32
typedef struct 
{
    //変更操作
    char method[10];
    //変更者
    char user_id;
    //変更先トークルーム
    char room_id;
    //メッセージ
    char message[MAX_REQUEST_MESSAGE_SIZE];
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
void Create_Request_Add_User(DBRequest* self,UserData user){
    memcpy(self->message,&user,sizeof(UserData));
    strcpy(self->method,"ADD_USER");
}
//ユーザーを編集
void Create_Request_Edit_User(DBRequest* self,char user_id,UserData user){
    self->user_id=user_id;
    memcpy(self->message,&user,sizeof(UserData));
    strcpy(self->method,"EDIT_USER");
}
#endif