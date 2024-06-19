#ifdef USERDATA_H
#else
#define USERDATA_H
#define MAX_NAME_SIZE 15
#include <string.h>
//チャットメッセージを保持する構造体
typedef struct 
{
    char name[MAX_NAME_SIZE];
    int icon;
}UserData;
UserData CreateUser(char* name,int icon){
    UserData user;
    strcpy(user.name,name);
    user.icon=icon;
    return user;
}
Texture2D* User_Get_Icon(UserData* self){
    char path[30];
    sprintf(path,"textures/icons/icon%d.txt",self->icon);
    return open_texture(path);
}
#endif