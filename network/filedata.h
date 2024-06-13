#ifdef FILEDATA_H
#else
#define FILEDATA_H
#define SampleSize 4096
#define StringSize 32
typedef struct 
{
    char name[StringSize];
    short sample[SampleSize];
}FileData;

#endif