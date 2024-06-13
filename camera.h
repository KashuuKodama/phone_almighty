#include "vector3.h"
#include "vector3.h"
#include "vector4.h"
#include "model.h"
#include "matrix4x4.h"
#include "texture2d.h"
#include "textures.h"
#include "primitives.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//描画を行うカメラを定義するヘッダ
#ifdef CAMERA_H
#else
#define CAMERA_H
#define CHAR_ASPECT 1.0f/2
#define DEFAULT_SIZE 30.0f
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))

typedef struct camera{
    float pov;
    Vector3 pos;
    Vector3 euler;
    Vector3 size; 
    int* buffer;
    float* depthbuffer;
}Camera;

//カメラの設定をする
Camera* SetupCamera(float pov,int width,int height){
    Camera* camera=(Camera*)malloc(sizeof(Camera));
    camera->pov=pov;
    camera->size=Vec3(width,height,0);
    camera->buffer=(int*)malloc(sizeof(int)*width*height);
    camera->depthbuffer=(float*)malloc(sizeof(float)*width*height);
    return camera;
}
//メモリを解放する
void FreeCamera(Camera* self){
    free(self->buffer);
    free(self);
}
//ワールド空間からスクリーン空間に変換する
Vector3 WorldToScreenPos(Camera* self,Vector3 wpos)
{
    Vector3 vec=Mul_Vec(Mul_Matrix(InverseEuler(self->euler),Trnaslate(Vec3_Mul(-1,self->pos))),wpos);
    return Vec3(vec.x*(self->size.x/DEFAULT_SIZE)/(vec.z*tan(self->pov)),vec.y*(self->size.x/DEFAULT_SIZE)/(vec.z*tan(self->pov)),vec.z);
}
//逆線形補間
Vector2 InvBilinear(Vector2 pos, Vector2 a, Vector2 b, Vector2 c, Vector2 d)
{
    Vector2 uv = Vec2(0,0);
    double A = Vec2_Cross(Vec2_Add(a,Vec2_Mul(-1,pos)),Vec2_Add(a,Vec2_Mul(-1,d)));
    double B =(Vec2_Cross(Vec2_Add(a,Vec2_Mul(-1,pos)),Vec2_Add(b,Vec2_Mul(-1,c))) + Vec2_Cross(Vec2_Add(b,Vec2_Mul(-1,pos)),Vec2_Add(a,Vec2_Mul(-1,d)))) / 2;
    double C = Vec2_Cross(Vec2_Add(b,Vec2_Mul(-1,pos)),Vec2_Add(b,Vec2_Mul(-1,c)));
    if (fabs(A + C - B * 2) < 0.0001f)
    {
        uv.x = A / (A - C);
    }
    else
    {
        float discrim = B * B - A * C;
        float s0 = Vec2_Cross(Vec2_Add(a,Vec2_Mul(-1,pos)),Vec2_Add(a,Vec2_Mul(-1,b)));
        float s1 = Vec2_Cross(Vec2_Add(b,Vec2_Mul(-1,pos)),Vec2_Add(b,Vec2_Mul(-1,c)));
        float s2 = Vec2_Cross(Vec2_Add(c,Vec2_Mul(-1,pos)),Vec2_Add(c,Vec2_Mul(-1,d)));
        float s3 = Vec2_Cross(Vec2_Add(d,Vec2_Mul(-1,pos)),Vec2_Add(d,Vec2_Mul(-1,a)));
        uv.x = (A - B + sqrt(discrim) * ( s0 < 0 && s1 < 0 && s2 < 0 && s3 < 0 ? -1 : 1)) / (A - 2 * B + C);
    }
    if (fabs((a.x - d.x)*(1.0f-uv.x)+(b.x - c.x)* uv.x) >fabs((a.y - d.y)*(1.0f-uv.x)+(b.y - c.y)* uv.x) )
    {
        uv.y = ((a.x - pos.x)*(1.0f-uv.x)+(b.x - pos.x)* uv.x) / ((a.x - d.x)*(1.0f-uv.x)+(b.x - c.x)* uv.x);
    }
    else
    {
         uv.y = ((a.y - pos.y)*(1.0f-uv.x)+(b.y - pos.y)* uv.x) / ((a.y - d.y)*(1.0f-uv.x)+(b.y - c.y)* uv.x);
    }
    return uv;
} 
void Set2DPixel(Camera* self,Vector3 v0,int color){
    if(color==0||v0.z<0)return;
    int x=(int)v0.x;
    int y=(int)((self->size.y-v0.y)*CHAR_ASPECT);
    if(0<=x &&x<self->size.x&&0<=y&&y<self->size.y)
    {
        //Culling処理
        if(self->depthbuffer[y*(int)self->size.x+x]>=v0.z){
            self->buffer[y*(int)self->size.x+x]=color;
            self->depthbuffer[y*(int)self->size.x+x]=v0.z;
        }
    }
}
//2D四角形を描画する
void Draw2DQuad(Camera* self,Vector3 v0,Vector3 v1,Vector3 v2,Vector3 v3,int color){
    v0=Vec3_Add(v0,Vec3_Mul(0.5f,self->size));
    v1=Vec3_Add(v1,Vec3_Mul(0.5f,self->size));
    v2=Vec3_Add(v2,Vec3_Mul(0.5f,self->size));
    v3=Vec3_Add(v3,Vec3_Mul(0.5f,self->size));
    int minx=MIN(v0.x,MIN(v1.x,MIN(v2.x,v3.x)));
    int miny=MIN(v0.y,MIN(v1.y,MIN(v2.y,v3.y)));
    int maxx=MAX(v0.x,MAX(v1.x,MAX(v2.x,v3.x)));
    int maxy=MAX(v0.y,MAX(v1.y,MAX(v2.y,v3.y)));
    for(int x=MAX(0,minx);x<=MIN(self->size.x,maxx);x++){
        for(int y=MAX(-self->size.y,miny);y<=MIN(self->size.y,maxy);y++){
            Vector3 point=Vec3(x,y,0);
            Vector3 d0=Vec3_Add(v1,Vec3_Mul(-1,v0));
            Vector3 d1=Vec3_Add(v2,Vec3_Mul(-1,v1));
            Vector3 d2=Vec3_Add(v3,Vec3_Mul(-1,v2));
            Vector3 d3=Vec3_Add(v0,Vec3_Mul(-1,v3));
            float dot0=Vec3_Cross(d0,Vec3_Add(point,Vec3_Mul(-1,v0))).z;
            float dot1=Vec3_Cross(d1,Vec3_Add(point,Vec3_Mul(-1,v1))).z;
            float dot2=Vec3_Cross(d2,Vec3_Add(point,Vec3_Mul(-1,v2))).z;
            float dot3=Vec3_Cross(d3,Vec3_Add(point,Vec3_Mul(-1,v3))).z;
            if((dot0<=0 && dot1<=0 && dot2<=0 && dot3<=0)||(dot0>=0 && dot1>=0 && dot2>=0 && dot3>=0)){
                Vector3 normal=Vec3_Cross(Vec3_Add(v1,Vec3_Mul(-1,v0)),Vec3_Add(v2,Vec3_Mul(-1,v0)));
                point.z=-Vec3_Dot(normal,Vec3_Add(point,Vec3_Mul(-1,v0)))/normal.z;
                Set2DPixel(self,point,color);
            }
        }
    }
}
//2Dテクスチャを描画する
void Draw2DTexture(Camera* self,Vector3 v0,Vector3 v1,Vector3 v2,Vector3 v3,Vector2 uv0,Vector2 uv1,Vector2 uv2,Vector2 uv3,Texture2D texture,float color_multiplier){
    v0=Vec3_Add(v0,Vec3_Mul(0.5f,self->size));
    v1=Vec3_Add(v1,Vec3_Mul(0.5f,self->size));
    v2=Vec3_Add(v2,Vec3_Mul(0.5f,self->size));
    v3=Vec3_Add(v3,Vec3_Mul(0.5f,self->size));
    int minx=MIN(v0.x,MIN(v1.x,MIN(v2.x,v3.x)));
    int miny=MIN(v0.y,MIN(v1.y,MIN(v2.y,v3.y)));
    int maxx=MAX(v0.x,MAX(v1.x,MAX(v2.x,v3.x)));
    int maxy=MAX(v0.y,MAX(v1.y,MAX(v2.y,v3.y)));
    for(int x=MAX(0,minx);x<=MIN(self->size.x,maxx);x++){
        for(int y=MAX(-self->size.y,miny);y<=MIN(self->size.y,maxy);y++){
            Vector3 point=Vec3(x,y,0);
            Vector3 d0=Vec3_Add(v1,Vec3_Mul(-1,v0));
            Vector3 d1=Vec3_Add(v2,Vec3_Mul(-1,v1));
            Vector3 d2=Vec3_Add(v3,Vec3_Mul(-1,v2));
            Vector3 d3=Vec3_Add(v0,Vec3_Mul(-1,v3));
            float dot0=Vec3_Cross(d0,Vec3_Add(point,Vec3_Mul(-1,v0))).z;
            float dot1=Vec3_Cross(d1,Vec3_Add(point,Vec3_Mul(-1,v1))).z;
            float dot2=Vec3_Cross(d2,Vec3_Add(point,Vec3_Mul(-1,v2))).z;
            float dot3=Vec3_Cross(d3,Vec3_Add(point,Vec3_Mul(-1,v3))).z;
            if((dot0<=0 && dot1<=0 && dot2<=0 && dot3<=0)||(dot0>=0 && dot1>=0 && dot2>=0 && dot3>=0)){
                Vector2 uv=InvBilinear(Vec2(point.x,point.y),Vec2(v0.x,v0.y),Vec2(v1.x,v1.y),Vec2(v2.x,v2.y),Vec2(v3.x,v3.y));
                Vector2 texcoord=Vec2_Lerp(Vec2_Lerp(uv0,uv1,uv.x),Vec2_Lerp(uv3,uv2,uv.x),uv.y);
                Vector3 normal=Vec3_Cross(Vec3_Add(v1,Vec3_Mul(-1,v0)),Vec3_Add(v2,Vec3_Mul(-1,v0)));
                point.z=-Vec3_Dot(normal,Vec3_Add(point,Vec3_Mul(-1,v0)))/normal.z;
                Set2DPixel(self,point,Texture_GetPixel(texture,texcoord)*color_multiplier);
            }
        }
    }
}
//2Dラインを描画する
void Draw2DLine(Camera* self,Vector3 v0,Vector3 v1,int color){
    v0=Vec3_Add(v0,Vec3_Mul(0.5f,self->size));
    v1=Vec3_Add(v1,Vec3_Mul(0.5f,self->size));
    Vector3 vec=Vec3_Add(v1,Vec3_Mul(-1,v0));
    if(fabs(vec.x)>fabs(vec.y)){
        for(int i=0;i<fabs(vec.x);i++){
            float t=(float)i/fabs(vec.x);
            Set2DPixel(self,Vec3_Add(v0,Vec3_Mul(t,vec)),color);
        }
    }
    else{
        for(int i=0;i<fabs(vec.y);i++){
            float t=(float)i/fabs(vec.y);
            Set2DPixel(self,Vec3_Add(v0,Vec3_Mul(t,vec)),color);
        }
    }
}
void Draw2DText(Camera* self,char* text,float padding,Vector3 pos,Vector2 size,int color){
    int length=0;
    while(*(text+length)!='\0'){
        length++;
    }
    float offset=-((float)length/2-padding*(length-1)/2)*size.x;
    while(*text!='\0'){
        Draw2DTexture(self,Vec3_Add(pos,Vec3(offset,0,0)),Vec3_Add(pos,Vec3(offset+size.x,0,0)),Vec3_Add(pos,Vec3(offset+size.x,size.y,0)),Vec3_Add(pos,Vec3(offset,size.y,0)),Vec2(0,0),Vec2(1,0),Vec2(1,1),Vec2(0,1),*AlphabetTexture(*text),color);
        offset+=(1+padding)*size.x;
        text++;
    }
}
//3Dラインを描画する
void Draw3DLine(Camera* self,Vector3 wv0,Vector3 wv1,int color){
    Vector3 v0=WorldToScreenPos(self,wv0);
    Vector3 v1=WorldToScreenPos(self,wv1);
    Draw2DLine(self,v0,v1,color);
}
//3D四角形を描画する
void Draw3DQuad(Camera* self,Vector3 wv0,Vector3 wv1,Vector3 wv2,Vector3 wv3,Vector2 uv0,Vector2 uv1,Vector2 uv2,Vector2 uv3,Texture2D texture,float color_multiplier){
    Vector3 v0=WorldToScreenPos(self,wv0);
    Vector3 v1=WorldToScreenPos(self,wv1);
    Vector3 v2=WorldToScreenPos(self,wv2);
    Vector3 v3=WorldToScreenPos(self,wv3);
    Draw2DTexture(self,v0,v1,v2,v3,uv0,uv1,uv2,uv3,texture,color_multiplier);
}
//3Dモデルを描画する
void Draw3DModel(Camera* self,Model3D model,Matrix4x4 transform,Texture2D texture,int color_multiplier){
    Vector3 tmp[model.vertices_length];
    for(int i=0;i<model.vertices_length;i++){
        tmp[i]=Mul_Vec(transform,model.vertices[i]);
    }
    for(int i=0;i<model.indices_length;i+=4){
        //Draw3DLine(self,tmp[model.indices[i]],tmp[model.indices[i+1]],250);
        //Draw3DLine(self,tmp[model.indices[i+1]],tmp[model.indices[i+2]],250);
        //Draw3DLine(self,tmp[model.indices[i+2]],tmp[model.indices[i+3]],250);
        //Draw3DLine(self,tmp[model.indices[i+3]],tmp[model.indices[i]],250);
        Draw3DQuad(self,tmp[model.indices[i]],tmp[model.indices[i+1]],tmp[model.indices[i+2]],tmp[model.indices[i+3]],model.uvs[model.indices[i]],model.uvs[model.indices[i+1]],model.uvs[model.indices[i+2]],model.uvs[model.indices[i+3]],texture,color_multiplier);
    }
}
void Draw3DText(Camera* self,char* text,float padding,Matrix4x4 transform,int color){
    int length=0;
    while(*(text+length)!='\0'){
        length++;
    }
    float offset=-(float)length/2-padding*(length-1)/2;
    while(*text!='\0'){
        Draw3DModel(self,*Plane(),Mul_Matrix(transform,Trnaslate(Vec3(offset,0,0))),*AlphabetTexture(*text),color);
        offset+=1+padding;
        text++;
    }
}
//描画を開始する
void BeginCamera(Camera* self){
    for(int i=0;i<self->size.y;i++){
        for(int j=0;j<self->size.x;j++){
            self->buffer[i*(int)self->size.x+j]=0;
            self->depthbuffer[i*(int)self->size.x+j]=10000;
        }
    }
}
char swapbuffer[600000];
//描画を終了する
void EndCamera(Camera* self){
    int swapbufferindex=0;
    for(int i=0;i<self->size.y;i++){
        for(int j=0;j<self->size.x;j++){
            if(self->buffer[i*(int)self->size.x+j]){
                swapbufferindex +=sprintf(&swapbuffer[swapbufferindex],"\x1b[38;5;%dm●\x1b[0m",self->buffer[i*(int)self->size.x+j]);
            }
            else{
                swapbufferindex +=sprintf(&swapbuffer[swapbufferindex]," ");
            }
        }
        swapbufferindex +=sprintf(&swapbuffer[swapbufferindex],"\n");
    }
    printf("%s",swapbuffer);
    printf("\033[%dA",(int)self->size.y);
    fflush(stdout);
}
#endif