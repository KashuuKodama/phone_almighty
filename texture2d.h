//2次元テクスチャを定義するヘッダ
#include <stdlib.h>
#ifdef TEXTURE_H
#else
#define TEXTURE_H
typedef struct texture2d{
    int width;
    int height;
    int* buffer;
}Texture2D;
Texture2D* Texture(int width,int height){
    Texture2D* self=(Texture2D*)malloc(sizeof(Texture2D));
    self->width=width;
    self->height=height;
    self->buffer=(int*)malloc(sizeof(int)*width*height);
    for(int i=0;i<width*height;i++){
        self->buffer[i]=0;
    }
    return self;
}
void Texture_SetPixel(Texture2D* self,int x,int y,int color){
    self->buffer[self->width*y+x]=color;
}
int Texture_GetPixel(Texture2D self,Vector2 uv){
    int x=uv.x*self.width;
    int y=uv.y*self.height;
    x=x>=0?x<self.width?x:self.width-1:0;
    y=y>=0?y<self.height?y:self.height-1:0;
    return self.buffer[self.width*y+x];
}
#endif