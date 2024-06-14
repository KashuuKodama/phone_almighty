//基本形状を定義するヘッダ
#include <math.h>
#include "vector3.h"
#include "model.h"
#include <stdio.h>
#include <string.h>
#ifdef PRIMITIVES_H
#else
#define PRIMITIVES_H
#define MAX_STR_LENGTH 200
Model3D* plane(){
    static Model3D* model;
    if(model!=NULL){
        return model;
    }
    model=gen_model();
    add_vertex(model,vec3(-0.5f,-0.5f,-0.5f));
    add_vertex(model,vec3(0.5f,-0.5f,-0.5f));
    add_vertex(model,vec3(0.5f,0.5f,-0.5f));
    add_vertex(model,vec3(-0.5f,0.5f,-0.5f));
    add_uv(model,vec2(0,0));
    add_uv(model,vec2(0,1));
    add_uv(model,vec2(1,1));
    add_uv(model,vec2(1,0));
    set_surface(model,0,1,2);
    set_surface(model,0,2,3);
    return model;
}
Model3D* open_obj(char* path){
    Model3D* model=gen_model();
    FILE* fp=fopen(path,"r");
    char line[MAX_STR_LENGTH];
    Vector3* vertices=malloc(sizeof(Vector3)*MAX_VERTICES_SIZE);
    Vector2* uvs=malloc(sizeof(Vector2)*MAX_VERTICES_SIZE);
    int vertices_length=0;
    int uvs_length=0;
    int face_length=0;
    while (fgets(line,MAX_STR_LENGTH,fp)!=NULL)
    {
        char head[MAX_STR_LENGTH];
        char tmp[MAX_STR_LENGTH];
        sscanf(line,"%s ",head);

        if(strcmp(head,"v")==0){
            Vector3 v=vec3(0,0,0);
            sscanf(line,"%s %f %f %f",tmp,&v.x,&v.y,&v.z);
            vertices[vertices_length]=v;
            vertices_length++;
        }
        if(strcmp(head,"vt")==0){
            Vector2 uv=vec2(0,0);
            sscanf(line,"%s %f %f",tmp,&uv.x,&uv.y);
            uvs[uvs_length]=uv;
            uvs_length++;
        }
        if(strcmp(head,"f")==0){
            int v0,v1,v2;
            int t0,t1,t2;
            int n0,n1,n2;
            sscanf(line,"%s %d/%d/%d %d/%d/%d %d/%d/%d",tmp,&v0,&t0,&n0,&v1,&t1,&n1,&v2,&t2,&n2);
            sscanf(line,"%s %d/%d %d/%d %d/%d",tmp,&v0,&t0,&v1,&t1,&v2,&t2);
            sscanf(line,"%s %d %d %d",tmp,&v0,&v1,&v2);

            add_vertex(model,vertices[v0-1]);
            add_uv(model,uvs[t0-1]);
            add_vertex(model,vertices[v1-1]);
            add_uv(model,uvs[t1-1]);
            add_vertex(model,vertices[v2-1]);
            add_uv(model,uvs[t2-1]);

            set_surface(model,3*face_length,3*face_length+1,3*face_length+2);
            face_length++;
        }
    }
    fclose(fp);
    free(uvs);
    free(vertices);
   return model;
}
#endif