#ifdef MESSAGEDATA_H
#else
#define MESSAGEDATA_H
#define MAX_NAME_SIZE 32
#define MAX_MESSAGE_SIZE 32
typedef struct 
{
    char user[MAX_NAME_SIZE];
    char message[MAX_MESSAGE_SIZE];
}MessageData;

#endif