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

    EN_TYPE projType = assignProj_EN(e->type);
    bErrorTypeNA(projType, "Enitity missing type in Attacks_singStraight\n");

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
    
    EN_TYPE projType = assignProj_EN(e->type);
    bErrorTypeNA(projType, "Enitity missing type in Attack_spreadt\n");
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
    
    EN_TYPE projType = assignProj_EN(e->type);
    bErrorTypeNA(projType, "Enitity missing type in Attacks_radius\n");
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

    EN_TYPE projType = assignProj_EN(e->type);
    bErrorTypeNA(projType, "Enitity missing type in Attacks_radius_time\n");
    
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
        
        ai->argSize = sizeof(AI_TIMERAD_STRUCT);
        AI_TIMERAD_STRUCT timeRadStruct;
        timeRadStruct.disperseTick = dispTickTime;
        timeRadStruct.finalDeg = e->pos.degree;
        memcpy(ai->args,&timeRadStruct,sizeof(AI_TIMERAD_STRUCT));
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
    bErrorNull(e,"Passed Null Entity Attack_helix\n");
    bErrorNull(projShape,"Passed Null projectile shape Attack_helix\n");

    EN_TYPE projType = assignProj_EN(e->type);
    bErrorTypeNA(projType, "Enitity missing type in Attack_helix\n");
    
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

    AI_DATA *ai = calloc(1,sizeof(*ai) + sizeof(AI_HELIX_STRUCT));
    bErrorNull(ai, "Out of memory creating ai\n");
    
    ai->argSize = sizeof(AI_HELIX_STRUCT);
    AI_HELIX_STRUCT helixStruct;
    helixStruct.period = period;
    helixStruct.curPeriod = initPeriod;
    helixStruct.degDelta = degDelta;
    memcpy(ai->args,&helixStruct,sizeof(AI_HELIX_STRUCT));
    ai->aiType = AI_HELIX;
    ai->action = AI_move_helix;
    proj->ai = ai;

    // printf("%f\n",degDelta);
    E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
    bErrorNull(E_LIST[LIST_PROJ], "Failed to add straight proj to list radius spread\n");
    
}

void ATTACKS_spiralShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float amp, float freq)
{
    bErrorNull(e,"Passed Null Entity Attack_spiral\n");
    bErrorNull(projShape,"Passed Null projectile shape Attack_spiral\n");

    EN_TYPE projType = assignProj_EN(e->type);
    bErrorTypeNA(projType, "Enitity missing type in Attack_spiral\n");
    
    vec2 res;
    ENTITY_vertexDirection(e,res,windHeight,windWidth);
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

    AI_DATA *ai = calloc(1,sizeof(*ai) + sizeof(AI_SPIRAL_STRUCT));
    bErrorNull(ai, "Out of memory creating ai\n");
    
    ai->argSize = sizeof(AI_SPIRAL_STRUCT);
    AI_SPIRAL_STRUCT spiralStruct;
    // spiralStruct.upperAng = widthAngle/2 + currDegree;
    // spiralStruct.lowerAng = widthAngle/2 - currDegree;
    // spiralStruct.baseDeg = currDegree;
    glmc_vec2_copy(e->pos.direction,spiralStruct.baseDir);
    spiralStruct.amp = amp;
    spiralStruct.period = M_PI/2;
    spiralStruct.freq = freq;
    proj->pos.velocity = velocity;
    ENTITY_updateDeg(proj, proj->pos.degree);
    memcpy(ai->args,&spiralStruct,sizeof(AI_SPIRAL_STRUCT));
    ai->aiType = AI_SPIRAL;
    ai->action = AI_move_spiral;
    proj->ai = ai;

    // printf("%f\n",degDelta);
    E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
    bErrorNull(E_LIST[LIST_PROJ], "Failed to add straight proj to list radius spread\n");
}

EN_TYPE assignProj_EN(EN_TYPE type)
{
    switch (type)
    {
        case TYPE_PLAY_MAIN:
            return TYPE_PLAY_PROJ;
        case TYPE_ENEMY:
            return TYPE_ENEMY_PROJ;
        case TYPE_PLAY_PROJ:
            return TYPE_PLAY_PROJ;
        case TYPE_ENEMY_PROJ:
            return TYPE_ENEMY_PROJ;
        default:
            return TYPE_NA;
            fprintf(stderr,"Enitity missing type in Attacks_radius_time\n");
            exit(EXIT_FAILURE);
    }
}