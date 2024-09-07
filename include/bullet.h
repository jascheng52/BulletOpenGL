#ifndef __bullet_h__
#define __bullet_h__

#include <GLFW/glfw3.h>
#include <entity.h>

#define WINDOWHEIGHT 900
#define WINDOWWIDTH 600
extern int windWidth;
extern int windHeight;


//Shader vertices
extern float squareEntityCords[8];

extern int squareEntityOrder[6];

extern float arrowEntityCords[8];

extern int arrowEntityOrder[6];

extern float lineBuffer[4];




//Input handeling
#define KEY_LOCK_OUT 10
extern double LAST_W;
extern double LAST_A;
extern double LAST_S;
extern double LAST_D;
extern double LAST_SPACE;


void userInput(GLFWwindow *window , ENTITY *player);

void updateProj();


#endif