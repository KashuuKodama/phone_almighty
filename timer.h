#include <time.h>
#ifdef TIMER_H
#else
#define TIMER_H
//時間管理を定義するヘッダ
float current_time;
float previous_time;
float GetTime(){
    return current_time;
}
float GetDeltaTime(){
    return current_time-previous_time;
}
void UpdateTime(){
    previous_time=current_time;
    static clock_t start_time;
    if(start_time==0){
        start_time=clock();
    }
    current_time=((float) (clock() - start_time)) / CLOCKS_PER_SEC;
}
#endif