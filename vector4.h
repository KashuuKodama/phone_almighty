
#include <math.h>
#ifdef VECTOR4_H
#else
#define VECTOR4_H
//4次元ベクターを定義するヘッダ
typedef struct vector4{
    float x;
    float y;
    float z;
    float w;
}Vector4;
Vector4 vec4(float x,float y,float z,float w){
    Vector4 vec={x,y,z,w};
    return vec;
}
Vector4 vec4_add(Vector4 a,Vector4 b){
    return vec4(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);
}
Vector4 vec4_mul(float s,Vector4 a){
    return vec4(a.x*s,a.y*s,a.z*s,a.w*s);
}
Vector4 vec4_linearsum(float sa,Vector4 a,float sb,Vector4 b){
    return vec4(sa*a.x+sb*b.x,sa*a.y+sb*b.y,sa*a.z+sb*b.z,sa*a.w+sb*b.w);
}
float vec4_dot(Vector4 a,Vector4 b){
    return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.y;
}
Vector4 vec4_lerp(Vector4 a,Vector4 b,float t){
    return vec4(a.x*(1.0f-t)+b.x*t,a.y*(1.0f-t)+b.y*t,a.z*(1.0f-t)+b.z*t,a.w*(1.0f-t)+b.w*t);
}
float vec4_sqrmagnitude(Vector4 a){
    return pow(a.x,2)+pow(a.y,2)+pow(a.z,2)+pow(a.w,2);
}
#endif