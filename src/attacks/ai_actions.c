#include <string.h>
#include <math.h>

#include <bErrors.h>
#include <aiActions.h>

#include <cglm/call.h>
#include <cglm/util.h>
#include <bullet.h>
#include <entity.h>

//Moves entity forward based on velocity and direction
void AI_moveForward(ENTITY *e)
{
    bErrorNull(e, "Attempted move forward action on null entity\n");
    e->pos.prevXPos = e->pos.xPos;
    e->pos.prevYPos = e->pos.yPos;
    e->pos.xPos = e->pos.velocity*1 * e->pos.direction[0] + e->pos.xPos;        
    e->pos.yPos = e->pos.velocity*1 * e->pos.direction[1] + e->pos.yPos;

}
void (*AI_move_for)(ENTITY *) = &AI_moveForward;

void AI_moveTimeRad(ENTITY *e)
{
    bErrorNull(e, "Attempted move time rad action on null entity\n");
    AI_TIMERAD_STRUCT *timeRad = (void *)e->ai->args;
    size_t disperseTick = timeRad->disperseTick;
    // printf("e:%p   timeDiff: %lld\n", e,GLOB_GAME_TICK - e->timeAlive);
    if(GLOB_GAME_TICK - e->timeAlive >= disperseTick)
    {
        float finalDeg = timeRad->finalDeg;
        ENTITY_updateDeg(e, finalDeg);
        e->pos.prevDeg = e->pos.degree;
        glmc_quat_copy(e->pos.rotQuat, e->pos.prevQuat);
        e->ai->action = AI_move_for;
        return;
    }
    
    e->pos.prevXPos = e->pos.xPos;
    e->pos.prevYPos = e->pos.yPos;
    e->pos.xPos = e->pos.velocity*1 * e->pos.direction[0] + e->pos.xPos;        
    e->pos.yPos = e->pos.velocity*1 * e->pos.direction[1] + e->pos.yPos;

}
void (*AI_move_timeRad)(ENTITY *) = &AI_moveTimeRad;

void AI_moveHelix(ENTITY *e)
{
    bErrorNull(e, "Attempted moveHelix on null entity\n");
    AI_HELIX_STRUCT *helixStruct = (void *)e->ai->args;

    int period = helixStruct->period;
    int curTime = helixStruct->curPeriod;
    float sign = helixStruct->degDelta;
    if(curTime == 0)
    {
        curTime = period;
        helixStruct->curPeriod = curTime;
        sign = -sign;
        helixStruct->degDelta = sign;
         
    }
    ENTITY_updateDeg(e, e->pos.degree + sign);
    e->pos.prevXPos = e->pos.xPos;
    e->pos.prevYPos = e->pos.yPos;
    e->pos.xPos = e->pos.velocity*1 * e->pos.direction[0] + e->pos.xPos;        
    e->pos.yPos = e->pos.velocity*1 * e->pos.direction[1] + e->pos.yPos;
    curTime--;
    // printf("%f\n", sign);
    helixStruct->curPeriod = curTime;

}
void(*AI_move_helix)(ENTITY *) = &AI_moveHelix;

void AI_moveSpiral(ENTITY *e)
{
    bErrorNull(e, "Attempted moveHelix on spiral entity\n");
    AI_SPIRAL_STRUCT *spiralStruct = (void *) (e->ai->args);
    
    float sinRes = (float)sin(spiralStruct->period/spiralStruct->freq);
    float xSinOff = spiralStruct->baseDir[1] * sinRes * spiralStruct->amp;
    float ySinOff = spiralStruct->baseDir[0] * sinRes * spiralStruct->amp;

    spiralStruct->period = spiralStruct->period + 1.0/120;
    if(spiralStruct->period >  M_PI * 2)
        spiralStruct->period = 0;
   
    e->pos.prevXPos = e->pos.xPos;
    e->pos.prevYPos = e->pos.yPos;
    e->pos.xPos = e->pos.velocity * e->pos.direction[0]/2 + e->pos.xPos + xSinOff/2;        
    e->pos.yPos = e->pos.velocity * e->pos.direction[1]/2 + e->pos.yPos + ySinOff/2;

    
}
void (*AI_move_spiral)(ENTITY *) = &AI_moveSpiral;