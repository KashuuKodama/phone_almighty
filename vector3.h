#include<math.h>
#ifdef VECTOR3_H
#else
#define VECTOR3_H
//3次元ベクターを定義するヘッダ
typedef struct vector3{
    float x;
    float y;
    float z;
}Vector3;
Vector3 Vec3(float x,float y,float z){
    Vector3 vec={x,y,z};
    return vec;
}
Vector3 Vec3_Add(Vector3 a,Vector3 b){
    return Vec3(a.x+b.x,a.y+b.y,a.z+b.z);
}
Vector3 Vec3_Mul(float s,Vector3 a){
    return Vec3(a.x*s,a.y*s,a.z*s);
}
Vector3 Vec3_LinearSum(float sa,Vector3 a,float sb,Vector3 b){
    return Vec3(sa*a.x+sb*b.x,sa*a.y+sb*b.y,sa*a.z+sb*b.z);
}
float Vec3_Dot(Vector3 a,Vector3 b){
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
Vector3 Vec3_Cross(Vector3 a,Vector3 b){
    return Vec3(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}
Vector3 Vec3_Lerp(Vector3 a,Vector3 b,float t){
    return Vec3(a.x*(1.0f-t)+b.x*t,a.y*(1.0f-t)+b.y*t,a.z*(1.0f-t)+b.z*t);
}
float Vec3_SqrMagnitude(Vector3 a){
    return pow(a.x,2)+pow(a.y,2)+pow(a.z,2);
}
#endif