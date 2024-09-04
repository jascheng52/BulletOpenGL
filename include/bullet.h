#ifndef __bullet_h__
#define __bullet_h__

#include <GLFW/glfw3.h>
#include <entity.h>

#define WINDOWHEIGHT 900
#define WINDOWWIDTH 600

//Shader vertices

const float squareEntityCords[] = 
{
    //Four corners of square clockwise
    //Starting from bottom left
    -1,  -1,
    -1,   1, 
     1,   1, 
     1,  -1
};

const int squareEntityOrder[] =
{
    0 , 1, 3, 
    3 , 1, 2 
};

const float lineBuffer[] =
{
    0,0,
    0, 1
};


//Input handeling

#define KEY_LOCK_OUT 10
double LAST_W = 0;
double LAST_A = 0;
double LAST_S = 0;
double LAST_D = 0;
double LAST_SPACE = 0;


void userInput(GLFWwindow *window , ENTITY *player);
void updateProj();


#endif