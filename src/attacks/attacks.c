#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cglm/call.h>
#include <cglm/util.h>
#include <bErrors.h>
#include <attacks.h>
#include <bullet.h>
#include <entity.h>
#include <aiActions.h>

void ATTACKS_singleStraight(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity)
{
    bErrorNull(e, "Passed Null Entity Attack_Straight\n");
    bErrorNull(projShape, "Passed Null projectile shape Attack_Straight\n");

    EN_TYPE projType;
    switch (e->type)
    {
        case TYPE_PLAY_MAIN:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY:
            projType = TYPE_ENEMY_PROJ;
            break;
        case TYPE_PLAY_PROJ:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY_PROJ:
            projType = TYPE_ENEMY_PROJ;
            break;
        default:
            fprintf(stderr,"Enitity missing type in Attacks_singStraight\n");
            exit(EXIT_FAILURE);
            break;
    }

    vec2 *verts = malloc(sizeof(vec2) * numVerts);
    bErrorNull(verts, "Out of Memory\n");

    memcpy(verts, projShape, sizeof(vec2) * numVerts);
    vec2 res;
    ENTITY_vertexDirection(e,res, windHeight,windWidth);


    ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
        res[0],res[1],e->pos.degree);
    bErrorNull(proj, "Out of Memory creating proj\n");
    
    proj->pos.velocity = velocity;
    proj->pos.xPos = proj->pos.xPos + proj->pos.direction[0] *proj->pos.scale;
    proj->pos.yPos = proj->pos.yPos + proj->pos.direction[1] *proj->pos.scale;
    proj->pos.prevDeg = proj->pos.degree;
    glmc_quat_copy(proj->pos.rotQuat, proj->pos.prevQuat);

    AI_DATA *projAI = malloc(sizeof(*projAI));
    bErrorNull(projAI,"Out of memory attack ai\n");

    projAI->aiType = AI_MOVE_FOR;
    projAI->argSize = 0;
    projAI->action = AI_move_for;
    proj->ai = projAI;
    // ENTITY_printLoc(proj);
    E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
    bErrorNull(E_LIST[LIST_PROJ], "Failed to add straight proj to list attack single straight\n");
    
}


void ATTACKS_spreadShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount)
{
    bErrorNull(e, "Passed Null Entity Attack_spread\n");
    bErrorNull(projShape, "Passed Null projectile shape Attack_spread\n");
    
    EN_TYPE projType;
    switch (e->type)
    {
        case TYPE_PLAY_MAIN:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY:
            projType = TYPE_ENEMY_PROJ;
            break;
        case TYPE_PLAY_PROJ:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY_PROJ:
            projType = TYPE_ENEMY_PROJ;
            break;
        default:
            fprintf(stderr,"Enitity missing type in Attack_spreadt\n");
            exit(EXIT_FAILURE);
            break;
    }
    if(amount == 0)
    {
        fprintf(stderr, "Did not specify amount of attacks in Attacks_spread");
        exit(EXIT_FAILURE);
    }

    vec2 res;
    ENTITY_vertexDirection(e,res, windHeight,windWidth);

    float angApart;
    if(amount != 1)
        angApart = widthAngle / (amount-1);
    else
        angApart = widthAngle / 2;

    float currDegree = e->pos.degree - widthAngle/2;
    for(size_t i = 0; i < amount; i++)
    {
        vec2 *verts = malloc(sizeof(vec2) * numVerts);
        bErrorNull(verts, "Out of Memory creating verts\n");
        
        memcpy(verts, projShape, sizeof(vec2) * numVerts);
        ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
        res[0],res[1],currDegree);
        if(proj == NULL)
        {
            fprintf(stderr, "Out of memory");
            exit(EXIT_FAILURE);
        }

        proj->pos.velocity = velocity;
        proj->pos.xPos = proj->pos.xPos;
        proj->pos.yPos = proj->pos.yPos;
        proj->pos.prevDeg = proj->pos.degree;
        glmc_quat_copy(proj->pos.rotQuat, proj->pos.prevQuat);

        AI_DATA *projAI = malloc(sizeof(*projAI));
        bErrorNull(projAI,"Out of memory attack ai\n");

        projAI->aiType = AI_MOVE_FOR;
        projAI->argSize = 0;
        projAI->action = AI_move_for;
        proj->ai = projAI;

        E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
        bErrorNull(E_LIST[LIST_PROJ], "Failed to add straight proj to list attack spread\n");
        
        // printf("Prev %f\n", currDegree);
        currDegree = currDegree + angApart;

        // printf("Shot number %lld , degree :%f\n", i, proj->degree);
    }
}

void ATTACKS_radiusShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount, float radius)
{

    bErrorNull(e,"Passed Null Entity Attack_radius\n");
    bErrorNull(projShape, "Passed Null projectile shape Attack_radius\n");
    
    EN_TYPE projType;
    switch (e->type)
    {
        case TYPE_PLAY_MAIN:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY:
            projType = TYPE_ENEMY_PROJ;
            break;
        case TYPE_PLAY_PROJ:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY_PROJ:
            projType = TYPE_ENEMY_PROJ;
            break;
        default:
            fprintf(stderr,"Enitity missing type in Attacks_radius\n");
            exit(EXIT_FAILURE);
            break;
    }
    if(amount == 0)
    {
        fprintf(stderr, "Did not specify amount of attacks in Attacks_radius");
        exit(EXIT_FAILURE);
    }

    vec2 res;
    ENTITY_vertexDirection(e,res, windHeight,windWidth);
    float angApart;
    if(amount != 1)
        angApart = widthAngle / (amount-1);
    else
        angApart = widthAngle / 2;

    float currDegree = e->pos.degree - widthAngle/2;
    for(size_t i = 0; i < amount; i++)
    {
        vec2 *verts = malloc(sizeof(vec2) * numVerts);
        bErrorNull(verts, "Out of Memory creating verts\n");
        
        memcpy(verts, projShape, sizeof(vec2) * numVerts);
        ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
        res[0],res[1],currDegree);
        bErrorNull(proj, "Out of memory creating proj");
        
        proj->pos.velocity = velocity;
        double rads = glm_rad(currDegree);
        double sinRes = sin(rads);
        double cosRes = cos(rads);
        // POS_DATA *pos = &proj->pos;
        proj->pos.xPos = proj->pos.xPos + cosRes * radius;
        proj->pos.yPos = proj->pos.yPos + sinRes * radius;
        // printf("XPos: %f", proj->pos.xPos);
        ENTITY_updateDeg(proj, e->pos.degree);
        proj->pos.prevDeg = proj->pos.degree;
        glmc_quat_copy(proj->pos.rotQuat, proj->pos.prevQuat);
        AI_DATA *projAI = malloc(sizeof(*projAI));
        bErrorNull(projAI,"Out of memory attack ai\n");
        projAI->aiType = AI_MOVE_FOR;
        projAI->argSize = 0;
        projAI->action = AI_move_for;
        proj->ai = projAI;

        E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
        bErrorNull(E_LIST[LIST_PROJ],"Failed to add straight proj to list radius spread\n");
        
        currDegree = currDegree + angApart;
    }
    
}



void ATTACKS_timedRadShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount, 
    size_t dispTickTime)
{
    bErrorNull(e,"Passed Null Entity Attack_radius\n");
    bErrorNull(projShape,"Passed Null projectile shape Attack_radius\n");

    EN_TYPE projType;
    switch (e->type)
    {
        case TYPE_PLAY_MAIN:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY:
            projType = TYPE_ENEMY_PROJ;
            break;
        case TYPE_PLAY_PROJ:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY_PROJ:
            projType = TYPE_ENEMY_PROJ;
            break;
        default:
            fprintf(stderr,"Enitity missing type in Attacks_radius_time\n");
            exit(EXIT_FAILURE);
            break;
    }
    if(amount == 0)
    {
        fprintf(stderr, "Did not specify amount of attacks in Attacks_radius_time");
        exit(EXIT_FAILURE);
    }

    vec2 res;
    res[0] = e->pos.xPos;
    res[1] = e->pos.yPos;
    float angApart;
    if(amount != 1)
        angApart = widthAngle / (amount-1);
    else
        angApart = widthAngle / 2;

    float currDegree = e->pos.degree - widthAngle/2;
    for(size_t i = 0; i < amount; i++)
    {
        vec2 *verts = malloc(sizeof(vec2) * numVerts);
        bErrorNull(verts, "Out of Memory creating verts\n");
    
        memcpy(verts, projShape, sizeof(vec2) * numVerts);
        ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
        res[0],res[1],currDegree);
        bErrorNull(proj, "Out of Memory creating proj\n");

        proj->pos.velocity = velocity;
        // POS_DATA *pos = &proj->pos;
        proj->pos.prevXPos = proj->pos.xPos;
        proj->pos.prevYPos = proj->pos.yPos;
        // printf("XPos: %f", proj->pos.xPos);
        glmc_quat_copy(proj->pos.rotQuat, proj->pos.prevQuat);

        AI_DATA *ai = calloc(1,sizeof(*ai) + sizeof(size_t) + sizeof(float));
        bErrorNull(ai, "Out of memory creating ai\n");
        
        ai->argSize = sizeof(size_t) + sizeof(float);
        memcpy(ai->args,&dispTickTime,sizeof(size_t));
        memcpy(ai->args + sizeof(size_t),&e->pos.degree,sizeof(float));
        ai->aiType = AI_TIME_RAD;
        ai->action = AI_move_timeRad;
        proj->ai = ai;

        E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
        bErrorNull(E_LIST[LIST_PROJ], "Failed to add straight proj to list radius spread\n");
        
        currDegree = currDegree + angApart;
    }
    
}

void ATTACKS_helixShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, size_t period, float degDelta)
{
    bErrorNull(e,"Passed Null Entity Attack_radius\n");
    bErrorNull(projShape,"Passed Null projectile shape Attack_radius\n");

    EN_TYPE projType;
    switch (e->type)
    {
        case TYPE_PLAY_MAIN:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY:
            projType = TYPE_ENEMY_PROJ;
            break;
        case TYPE_PLAY_PROJ:
            projType = TYPE_PLAY_PROJ;
            break;
        case TYPE_ENEMY_PROJ:
            projType = TYPE_ENEMY_PROJ;
            break;
        default:
            fprintf(stderr,"Enitity missing type in Attacks_radius_time\n");
            exit(EXIT_FAILURE);
            break;
    }
    

    vec2 res;
    ENTITY_vertexDirection(e,res,windHeight,windWidth);
    size_t initPeriod = period / 2;
    float currDegree = e->pos.degree;
    
    vec2 *verts = malloc(sizeof(vec2) * numVerts);
    bErrorNull(verts, "Out of Memory creating verts\n");

    memcpy(verts, projShape, sizeof(vec2) * numVerts);
    ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
    res[0],res[1],currDegree);
    bErrorNull(proj, "Out of Memory creating proj\n");

    proj->pos.velocity = velocity;
    proj->pos.prevXPos = proj->pos.xPos;
    proj->pos.prevYPos = proj->pos.yPos;
    glmc_quat_copy(proj->pos.rotQuat, proj->pos.prevQuat);

    AI_DATA *ai = calloc(1,sizeof(*ai) + sizeof(int)* 2+ sizeof(float));
    bErrorNull(ai, "Out of memory creating ai\n");
    
    ai->argSize = sizeof(int) * 2 + sizeof(float);
    memcpy(ai->args,&period,sizeof(int));
    memcpy(ai->args + sizeof(int),&initPeriod ,sizeof(int));
    memcpy(ai->args + sizeof(int)*2,&degDelta ,sizeof(float));

    ai->aiType = AI_HELIX;
    ai->action = AI_move_helix;
    proj->ai = ai;

    printf("%f\n",degDelta);
    E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
    bErrorNull(E_LIST[LIST_PROJ], "Failed to add straight proj to list radius spread\n");
    
}