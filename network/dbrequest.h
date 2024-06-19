#ifdef DBREQUEST_H
#else
#define DBREQUEST_H
#include "roomdata.h"
#include "messagedata.h"
#define MAX_REQUESTS_COUNT 3
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
//同時に複数リクエストを送る
typedef struct 
{
    int requests_length;
    DBRequest requests[MAX_REQUESTS_COUNT];
}DBRequests;

//チャットメッセージの追加
void Create_Request_Chat(DBRequests* self,char user_id,char room_id,char* message){
    DBRequest* request=self->requests+self->requests_length;
    request->user_id=user_id;
    request->room_id=room_id;
    strcpy(request->message,message);
    strcpy(request->method,"CHAT");
    self->requests_length++;
}
//トークルームに参加
void Create_Request_Join(DBRequests* self,char user_id,char room_id){
    DBRequest* request=self->requests+self->requests_length;
    request->user_id=user_id;
    request->room_id=room_id;
    strcpy(request->method,"JOIN");
    self->requests_length++;
}
//ユーザーを追加
void Create_Request_Add_User(DBRequests* self,UserData user){
    DBRequest* request=self->requests+self->requests_length;
    memcpy(request->message,&user,sizeof(UserData));
    strcpy(request->method,"ADD_USER");
    self->requests_length++;
}
//ルームを追加
void Create_Request_Add_Room(DBRequests* self,char* name,char icon){
    DBRequest* request=self->requests+self->requests_length;
    request->user_id=icon;
    strcpy(request->message,name);
    strcpy(request->method,"ADD_ROOM");
    self->requests_length++;
}
//ルームを編集
void Create_Request_Edit_Room(DBRequests* self,char room_id,RoomData room){
    DBRequest* request=self->requests+self->requests_length;
    request->user_id=room.icon;
    request->room_id=room_id;
    strcpy(request->message,room.name);
    strcpy(request->method,"EDIT_ROOM");
    self->requests_length++;
}
//ユーザーを編集
void Create_Request_Edit_User(DBRequests* self,char user_id,UserData user){
    DBRequest* request=self->requests+self->requests_length;
    request->user_id=user_id;
    memcpy(request->message,&user,sizeof(UserData));
    strcpy(request->method,"EDIT_USER");
    self->requests_length++;
}
//通話を開始
void Create_Request_Phone(DBRequests* self,int room_id){
    DBRequest* request=self->requests+self->requests_length;
    request->room_id=room_id;
    strcpy(request->method,"PHONE");
    self->requests_length++;
}
//オーディオステータスの反映
void Create_Request_Status(DBRequests* self,char user_id,AudioStatus status){
    DBRequest* request=self->requests+self->requests_length;
    request->user_id=user_id;
    memcpy(request->message,&status,sizeof(AudioStatus));
    strcpy(request->method,"STATUS");
    self->requests_length++;
}
#endif