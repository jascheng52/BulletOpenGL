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
        res[0],res[1],e->degree);
    if(proj == NULL)
    {
        fprintf(stderr, "Out of memory");
        exit(EXIT_FAILURE);
    }

    proj->velocity = velocity;
    proj->xPos = proj->xPos + proj->direction[0] *proj->scale;
    proj->yPos = proj->yPos + proj->direction[1] *proj->scale;
    // ENTITY_printLoc(proj);
    ENTITY_eListAdd(proj);

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

    float currDegree = e->degree - widthAngle/2;
    for(size_t i = 0; i < amount; i++)
    {
        vec2 *verts = malloc(sizeof(vec2) * numVerts);
        if(verts == NULL)
        {
            fprintf(stderr,"Out of Memory\n");
            exit(EXIT_FAILURE);
        }
        
        ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
        res[0],res[1],currDegree);
        if(proj == NULL)
        {
            fprintf(stderr, "Out of memory");
            exit(EXIT_FAILURE);
        }

        // printf("PREOFFSET\n");
        // ENTITY_printLoc(proj);
        proj->velocity = velocity;
        proj->xPos = proj->xPos + proj->direction[0];
        proj->yPos = proj->yPos + proj->direction[1];
        // printf("AFTER\n");
        // ENTITY_printLoc(proj);


        ENTITY_eListAdd(proj);
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

    float currDegree = e->degree - widthAngle/2;
    for(size_t i = 0; i < amount; i++)
    {
        vec2 *verts = malloc(sizeof(vec2) * numVerts);
        if(verts == NULL)
        {
            fprintf(stderr,"Out of Memory\n");
            exit(EXIT_FAILURE);
        }
        
        ENTITY *proj = ENTITY_create(projType,verts, numVerts, scale,
        res[0],res[1],currDegree);
        if(proj == NULL)
        {
            fprintf(stderr, "Out of memory");
            exit(EXIT_FAILURE);
        }
        proj->velocity = velocity;

        double rads = glm_rad(currDegree);
        double sinRes = sin(rads);
        double cosRes = cos(rads);
        proj->xPos = proj->xPos + sinRes * radius;
        proj->yPos = proj->yPos + cosRes * radius;
        
        ENTITY_updateDeg(proj, e->degree);

        ENTITY_eListAdd(proj);
        currDegree = currDegree + angApart;
    }
    
}
