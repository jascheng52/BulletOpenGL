#ifndef __aiActions_h__
#define __aiActions_h__
#include <cglm/call.h>

#include <entity.h>

/*Types of ai*/
typedef enum ai_type
{
    AI_MOVE_FOR,//(No args)
    AI_TIME_RAD, //(AI_TIMERAD_STRUCT)
    AI_HELIX, // (AI_HELIX_STRUCT)
    AI_SPIRAL // (AI_SPIRAL_STRUCT)
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

typedef struct ai_timeRad_struct
{
    size_t disperseTick;
    float finalDeg;
}AI_TIMERAD_STRUCT;

typedef struct ai_helix_struct
{
    int period;
    int curPeriod;
    float degDelta;
}AI_HELIX_STRUCT;

typedef struct ai_spiral_struct
{
    float period;
    float amp;
    float freq;
    vec2 baseDir;
}AI_SPIRAL_STRUCT;


extern void (*AI_move_for)(ENTITY *);
void AI_moveForward(ENTITY *e);

extern void (*AI_move_timeRad)(ENTITY *);
void AI_moveTimeRad(ENTITY *e);

extern void (*AI_move_helix)(ENTITY *);
void AI_moveHelix(ENTITY *e);

extern void (*AI_move_spiral)(ENTITY *);
void AI_moveSpiral(ENTITY *e);


#endif