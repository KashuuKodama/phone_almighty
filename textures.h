//基本テクスチャを定義するヘッダ
#include "texture2d.h"
#include <stdlib.h>
#ifdef TEXTURES_H
#else
#define TEXTURES_H
Texture2D* GenerateColortexture(int color){
   Texture2D* tex=Texture(1,1);
   Texture_SetPixel(tex,0,0,color);
   return tex;
}
Texture2D* BrickTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=8;
   const int height=8;
   int data[64]={
      10,10,113,10,10,10,113,113,
      10,10,113,113,10,10,10,113,
      10,10,10,95,10,22,94,10,
      94,10,95,95,10,94,94,94,
      95,95,237,94,94,94,94,94,
      94,95,95,94,94,94,94,237,
      94,94,95,94,94,94,94,94,
      94,94,237,94,94,94,95,94
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}
Texture2D* SuikaTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=10;
   const int height=10;
   int data[100]={
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22,
      83,22,83,22,83,22,83,22,83,22
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}
Texture2D* RingoTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=8;
   const int height=8;
   int data[64]={
      196,196,196,16,82,82,196,196,
      196,196,196,16,82,82,196,196,
      196,196,196,16,196,196,196,196,
      196,196,196,196,196,198,198,196,
      196,196,196,196,196,198,198,196,
      196,196,196,196,196,196,196,196,
      196,196,196,196,196,196,196,196,
      196,196,196,196,196,196,196,196
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}
Texture2D* BudouTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=8;
   const int height=8;
   int data[64]={
      10,10,10,10,10,10,10,10,
      10,10,10,10,10,10,10,20,
      20,10,10,20,10,10,20,20,
      20,20,20,20,20,20,20,20,
      20,20,20,20,20,20,20,20,
      20,20,20,20,20,20,20,20,
      20,20,20,20,20,20,20,20,
      20,20,20,20,20,20,20,20
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}Texture2D* OrengeTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=8;
   const int height=8;
   int data[64]={
      10,10,10,10,10,10,10,10,
      10,172,10,10,172,10,10,10,
      172,172,172,172,172,10,172,172,
      172,172,172,172,172,172,172,172,
      172,172,172,172,172,172,172,172,
      172,172,172,172,172,172,172,172,
      172,172,172,172,172,172,172,172,
      172,172,172,172,172,172,172,172
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}
Texture2D* SakuranboTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=8;
   const int height=8;
   int data[64]={
      10,10,10,10,10,10,10,10,
      196,196,196,196,196,196,196,196,
      196,196,196,196,196,196,196,196,
      196,196,196,196,196,196,196,196,
      196,196,196,196,196,196,196,196,
      196,196,196,196,196,196,196,196,
      196,196,196,196,196,196,196,196,
      196,196,196,196,196,196,196,196
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}
Texture2D* MomoTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=16;
   const int height=16;
   int data[256]={
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      160,160,160,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,
      131,131,131,131,131,131,131,131,131,131,131,131,131,131,131,131
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}
Texture2D* PineappleTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=16;
   const int height=16;
   int data[256]={
      10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
      10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
      100,227,227,100,227,227,100,227,227,100,227,227,100,227,227,
      100,227,227,100,227,227,100,227,227,100,227,227,100,227,227,
      100,227,227,100,227,227,100,227,227,100,100,100,100,227,227,
      100,227,227,100,227,227,100,227,227,100,227,100,100,227,227,
      100,100,100,100,227,227,100,227,227,100,227,227,100,227,227,
      100,227,227,100,227,227,100,227,227,100,227,227,100,227,227,
      100,227,227,100,227,227,100,227,227,100,227,227,100,227,227,
      100,227,227,100,100,100,100,227,227,100,227,227,100,227,227,
      100,227,227,100,227,227,100,227,227,100,100,100,100,227,227,
      100,227,227,100,227,227,100,227,227,100,227,227,100,227,227,
      100,227,227,100,100,100,100,227,227,100,227,227,100,227,227,
      100,227,227,100,227,227,100,100,100,100,100,100,100,227,227,
      100,227,227,100,227,227,100,227,227,100,100,100,100,227,227,
      100,227,227,100,227,227,100,227,227,100,100,100,100,227,227,
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}
Texture2D* FloorTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=8;
   const int height=8;
   int data[64]={
      103,103,103,103, 103,103,103,103,
      103,103,103,103, 103,103,103,103,
      103,103,103,103, 103,103,103,103,
      103,103,103,103, 103,103,103,103,
      103,103,103,103, 103,103,103,103,
      103,103,103,103, 103,103,103,103,
      103,103,103,103, 103,103,103,103,
      103,103,103,103, 103,103,103,103
   };
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,data[width*i+j]);
      }
   }
   return tex;
}
Texture2D* ShadowTexture(){
   static Texture2D* tex;
   if(tex!=NULL){
      return tex;
   }
   const int width=2;
   const int height=2;
   tex=Texture(width,height);
   for(int i=0;i<width;i++){
      for(int j=0;j<height;j++){
         Texture_SetPixel(tex,j,height-1-i,16);
      }
   }
   return tex;
}
Texture2D* AlphabetTexture(char c){
   const int width=6;
   const int height=6;
   static Texture2D* textures[37];
   int data[37][36]={
      //space
      {
         0,0,0,0,0,0,
         0,0,0,0,0,0,
         0,0,0,0,0,0,
         0,0,0,0,0,0,
         0,0,0,0,0,0,
         0,0,0,0,0,0
      },
      //A
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,1,1,1,1,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1
      },
      //B
      {
         1,1,1,1,1,0,
         1,0,0,0,0,1,
         1,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,1,1,1,1,0
      },
      //C
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,0,
         1,0,0,0,0,0,
         1,0,0,0,0,1,
         0,1,1,1,1,0
      },
      //D
      {
         1,1,1,1,0,0,
         1,0,0,0,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,1,0,
         1,1,1,1,0,0
      },
      //E
      {
         1,1,1,1,1,1,
         1,0,0,0,0,0,
         1,1,1,1,1,0,
         1,0,0,0,0,0,
         1,0,0,0,0,0,
         1,1,1,1,1,1
      },
      //F
      {
         1,1,1,1,1,1,
         1,0,0,0,0,0,
         1,1,1,1,1,0,
         1,0,0,0,0,0,
         1,0,0,0,0,0,
         1,0,0,0,0,0
      },
      //G
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,0,
         1,0,0,1,1,1,
         1,0,0,0,0,1,
         0,1,1,1,1,0
      },
      //H
      {
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,1,1,1,1,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1
      },
      //I
      {
         0,0,1,1,1,0,
         0,0,0,1,0,0,
         0,0,0,1,0,0,
         0,0,0,1,0,0,
         0,0,0,1,0,0,
         0,0,1,1,1,0
      },
      //J
      {
         0,0,0,1,1,1,
         0,0,0,0,1,0,
         0,0,0,0,1,0,
         1,0,0,0,1,0,
         1,0,0,0,1,0,
         0,1,1,1,0,0
      },
      //K
      {
         1,0,0,0,1,0,
         1,0,0,1,0,0,
         1,1,1,0,0,0,
         1,0,0,1,0,0,
         1,0,0,0,1,0,
         1,0,0,0,0,1
      },
      //L
      {
         1,0,0,0,0,0,
         1,0,0,0,0,0,
         1,0,0,0,0,0,
         1,0,0,0,0,0,
         1,0,0,0,0,0,
         1,1,1,1,1,1
      },
      //M
      {
         1,0,0,0,0,1,
         1,1,0,0,1,1,
         1,0,1,1,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1
      },
      //N
      {
         1,0,0,0,0,1,
         1,1,0,0,0,1,
         1,0,1,0,0,1,
         1,0,0,1,0,1,
         1,0,0,0,1,1,
         1,0,0,0,0,1
      },
      //O
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         0,1,1,1,1,0
      },
      //P
      {
         1,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,1,1,1,1,0,
         1,0,0,0,0,0,
         1,0,0,0,0,0
      },
      //Q
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,1,0,1,
         1,0,0,0,1,1,
         0,1,1,1,1,0
      },
      //R
      {
         1,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,1,1,1,1,0,
         1,0,0,0,1,0,
         1,0,0,0,0,1
      },
      //S
      {
         0,1,1,1,1,1,
         1,0,0,0,0,0,
         0,1,1,1,1,0,
         0,0,0,0,0,1,
         0,0,0,0,0,1,
         1,1,1,1,1,0
      },
       //T
      {
         1,1,1,1,1,0,
         0,0,1,0,0,0,
         0,0,1,0,0,0,
         0,0,1,0,0,0,
         0,0,1,0,0,0,
         0,0,1,0,0,0
      },
      //U
      {
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         0,1,1,1,1,0
      },
      //V
      {
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         0,1,0,0,1,0,
         0,0,1,1,0,0
      },
      //W
      {
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         1,0,1,1,0,1,
         0,1,0,0,1,0
      },
      //X
       {
         1,0,0,0,0,1,
         0,1,0,0,1,0,
         0,0,1,1,0,0,
         0,0,1,1,0,0,
         0,1,0,0,1,0,
         1,0,0,0,0,1
      },
      //Y
       {
         1,0,0,0,1,0,
         0,1,0,1,0,0,
         0,0,1,0,0,0,
         0,0,1,0,0,0,
         0,0,1,0,0,0,
         0,0,1,0,0,0
      },
      //Z
      {
         1,1,1,1,1,1,
         0,0,0,0,1,0,
         0,0,0,1,0,0,
         0,0,1,0,0,0,
         0,1,0,0,0,0,
         1,1,1,1,1,1
      },
      //0
      {
         0,1,1,1,1,0,
         1,0,0,0,1,1,
         1,0,0,1,0,1,
         1,0,1,0,0,1,
         1,1,0,0,0,1,
         0,1,1,1,1,0
      },
      //1
      {
         0,0,0,1,0,0,
         0,0,1,1,0,0,
         0,1,0,1,0,0,
         0,0,0,1,0,0,
         0,0,0,1,0,0,
         1,1,1,1,1,1
      },
      //2
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         0,0,0,0,0,1,
         0,1,1,1,1,0,
         1,0,0,0,0,0,
         1,1,1,1,1,1
      },
      //3
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         0,0,0,1,1,0,
         0,0,0,0,0,1,
         1,0,0,0,0,1,
         0,1,1,1,1,0
      },
      //4
      {
         0,0,0,0,1,0,
         0,0,0,1,1,0,
         0,0,1,0,1,0,
         0,1,0,0,1,0,
         1,1,1,1,1,1,
         0,0,0,0,1,0
      },
      //5
      {
         1,1,1,1,1,1,
         1,0,0,0,0,0,
         1,1,1,1,1,0,
         0,0,0,0,0,1,
         1,0,0,0,0,1,
         0,1,1,1,1,0
      },
      //6
      {
         0,1,1,1,1,0,
         1,0,0,0,0,0,
         1,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         0,1,1,1,1,0
      },
      //7
      {
         1,1,1,1,1,1,
         0,0,0,0,0,1,
         0,0,0,0,1,0,
         0,0,0,1,0,0,
         0,0,1,0,0,0,
         0,0,1,0,0,0
      },
      //8
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         0,1,1,1,1,0
      },
      //9
      {
         0,1,1,1,1,0,
         1,0,0,0,0,1,
         1,0,0,0,0,1,
         0,1,1,1,1,1,
         0,0,0,0,0,1,
         0,1,1,1,1,0
      },
   };
   for(int l=0;l<37;l++){
      if(textures[l]==NULL){
         textures[l]=Texture(width,height);
         for(int i=0;i<width;i++){  
            for(int j=0;j<height;j++){
               Texture_SetPixel(textures[l],j,height-1-i,data[l][width*i+j]);
            }
         }
      }
   }
   if(c==' '){
      return textures[0];
   }
   if('a'<=c && c<='z'){
      return textures[c-'a'+1];
   }
   if('A'<=c && c<='Z'){
      return textures[c-'A'+1];
   }
   if('0'<=c && c<='9'){
      return textures[c-'0'+27];
   }
   return textures[0];
}
#endif