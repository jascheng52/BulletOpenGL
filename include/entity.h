#ifndef __entity_h__
#define __entity_h__

#include <cglm/call.h>
#include <aiDataStructs.h>

typedef enum en_type
{
    TYPE_PLAY_MAIN,
    TYPE_PLAY_PROJ,
    TYPE_ENEMY,
    TYPE_ENEMY_PROJ
}EN_TYPE;

typedef enum en_group
{
    ALLY,
    OPPONENT
}EN_GROUP;

//Stores positional information for an entity
typedef struct pos_data
{
    size_t sizeVertices;
    vec2 *vertices;
    vec2 direction;
    versor rotQuat;
    versor prevQuat;
    float degree;
    float prevDeg;
    float xPos;
    float yPos;
    float prevXPos;
    float prevYPos;
    float velocity;
    float scale;
}POS_DATA;
//deg should be updated via update deg
//deg from right x axis dir
typedef struct entity
{
    EN_TYPE type;
    EN_GROUP group;
    POS_DATA pos;
    int hp;
    size_t timeAlive; 
    size_t timeLeft;
    void (*aiAction)(struct entity *,size_t);
    AI_DATA *aiData;
}ENTITY;




#define DEF_MAX_ENTITY 1000
extern size_t ENTITY_maxSize;
extern size_t eListSize;
#define ELIST_PLAY_INDEX 0;
extern size_t lastEnemyIndex;
extern size_t firstProjIndex;
extern ENTITY **eList ;

//Creates an entity
//Assumes the vertices are already allocated
//Assume coordinates are given in Counter Clockwise 
//Takes ownership in releasing vertices memory using delete
//scale is mult by shader vertices
//xy is 2d position, xy direction expected to be normailized
//deg is degrees offset from 0,1 vector counter clockwise
//previous fields(prev quat) must be initialized
ENTITY *ENTITY_create(EN_TYPE type, vec2 *vertices, size_t lenVert,
    float scale, float x,float y, float deg);

//Frees entity instance and the vertices
void ENTITY_delete(ENTITY *entity);

//Determiines if 2 entities collide by comparing edge intersection
int ENTITY_collide(ENTITY *e1, ENTITY *e2);

//Returns to res the vertex that is intersection of
// boundary of e and the direction e is facing
// Midpoint if one edge otherwise the corner
// Returns fail/success
int ENTITY_vertexDirection(ENTITY *e, vec2 res, float windHeight, float windWidth);

//Initializes array for entities of max_size into eList
//Only one elist at time to avoid corruption
//Caller frees elist
void ENTITY_eListInit(size_t max_size);

//Adds *entity to end of elist. Resizes list by DEF_MAX * 2
void ENTITY_eListAdd(ENTITY *e);

//Deletes *entity at index. Swaps last entry to index
void ENTITY_eListDelete(size_t index);

//Frees the elist, frees all elist entries
void ENTITY_eListFree();

//Updates the degrees,direction vector, and rotational matrix
//Sets the previous degree
void ENTITY_updateDeg(ENTITY *e, float deg);

//Handles actions to be done at every game tick
//including timeAlive, Should be done via update loop
//Returns true if entity is deleted
int ENTITY_tickUpdate(ENTITY *e, size_t index);

//Calculate window cords of v based on e and store in des
void ENTITY_worldCords(ENTITY *e, vec2 v, vec2 des);


void ENTITY_printLoc(ENTITY * e);

//Checks if two lines intersect, using side test
int intersect_2d(vec2 line1_0, vec2 line1_1, vec2 line2_0, vec2 line2_1);

//Checks if point b is left of line a
int onLeft(vec2 a1, vec2 a2, vec2 b);

//Checks if point b is right of line a
int onRight(vec2 a1, vec2 a2, vec2 b);

//Checks if point b is on of line a
int collinear(vec2 a1, vec2 a2, vec2 b);

#endif