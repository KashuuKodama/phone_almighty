#include <math.h>
#ifdef MATRIX4x4_H
#else
#define MATRIX4x4_H
//3次アフィン変換に使う同次行列を定義するヘッダ
typedef struct matrix4x4{
    /*
    00 01 02 03
    10 11 12 13
    20 21 22 23
    30 31 32 33
    */
    float x00;
    float x01;
    float x02;
    float x03;
    float x10;
    float x11;
    float x12;
    float x13;
    float x20;
    float x21;
    float x22;
    float x23;
    float x30;
    float x31;
    float x32;
    float x33;
}Matrix4x4;
Matrix4x4 Matrix()
{
    Matrix4x4 matrix;
    matrix.x00=0;
    matrix.x01=0;
    matrix.x02=0;
    matrix.x03=0;

    matrix.x10=0;
    matrix.x11=0;
    matrix.x12=0;
    matrix.x13=0;

    matrix.x20=0;
    matrix.x21=0;
    matrix.x22=0;
    matrix.x23=0;

    matrix.x30=0;
    matrix.x31=0;
    matrix.x32=0;
    matrix.x33=0;
    return matrix;
}
//恒等変換
Matrix4x4 Identity(){
    Matrix4x4 matrix=Matrix();
    matrix.x00=1;
    matrix.x11=1;
    matrix.x22=1;
    matrix.x33=1;
    return matrix;
}
//平行移動変換
Matrix4x4 Trnaslate(Vector3 pos){
    Matrix4x4 matrix=Identity();
    matrix.x00=1;
    matrix.x11=1;
    matrix.x22=1;
    matrix.x33=1;
    matrix.x03=pos.x;
    matrix.x13=pos.y;
    matrix.x23=pos.z;
    return matrix;
}
//X周りの回転変換
Matrix4x4 RotateX(float rotation){
    Matrix4x4 matrix=Identity();
    matrix.x11=cos(rotation);
    matrix.x12=sin(rotation);
    matrix.x21=-sin(rotation);
    matrix.x22=cos(rotation);
    return matrix;
}
//Y周りの回転変換
Matrix4x4 RotateY(float rotation){
    Matrix4x4 matrix=Identity();
    matrix.x00=cos(rotation);
    matrix.x02=-sin(rotation);
    matrix.x20=sin(rotation);
    matrix.x22=cos(rotation);
    return matrix;
}
//Z周りの回転変換
Matrix4x4 RotateZ(float rotation){
    Matrix4x4 matrix=Identity();
    matrix.x00=cos(rotation);
    matrix.x01=sin(rotation);
    matrix.x10=-sin(rotation);
    matrix.x11=cos(rotation);
    return matrix;
}
//スケール変換
Matrix4x4 Scale(Vector3 scale){
    Matrix4x4 matrix=Identity();
    matrix.x00=scale.x;
    matrix.x11=scale.y;
    matrix.x22=scale.z;
    matrix.x33=1;
    return matrix;
}
//行列とベクトルの積
Vector3 Mul_Vec(Matrix4x4 m,Vector3 v){
    float x=v.x*m.x00+v.y*m.x01+v.z*m.x02+m.x03;
    float y=v.x*m.x10+v.y*m.x11+v.z*m.x12+m.x13;
    float z=v.x*m.x20+v.y*m.x21+v.z*m.x22+m.x23;
    return Vec3(x,y,z);
}
//行列と行列の積
Matrix4x4 Mul_Matrix(Matrix4x4 m0,Matrix4x4 m1){
   Matrix4x4 m;
   m.x00=m0.x00*m1.x00+m0.x01*m1.x10+m0.x02*m1.x20+m0.x03*m1.x30;
   m.x10=m0.x10*m1.x00+m0.x11*m1.x10+m0.x12*m1.x20+m0.x13*m1.x30;
   m.x20=m0.x20*m1.x00+m0.x21*m1.x10+m0.x22*m1.x20+m0.x23*m1.x30;
   m.x30=m0.x30*m1.x00+m0.x31*m1.x10+m0.x32*m1.x20+m0.x33*m1.x30;

   m.x01=m0.x00*m1.x01+m0.x01*m1.x11+m0.x02*m1.x21+m0.x03*m1.x31;
   m.x11=m0.x10*m1.x01+m0.x11*m1.x11+m0.x12*m1.x21+m0.x13*m1.x31;
   m.x21=m0.x20*m1.x01+m0.x21*m1.x11+m0.x22*m1.x21+m0.x23*m1.x31;
   m.x31=m0.x30*m1.x01+m0.x31*m1.x11+m0.x32*m1.x21+m0.x33*m1.x31;

   m.x02=m0.x00*m1.x02+m0.x01*m1.x12+m0.x02*m1.x22+m0.x03*m1.x32;
   m.x12=m0.x10*m1.x02+m0.x11*m1.x12+m0.x12*m1.x22+m0.x13*m1.x32;
   m.x22=m0.x20*m1.x02+m0.x21*m1.x12+m0.x22*m1.x22+m0.x23*m1.x32;
   m.x32=m0.x30*m1.x02+m0.x31*m1.x12+m0.x32*m1.x22+m0.x33*m1.x32;
     
   m.x03=m0.x00*m1.x03+m0.x01*m1.x13+m0.x02*m1.x23+m0.x03*m1.x33;
   m.x13=m0.x10*m1.x03+m0.x11*m1.x13+m0.x12*m1.x23+m0.x13*m1.x33;
   m.x23=m0.x20*m1.x03+m0.x21*m1.x13+m0.x22*m1.x23+m0.x23*m1.x33;
   m.x33=m0.x30*m1.x03+m0.x31*m1.x13+m0.x32*m1.x23+m0.x33*m1.x33;

   return m;
}

//回転変換
Matrix4x4 Euler(Vector3 euler){
    return Mul_Matrix(RotateX(euler.x),Mul_Matrix(RotateY(euler.y),RotateZ(euler.z)));
}
//逆回転変換
Matrix4x4 InverseEuler(Vector3 euler){
    return Mul_Matrix(RotateZ(-euler.z),Mul_Matrix(RotateY(-euler.y),RotateX(-euler.x)));
}
//トランスフォーム変換
Matrix4x4 TRS(Vector3 pos,Vector3 euler,Vector3 scale){
    return Mul_Matrix(Trnaslate(pos),Mul_Matrix(Euler(euler),Scale(scale)));
}
#endif