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
