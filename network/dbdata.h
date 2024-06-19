#ifdef DBDATA_H
#else
#define DBDATA_H
#define MAX_ROOM_COUNT 10
#define MAX_USER_COUNT 8
#include "roomdata.h"
#include "audiostatus.h"
typedef struct 
{
    //全トークルーム
    RoomData rooms[MAX_ROOM_COUNT];
    char rooms_length;
    //把握しているユーザー
    char registered_users_length;
    UserData registered_users[MAX_NAME_SIZE];
    //ユーザーのaudioステータス
    AudioStatus statuses[MAX_USER_COUNT];
    //通話中のトーク
    char phone_room_id;
    //local dbにアクセスするユーザー
    char user_id;
    //ユーザーがいるルーム
    char current_room_id;
    
    
}DBData;
RoomData* DB_Get_Room(DBData* self,int room_id){
    return self->rooms+room_id;
}
RoomData* DB_Get_User(DBData* self,int room_id){
    return self->rooms+room_id;
}
//ディスクからdbを読み込む(サーバー側の処理)
void DB_Load(DBData* self,char* path){
    FILE* fd=fopen(path,"rb");
    fread(self,sizeof(DBData),1,fd);
    fclose(fd);
}
//ディスクにdbを書き込む(サーバー側の処理)
void DB_Save(DBData* self,char* path){
    FILE* fd=fopen(path,"wb");
    fwrite(self,sizeof(DBData),1,fd);
    fclose(fd);
}
//ユーザーを検索してIDを取得
int DB_Find_User(DBData* self,char* name){
    for(int i=0;i<self->registered_users_length;i++){
        if(strcmp(self->registered_users[i].name,name)==0){
            return i;
        }
    }
    return -1;
}
//ユーザーを追加してIDを取得
int DB_Add_User(DBData* self,UserData user){
    //古参は追い出す
    if(self->registered_users_length==MAX_USER_COUNT){
        self->registered_users_length--;
        memmove(self->registered_users,self->registered_users+1,(MAX_USER_COUNT-1)*sizeof(int));
    }
    self->registered_users[self->registered_users_length]=user;
    self->registered_users_length++;
    return self->registered_users_length-1;
}
//ルームを追加
int DB_Add_Room(DBData* self,char* name,char icon){
    //古参は追い出す
    if(self->rooms_length==MAX_ROOM_COUNT){
        self->rooms_length--;
        memmove(self->rooms,self->rooms+1,(MAX_ROOM_COUNT-1)*sizeof(RoomData));
    }
    strcpy(self->rooms[self->rooms_length].name,name);
    self->rooms[self->rooms_length].icon=icon;
    self->rooms_length++;
    return self->rooms_length-1;
}
//ユーザーを編集
void DB_Edit_User(DBData* self,char user_id,UserData user){
    self->registered_users[user_id]=user;
}
//ルームを編集
void DB_Edit_Room(DBData* self,char room_id,char* name,char icon){
    strcpy(self->rooms[room_id].name,name);
    self->rooms[room_id].icon=icon;
}
#endif