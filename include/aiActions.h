#ifndef __aiActions_h__
#define __aiActions_h__

#include <entity.h>
/*Types of ai*/
typedef enum ai_type
{
    AI_MOVE_FOR,
    AI_TIME_RAD
}AI_TYPE;

struct entity;
typedef struct entity ENTITY;
/*
    Stores ai data for an entity
    args and argSize MUST be set before 
    action is called otherwise undefined behavior
*/
typedef struct ai_data
{
    AI_TYPE aiType;
    short argSize;
    void (*action)(ENTITY *);
    char args[0];
}AI_DATA;



extern void (*AI_move_for)(ENTITY *);
void AI_moveForward(ENTITY *e);

extern void (*AI_move_timeRad)(ENTITY *);
void AI_moveTimeRad(ENTITY *e);

#endif