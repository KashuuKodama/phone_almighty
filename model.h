//3Dモデルを定義するヘッダ
#include "vector3.h"
#include "stdlib.h"
#ifdef MODEL_H
#else
#define MODEL_H
#define MAX_VERTICES_SIZE 2000
#define MAX_INDICES_SIZE 2000
typedef struct model
{
    Vector3* vertices;
    Vector2* uvs;
    int vertices_length;
    int* indices;
    int indices_length;
}Model3D;
Model3D* Model(){
    Model3D* self=(Model3D*)malloc(sizeof(Model3D));
    self->vertices=(Vector3*)malloc(sizeof(Vector3)*MAX_VERTICES_SIZE);
    self->uvs=(Vector2*)malloc(sizeof(Vector2)*MAX_VERTICES_SIZE);
    self->vertices_length=0;
    self->indices=(int*)malloc(sizeof(int)*MAX_INDICES_SIZE);
    self->indices_length=0;
    return self;
}
//3Dモデルに頂点を追加する
void AddVertex(Model3D* self,Vector3 vertex,Vector2 uv){
    self->vertices[self->vertices_length]=vertex;
    self->uvs[self->vertices_length]=uv;
    self->vertices_length++;
}
//3Dモデルの面を追加する。（頂点の番号を4つ指定するとそれらを結ぶ四角形が設定される）
void SetSurface(Model3D* self,int v0,int v1,int v2,int v3){
    self->indices[self->indices_length]=v0;
    self->indices[self->indices_length+1]=v1;
    self->indices[self->indices_length+2]=v2;
    self->indices[self->indices_length+3]=v3;
    self->indices_length+=4;
}
#endif