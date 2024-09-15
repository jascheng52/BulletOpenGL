#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cglm/call.h>
#include <cglm/util.h>

#include <attacks.h>
#include <bullet.h>
#include <entity.h>

void ATTACKS_singleStraight(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity)
{
    if(e == NULL)
    {
        fprintf(stderr, "Passed Null Entity Attack_Straight\n");
        exit(EXIT_FAILURE);
    }
    if(projShape == NULL)
    {
        fprintf(stderr, "Passed Null projectile shape Attack_Straight\n");
        exit(EXIT_FAILURE);
    }

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
    if(verts == NULL)
    {
        fprintf(stderr,"Out of Memory\n");
        exit(EXIT_FAILURE);
    }
    memcpy(verts, projShape, sizeof(vec2) * numVerts);
    vec2 res;
    ENTITY_vertexDirection(e,res, windHeight,windWidth);


    ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
        res[0],res[1],e->pos.degree);
    if(proj == NULL)
    {
        fprintf(stderr, "Out of memory");
        exit(EXIT_FAILURE);
    }

    proj->pos.velocity = velocity;
    proj->pos.xPos = proj->pos.xPos + proj->pos.direction[0] *proj->pos.scale;
    proj->pos.yPos = proj->pos.yPos + proj->pos.direction[1] *proj->pos.scale;
    proj->pos.prevDeg = proj->pos.degree;
    glmc_quat_copy(proj->pos.rotQuat, proj->pos.prevQuat);
    // ENTITY_printLoc(proj);
    E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
    if(E_LIST[LIST_PROJ] == NULL)
    {
        fprintf(stderr,"Failed to add straight proj to list attack single straight\n");
        exit(EXIT_FAILURE);
    }
}


void ATTACKS_spreadShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount)
{
    if(e == NULL)
    {
        fprintf(stderr, "Passed Null Entity Attack_spreadt\n");
        exit(EXIT_FAILURE);
    }
    if(projShape == NULL)
    {
        fprintf(stderr, "Passed Null projectile shape Attack_spreadt\n");
        exit(EXIT_FAILURE);
    }

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
        if(verts == NULL)
        {
            fprintf(stderr,"Out of Memory\n");
            exit(EXIT_FAILURE);
        }
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

        E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
        if(E_LIST[LIST_PROJ] == NULL)
        {
            fprintf(stderr,"Failed to add straight proj to list attack spread\n");
            exit(EXIT_FAILURE);
        }
        // printf("Prev %f\n", currDegree);
        currDegree = currDegree + angApart;

        // printf("Shot number %lld , degree :%f\n", i, proj->degree);
    }
}

void ATTACKS_radiusShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount, float radius)
{
    if(e == NULL)
    {
        fprintf(stderr, "Passed Null Entity Attack_radius\n");
        exit(EXIT_FAILURE);
    }
    if(projShape == NULL)
    {
        fprintf(stderr, "Passed Null projectile shape Attack_radius\n");
        exit(EXIT_FAILURE);
    }

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
        if(verts == NULL)
        {
            fprintf(stderr,"Out of Memory\n");
            exit(EXIT_FAILURE);
        }
        memcpy(verts, projShape, sizeof(vec2) * numVerts);
        ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
        res[0],res[1],currDegree);
        if(proj == NULL)
        {
            fprintf(stderr, "Out of memory");
            exit(EXIT_FAILURE);
        }
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
        E_LIST[LIST_PROJ] = LIST_UNORD_add(E_LIST[LIST_PROJ],proj);
        if(E_LIST[LIST_PROJ] == NULL)
        {
            fprintf(stderr,"Failed to add straight proj to list radius spread\n");
            exit(EXIT_FAILURE);
        }

        currDegree = currDegree + angApart;
    }
    
}
