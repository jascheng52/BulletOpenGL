#include <string.h>


#include <bErrors.h>
#include <aiActions.h>

#include <cglm/call.h>
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
    size_t disperseTick = *(size_t *)(e->ai->args);
    // printf("e:%p   timeDiff: %lld\n", e,GLOB_GAME_TICK - e->timeAlive);
    if(GLOB_GAME_TICK - e->timeAlive >= disperseTick)
    {
        float finalDeg = *(float *)(e->ai->args + sizeof(size_t));
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
    int *period = (int *)(e->ai->args);
    int curTime = *(int *)(e->ai->args + sizeof(int));
    float sign = *(float *)(e->ai->args + sizeof(int) * 2);
    if(curTime == 0)
    {
        curTime = *period;
        memcpy(e->ai->args + sizeof(int), &curTime, sizeof(int));
        sign = -sign;
        memcpy(e->ai->args + sizeof(int) * 2, &sign, sizeof(float));
         
    }

    ENTITY_updateDeg(e, e->pos.degree + sign);
    e->pos.prevXPos = e->pos.xPos;
    e->pos.prevYPos = e->pos.yPos;
    e->pos.xPos = e->pos.velocity*1 * e->pos.direction[0] + e->pos.xPos;        
    e->pos.yPos = e->pos.velocity*1 * e->pos.direction[1] + e->pos.yPos;
    curTime--;
    // printf("%f\n", sign);
    memcpy(e->ai->args + sizeof(int), &curTime, sizeof(int));

}

void(*AI_move_helix)(ENTITY *) = &AI_moveHelix;