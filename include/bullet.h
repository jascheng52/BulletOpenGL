#ifndef __bullet_h__
#define __bullet_h__

#include <GLFW/glfw3.h>
#include <entity.h>
#include <unorderList.h>

#define DEF_TICK_RATE 120
#define DEF_MAX_TICK_SKIP 20

#define WINDOWHEIGHT 900
#define WINDOWWIDTH 600
extern int windWidth;
extern int windHeight;
extern size_t GLOB_GAME_TICK;


//Shader vertices
extern float squareEntityCords[8];

extern int squareEntityOrder[6];

extern float arrowEntityCords[8];

extern int arrowEntityOrder[6];

extern float lineBuffer[4];


extern LIST_UNORD * E_LIST[2];
#define LIST_ENEM 0
#define LIST_PROJ 1
#define DEF_MAX_PROJLIST_SIZE 1000
#define DEF_MAX_ENEMYLIST_SIZE 20

//Input handeling
#define KEY_LOCK_OUT 60
extern double LAST_W;
extern double LAST_A;
extern double LAST_S;
extern double LAST_D;
extern double LAST_SPACE;


void userInput(GLFWwindow *window , ENTITY *player);

void updateEnemy();

void updateProj();


#endif