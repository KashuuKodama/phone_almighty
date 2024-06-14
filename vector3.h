#include<math.h>
#ifdef VECTOR3_H
#else
#define VECTOR3_H
//3次元ベクターを定義するヘッダ
typedef struct{
    float x;
    float y;
    float z;
}Vector3;
Vector3 vec3(float x,float y,float z){
    Vector3 vec={x,y,z};
    return vec;
}
Vector3 vec3_add(Vector3 a,Vector3 b){
    return vec3(a.x+b.x,a.y+b.y,a.z+b.z);
}
Vector3 vec3_mul(float s,Vector3 a){
    return vec3(a.x*s,a.y*s,a.z*s);
}
Vector3 vec3_linearsum(float sa,Vector3 a,float sb,Vector3 b){
    return vec3(sa*a.x+sb*b.x,sa*a.y+sb*b.y,sa*a.z+sb*b.z);
}
float vec3_dot(Vector3 a,Vector3 b){
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
Vector3 vec3_cross(Vector3 a,Vector3 b){
    return vec3(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}
Vector3 vec3_lerp(Vector3 a,Vector3 b,float t){
    return vec3(a.x*(1.0f-t)+b.x*t,a.y*(1.0f-t)+b.y*t,a.z*(1.0f-t)+b.z*t);
}
float vec3_sqrmagnitude(Vector3 a){
    return pow(a.x,2)+pow(a.y,2)+pow(a.z,2);
}
#endif