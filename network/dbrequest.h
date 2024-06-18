#ifdef DBREQUEST_H
#else
#define DBREQUEST_H
#include "roomdata.h"
#include "messagedata.h"
typedef struct 
{
    char method[10];
    char user[MAX_NAME_SIZE];
    int room;
    char message[MAX_MESSAGE_SIZE];
}DBRequest;

#endif