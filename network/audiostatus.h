#ifdef AUDIOSTATUS_H
#else
#define AUDIOSTATUS_H
//ミュートなどを管理する構造体
typedef struct 
{
    //ミュートしてたら1
    int muted;
    //通話に参加してたら1 
    int joined;
}AudioStatus;

#endif