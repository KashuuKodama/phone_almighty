#ifdef USERDATA_H
#else
#define USERDATA_H
#define MAX_NAME_SIZE 15
#include <string.h>
//チャットメッセージを保持する構造体
typedef struct 
{
    char name[MAX_NAME_SIZE];
}UserData;
void User_SetName(UserData* self,char* name){
    strcpy(self->name,name);
}
#endif