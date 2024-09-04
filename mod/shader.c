//Definition of shader methods
#include <shader.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad.h>
#include <cglm/call.h>
SHADER *SHADER_build(char * vertexPath, char* fragmentPath)
{
    if(vertexPath == NULL || fragmentPath == NULL)
    {
        fprintf(stderr,"File path is null for vertex or fragment shader");
        fprintf(stderr,"\nFailed to build shaders\n");
        return NULL;
    }
    
    FILE *vShaderFile = fopen(vertexPath,"rb");
    if(vShaderFile == NULL)
    {
        fprintf(stderr,"Failed to open vertex shader file %s",vertexPath);
        fprintf(stderr,"\nFailed to build shaders\n");
        return NULL;
    }
    FILE *fShaderFile = fopen(fragmentPath,"rb");
    if(vShaderFile == NULL)
    {
        fprintf(stderr,"Failed to open fragment shader file %s",fragmentPath);
        fprintf(stderr,"\nFailed to build shaders\n");
        return NULL;
    }

    if(fseek(vShaderFile, 0 , SEEK_END) != 0)
    {
        fprintf(stderr,"Failed to calculate file length: %s",vertexPath);
        fprintf(stderr,"\nFailed to build shaders\n");
        return NULL;
    }
    long vFileLength = 0;
    vFileLength = ftell(vShaderFile);
    // printf("Vertex Length: %ld\n", vFileLength);
    if(vFileLength < 0)
    {
        fprintf(stderr,"Failed to calculate file length: %s",vertexPath);
        fprintf(stderr,"\nFailed to build shaders\n");
        return NULL;
    }
    rewind(vShaderFile);

    if(fseek(fShaderFile, 0 , SEEK_END) != 0)
    {
        fprintf(stderr,"Failed to calculate file length: %s",fragmentPath);
        fprintf(stderr,"\nFailed to build shaders\n");
        return NULL;
    }
    long fFileLength = 0;
    fFileLength = ftell(fShaderFile);
    // printf("Frag Length: %ld\n", fFileLength);

    if(fFileLength < 0)
    {
        fprintf(stderr,"Failed to calculate file length: %s",fragmentPath);
        fprintf(stderr,"\nFailed to build shaders\n");
        return NULL;
    }
    rewind(fShaderFile);

    char *vShaderString = malloc(vFileLength + 1);
    char *fShaderString = malloc(fFileLength + 1); 
    if(fread(vShaderString, 1, vFileLength, vShaderFile) == 0)
    {
        fprintf(stderr,"Failed to read file length: %s",vertexPath);
        fprintf(stderr,"\nFailed to build shaders\n");
        free(vShaderString);
        free(fShaderString);
        return NULL;
    }
    if(fread(fShaderString, 1, fFileLength, fShaderFile) == 0)
    {
        fprintf(stderr,"Failed to read file length: %s", fragmentPath);
        fprintf(stderr,"\nFailed to build shaders\n");
        free(vShaderString);
        free(fShaderString);
        return NULL;
    }
    vShaderString[vFileLength] = '\0';
    fShaderString[fFileLength] = '\0';

    // printf("%s\n", vShaderString);
    // printf("%s\n", fShaderString);
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar * const *) &vShaderString, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        fprintf(stderr,"Failed to compile vertex shader\n");
        fprintf(stderr,"Error Log: %s\n", infoLog);
        free(vShaderString);
        free(fShaderString);
        return NULL;

    };
    memset(infoLog, 0, 512);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar * const *) &fShaderString, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        fprintf(stderr,"Failed to compile fragment shader\n");
        fprintf(stderr,"Error Log: %s\n", infoLog);
        free(vShaderString);
        free(fShaderString);
        return NULL;

    };
    memset(infoLog, 0, 512);

    unsigned int ID = glCreateProgram();
    glAttachShader(ID,vertex);
    glAttachShader(ID,fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        fprintf(stderr,"Failed to compile shader\n");
        fprintf(stderr,"Error Log: %s\n", infoLog);
        free(vShaderString);
        free(fShaderString);
        return NULL;
    }
    memset(infoLog, 0, 512);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    SHADER *shader = malloc(sizeof(*shader));
    if(shader == NULL)
    {
        fprintf(stderr,"Failed to return shader\n");
        return NULL;
    }
    shader->ID = ID;
    
    free(vShaderString);
    free(fShaderString);
    return shader;
}


void SHADER_setBool(SHADER *shader, char *name, int val)
{
    if(shader == NULL || name == NULL)
    {
        fprintf(stderr, "Failed to pass param\n");
        exit(EXIT_FAILURE);
    }
    glUniform1i(glGetUniformLocation(shader->ID, name), val);

}

void SHADER_setInt(SHADER *shader, char *name, int val)
{
    if(shader == NULL || name == NULL)
    {
        fprintf(stderr, "Failed to pass param\n");
        exit(EXIT_FAILURE);
    }
    glUniform1i(glGetUniformLocation(shader->ID, name), val);

}

void SHADER_setFloat(SHADER *shader, char *name, float val)
{
    if(shader == NULL || name == NULL)
    {
        fprintf(stderr, "Failed to pass param\n");
        exit(EXIT_FAILURE);
    }
    glUniform1f(glGetUniformLocation(shader->ID, name), val);
}

void SHADER_setVec3(SHADER *shader, char *name, vec3 val)
{
    if(shader == NULL || name == NULL)
    {
        fprintf(stderr, "Failed to pass param\n");
        exit(EXIT_FAILURE);
    }
    glUniform3f(glGetUniformLocation(shader->ID, name), val[0],val[1],val[2]);
}

void SHADER_setMat2(SHADER *shader, char* name, mat2 mat)
{
    if(shader == NULL || name == NULL)
    {
        fprintf(stderr, "Failed to pass param\n");
        exit(EXIT_FAILURE);
    }
    glUniformMatrix2fv(glGetUniformLocation(shader->ID, name), 1, GL_FALSE, (float *)mat);
}

void SHADER_use(SHADER *shader)
{
    if(shader == NULL)
    {
        fprintf(stderr, "Failed to pass param\n");
        exit(EXIT_FAILURE);
    }
    glUseProgram(shader->ID);
}

void SHADER_delete(SHADER *shader)
{
    if(shader == NULL)
    {
        fprintf(stderr, "Failed to pass param\n");
        exit(EXIT_FAILURE);
    }
    glDeleteProgram(shader->ID);
    free(shader);
}
