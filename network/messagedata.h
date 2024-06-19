#ifdef MESSAGEDATA_H
#else
#define MESSAGEDATA_H
#define MAX_NAME_SIZE 15
#define MAX_MESSAGE_SIZE 32
//チャットメッセージを保持する構造体
typedef struct 
{
    int user_id;
    char text[MAX_MESSAGE_SIZE];
}MessageData;
MessageData Create_MessageData(int user_id,char* text){
    MessageData message;
    message.user_id=user_id;
    strcpy(message.text,text);
    return message;
}
#endif