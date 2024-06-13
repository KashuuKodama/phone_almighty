#include <math.h>
#ifdef VECTOR2_H
#else
#define VECTOR2_H
//2次元ベクターを定義するヘッダ
typedef struct vector2{
    float x;
    float y;
}Vector2;
Vector2 Vec2(float x,float y){
    Vector2 vec={x,y};
    return vec;
}
Vector2 Vec2_Add(Vector2 a,Vector2 b){
    return Vec2(a.x+b.x,a.y+b.y);
}
Vector2 Vec2_Mul(float s,Vector2 a){
    return Vec2(a.x*s,a.y*s);
}
Vector2 Vec2_LinearSum(float sa,Vector2 a,float sb,Vector2 b){
    return Vec2(sa*a.x+sb*b.x,sa*a.y+sb*b.y);
}
float Vec2_Dot(Vector2 a,Vector2 b){
    return a.x*b.x+a.y*b.y;
}
float Vec2_Cross(Vector2 a,Vector2 b){
    return a.x*b.y-a.y*b.x;
}
Vector2 Vec2_Lerp(Vector2 a,Vector2 b,float t){
    return Vec2(a.x*(1.0f-t)+b.x*t,a.y*(1.0f-t)+b.y*t);
}
float Vec2_SqrMagnitude(Vector2 a){
    return pow(a.x,2)+pow(a.y,2);
}
#endif