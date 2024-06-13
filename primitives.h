//基本形状を定義するヘッダ
#include <math.h>
#include "vector3.h"
#include "model.h"

#ifdef PRIMITIVES_H
#else
#define PRIMITIVES_H
Model3D* Box(){
   static Model3D* model;
   if(model!=NULL){
      return model;
   }
   model=Model();
   //頂点設定
   AddVertex(model,Vec3(-0.5f,-0.5f,-0.5f),Vec2(0,0));
   AddVertex(model,Vec3(0.5f,-0.5f,-0.5f),Vec2(1,0));
   AddVertex(model,Vec3(0.5f,0.5f,-0.5f),Vec2(1,1));
   AddVertex(model,Vec3(-0.5f,0.5f,-0.5f),Vec2(0,1));

   AddVertex(model,Vec3(-0.5f,-0.5f,0.5f),Vec2(0,0));
   AddVertex(model,Vec3(0.5f,-0.5f,0.5f),Vec2(1,0));
   AddVertex(model,Vec3(0.5f,0.5f,0.5f),Vec2(1,1));
   AddVertex(model,Vec3(-0.5f,0.5f,0.5f),Vec2(0,1));

   AddVertex(model,Vec3(-0.5f,-0.5f,-0.5f),Vec2(0,0));
   AddVertex(model,Vec3(-0.5f,-0.5f,0.5f),Vec2(1,0));
   AddVertex(model,Vec3(-0.5f,0.5f,0.5f),Vec2(1,1));
   AddVertex(model,Vec3(-0.5f,0.5f,-0.5f),Vec2(0,1));

   AddVertex(model,Vec3(0.5f,-0.5f,-0.5f),Vec2(0,0));
   AddVertex(model,Vec3(0.5f,-0.5f,0.5f),Vec2(1,0));
   AddVertex(model,Vec3(0.5f,0.5f,0.5f),Vec2(1,1));
   AddVertex(model,Vec3(0.5f,0.5f,-0.5f),Vec2(0,1));

   SetSurface(model,0,1,2,3);
   SetSurface(model,4,5,6,7);
   SetSurface(model,0,1,5,4);
   SetSurface(model,2,3,7,6);
   SetSurface(model,8,9,10,11);
   SetSurface(model,12,13,14,15);

   return model;
}
Model3D* Plane(){
   static Model3D* model;
   if(model!=NULL){
      return model;
   }
   model=Model();
   AddVertex(model,Vec3(-0.5f,-0.5f,-0.5f),Vec2(0,0));
   AddVertex(model,Vec3(0.5f,-0.5f,-0.5f),Vec2(1,0));
   AddVertex(model,Vec3(0.5f,0.5f,-0.5f),Vec2(1,1));
   AddVertex(model,Vec3(-0.5f,0.5f,-0.5f),Vec2(0,1));
   SetSurface(model,0,1,2,3);
   return model;
}

Model3D* Sphere(){
   static Model3D* model;
   if(model!=NULL){
      return model;
   }
   model=Model();
   //頂点設定
   const int step=8;
   int count=0;
   for(int i=0;i<=step;i++){
      float theta = M_PI*i/step;
      for(int j=0;j<step;j++){
         float phi = 2*M_PI*j/step;
         AddVertex(model,Vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta)),Vec2((float)j/step,(float)i/step));
      }
      if(i>0){
         for(int k=0;k<step;k++){
            SetSurface(model,count,count+1,count+step+1,count+step);
            count++;
         }
      }
   }
   return model;
}
#endif