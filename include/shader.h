#ifndef __shader_h__
#define __shader_h__
#include <cglm/call.h>
typedef struct Shader
{
    unsigned int ID;    //id of shader
}SHADER;

//takes in file path of shaders to build glsl. Allocates shader
//caller should free with SHADER_delete
SHADER *SHADER_build(char * vertexPath, char* fragmentPath);


//Uniform set bool, val 0 is false
void SHADER_setBool(SHADER *shader, char *name, int val);
//Unifor set int
void SHADER_setInt(SHADER *shader, char *name, int val);
//Uniform set float
void SHADER_setFloat(SHADER *shader, char *name, float val);
//activate the Shader
void SHADER_use(SHADER *shader);
//deletes shader program and free
void SHADER_delete(SHADER *shader);

void SHADER_setVec3(SHADER *shader, char *name, vec3 val);
void SHADER_setMat2(SHADER *shader, char* name, mat2 mat);



#endif