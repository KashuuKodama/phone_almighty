#include <math.h>
#ifdef VECTOR2_H
#else
#define VECTOR2_H
//2次元ベクターを定義するヘッダ
typedef struct{
    float x;
    float y;
}Vector2;
Vector2 vec2(float x,float y){
    Vector2 vec={x,y};
    return vec;
}
Vector2 vec2_add(Vector2 a,Vector2 b){
    return vec2(a.x+b.x,a.y+b.y);
}
Vector2 vec2_mul(float s,Vector2 a){
    return vec2(a.x*s,a.y*s);
}
Vector2 vec2_linearsum(float sa,Vector2 a,float sb,Vector2 b){
    return vec2(sa*a.x+sb*b.x,sa*a.y+sb*b.y);
}
float vec2_dot(Vector2 a,Vector2 b){
    return a.x*b.x+a.y*b.y;
}
float vec2_cross(Vector2 a,Vector2 b){
    return a.x*b.y-a.y*b.x;
}
Vector2 vec2_lerp(Vector2 a,Vector2 b,float t){
    return vec2(a.x*(1.0f-t)+b.x*t,a.y*(1.0f-t)+b.y*t);
}
float vec2_sqrmagnitude(Vector2 a){
    return pow(a.x,2)+pow(a.y,2);
}
#endif