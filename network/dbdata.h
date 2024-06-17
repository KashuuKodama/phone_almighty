#ifdef DBDATA_H
#else
#define DBDATA_H
#define MAX_ROOM_COUNT 30
typedef struct 
{
    char name[StringSize];
    short spectrum[SampleSize];
}DBData;

#endif