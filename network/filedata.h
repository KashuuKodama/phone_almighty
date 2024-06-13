#ifdef FILEDATA_H
#else
#define FILEDATA_H
#define SampleSize 256
#define StringSize 32
typedef struct 
{
    char name[StringSize];
    short sample[SampleSize];
}FileData;

#endif