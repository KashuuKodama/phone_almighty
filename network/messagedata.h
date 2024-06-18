#ifdef MESSAGEDATA_H
#else
#define MESSAGEDATA_H
#define MAX_NAME_SIZE 32
#define MAX_MESSAGE_SIZE 32
typedef struct 
{
    char user[MAX_NAME_SIZE];
    char text[MAX_MESSAGE_SIZE];
}MessageData;

#endif