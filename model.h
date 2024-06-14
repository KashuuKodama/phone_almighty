//3Dモデルを定義するヘッダ
#include "vector3.h"
#include "stdlib.h"
#ifdef MODEL_H
#else
#define MODEL_H
#define MAX_VERTICES_SIZE 500000
#define MAX_INDICES_SIZE 200000
typedef struct model
{
    Vector3* vertices;
    int vertices_length;
    Vector2* uvs;
    int uvs_length;
    int* indices;
    int indices_length;
}Model3D;
Model3D* gen_model(){
    Model3D* self=(Model3D*)malloc(sizeof(Model3D));
    self->vertices=(Vector3*)malloc(sizeof(Vector3)*MAX_VERTICES_SIZE);
    self->uvs=(Vector2*)malloc(sizeof(Vector2)*MAX_VERTICES_SIZE);
    self->vertices_length=0;
    self->uvs_length=0;
    self->indices=(int*)malloc(sizeof(int)*MAX_INDICES_SIZE);
    self->indices_length=0;
    return self;
}
//3Dモデルに頂点を追加する
void add_vertex(Model3D* self,Vector3 vertex){
    self->vertices[self->vertices_length]=vertex;
    self->vertices_length++;
}
//3Dモデルにテクスチャ座標を追加する
void add_uv(Model3D* self,Vector2 uv){
    self->uvs[self->uvs_length]=uv;
    self->uvs_length++;
}
//3Dモデルに面を追加する
void set_surface(Model3D* self,int v0,int v1,int v2){
    self->indices[self->indices_length]=v0;
    self->indices[self->indices_length+1]=v1;
    self->indices[self->indices_length+2]=v2;
    self->indices_length+=3;
}
#endif