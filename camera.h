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
    double* depthbuffer;
}Camera;

//カメラの設定をする
Camera* SetupCamera(float pov,int width,int height){
    Camera* camera=(Camera*)malloc(sizeof(Camera));
    camera->pov=pov;
    camera->size=vec3(width,height,0);
    camera->buffer=(int*)malloc(sizeof(int)*width*height);
    camera->depthbuffer=(double*)malloc(sizeof(double)*width*height);
    return camera;
}
//メモリを解放する
void FreeCamera(Camera* self){
    free(self->buffer);
    free(self);
}
//ワールド空間からスクリーン空間に変換する
Vector3 world_to_screen_pos(Camera* self,Vector3 wpos)
{
    Vector3 vec=mul_vec(mul_matrix(inverse_euler(self->euler),translate(vec3_mul(-1,self->pos))),wpos);
    return vec3(vec.x*(self->size.x/DEFAULT_SIZE)/(vec.z*tan(self->pov)),vec.y*(self->size.x/DEFAULT_SIZE)/(vec.z*tan(self->pov)),vec.z);
}
void set_pixel(Camera* self,Vector3 v0,int color){
    if(color==0||v0.z<0)return;
    int x=(int)v0.x;
    int y=(int)(self->size.y-v0.y*CHAR_ASPECT);
    if(0<=x &&x<self->size.x&&0<=y&&y<self->size.y)
    {
        //Culling処理
        if(self->depthbuffer[y*(int)self->size.x+x]>=v0.z){
            self->buffer[y*(int)self->size.x+x]=color;
            self->depthbuffer[y*(int)self->size.x+x]=v0.z;
        }
    }
}
//2D3角形を描画する
void draw_2d_triangle(Camera* self,Vector3 v0,Vector3 v1,Vector3 v2,Vector2 uv0,Vector2 uv1,Vector2 uv2,Texture2D texture,float color_multiplier){
    Vector3 size=self->size;
    size.y*=2;
    v0=vec3_add(v0,vec3_mul(0.5f,size));
    v1=vec3_add(v1,vec3_mul(0.5f,size));
    v2=vec3_add(v2,vec3_mul(0.5f,size));
    int minx=MIN(v0.x,MIN(v1.x,v2.x));
    int miny=MIN(v0.y,MIN(v1.y,v2.y));
    int maxx=MAX(v0.x,MAX(v1.x,v2.x));
    int maxy=MAX(v0.y,MAX(v1.y,v2.y));
    for(int x=MAX(0,minx);x<MIN(size.x,maxx);x++){
        for(int y=MAX(0,miny),drew=0;y<MIN(size.y,maxy);y++){
            Vector3 point=vec3(x,y,0);
            Vector3 d0=vec3_add(v1,vec3_mul(-1,v0));
            Vector3 d1=vec3_add(v2,vec3_mul(-1,v1));
            Vector3 d2=vec3_add(v0,vec3_mul(-1,v2));
            float dot0=vec3_cross(d0,vec3_add(point,vec3_mul(-1,v0))).z;
            float dot1=vec3_cross(d1,vec3_add(point,vec3_mul(-1,v1))).z;
            float dot2=vec3_cross(d2,vec3_add(point,vec3_mul(-1,v2))).z;
            float total=dot0+dot1+dot2;
            if((dot0<=0 && dot1<=0 && dot2<=0 )||(dot0>=0 && dot1>=0 && dot2>=0)){
                Vector2 texcoord=vec2_add(vec2_add(vec2_mul(dot0/total,uv2),vec2_mul(dot1/total,uv0)),vec2_mul(dot2/total,uv1));
                point.z=v2.z*(dot0/total)+v0.z*(dot1/total)+v1.z*(dot2/total);
                set_pixel(self,point,texture_get_pixel(texture,texcoord)*color_multiplier);
                drew=1;
            }
            else if(drew==1){
                break;
            }
        }
    }
}
//2Dラインを描画する
void draw_2d_line(Camera* self,Vector3 v0,Vector3 v1,int color){
    v0=vec3_add(v0,vec3_mul(0.5f,self->size));
    v1=vec3_add(v1,vec3_mul(0.5f,self->size));
    Vector3 vec=vec3_add(v1,vec3_mul(-1,v0));
    if(fabs(vec.x)>fabs(vec.y)){
        for(int i=0;i<fabs(vec.x);i++){
            float t=(float)i/fabs(vec.x);
            set_pixel(self,vec3_add(v0,vec3_mul(t,vec)),color);
        }
    }
    else{
        for(int i=0;i<fabs(vec.y);i++){
            float t=(float)i/fabs(vec.y);
            set_pixel(self,vec3_add(v0,vec3_mul(t,vec)),color);
        }
    }
}

//3Dラインを描画する
void draw_3d_line(Camera* self,Vector3 wv0,Vector3 wv1,int color){
    Vector3 v0=world_to_screen_pos(self,wv0);
    Vector3 v1=world_to_screen_pos(self,wv1);
    draw_2d_line(self,v0,v1,color);
}
//3D3角形を描画する
void draw_3d_triangle(Camera* self,Vector3 wv0,Vector3 wv1,Vector3 wv2,Vector2 uv0,Vector2 uv1,Vector2 uv2,Texture2D texture,float color_multiplier){
    Vector3 v0=world_to_screen_pos(self,wv0);
    Vector3 v1=world_to_screen_pos(self,wv1);
    Vector3 v2=world_to_screen_pos(self,wv2);
    draw_2d_triangle(self,v0,v1,v2,uv0,uv1,uv2,texture,color_multiplier);
}
//3Dモデルを描画する
void draw_3d_model(Camera * self,Model3D model,Matrix4x4 transform,Texture2D texture,int color_multiplier){
    Vector3 tmp[model.vertices_length];
    for(int i=0;i<model.vertices_length;i++){
        tmp[i]=mul_vec(transform,model.vertices[i]);
    }
    for(int i=0;i<model.indices_length;i+=3){
        draw_3d_triangle(self,tmp[model.indices[i]],tmp[model.indices[i+1]],tmp[model.indices[i+2]],model.uvs[model.indices[i]],model.uvs[model.indices[i+1]],model.uvs[model.indices[i+2]],texture,color_multiplier);
    }
}
void draw_3d_text(Camera* c,char* text,float padding,Matrix4x4 transform,int color){
    int length=0;
    while(*(text+length)!='\0'){
        length++;
    }
    float offset=-(float)length/2-padding*(length-1)/2;
    while(*text!='\0'){
        draw_3d_model(c,*plane(),mul_matrix(transform,translate(vec3(offset,0,0))),*gen_alphabettexture(*text),color);
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
char swapbuffer[1000000];
//描画を終了する
void EndCamera(Camera* self){
    int swapbufferindex=0;
    int tmpcolor=0;
    for(int i=0;i<self->size.y;i++){
        for(int j=0;j<self->size.x;j++){
            if(self->buffer[i*(int)self->size.x+j]){
                if(self->buffer[i*(int)self->size.x+j]==tmpcolor){
                    swapbufferindex +=sprintf(&swapbuffer[swapbufferindex],"●");
                }
                else{
                    swapbufferindex +=sprintf(&swapbuffer[swapbufferindex],"\x1b[0m");
                    swapbufferindex +=sprintf(&swapbuffer[swapbufferindex],"\x1b[38;5;%dm●",(tmpcolor=self->buffer[i*(int)self->size.x+j]));
                }
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