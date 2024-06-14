#ifdef FILEDATA_H
#else
#define FILEDATA_H
#define SampleSize 8192
#define StringSize 32
typedef struct 
{
    char name[StringSize];
    short spectrum[SampleSize];
}FileData;

#endif