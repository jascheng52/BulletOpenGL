#include <stdlib.h>
#include <stdio.h>

#include <bErrors.h>
#include <entity.h>
#include <bullet.h>
#include <unorderList.h>
#include <math.h>

#include <cglm/call.h>
#include <cglm/util.h>
#include <math.h>

float EPSILON = 0.001;


ENTITY *ENTITY_create(EN_TYPE type, vec2 *vertices, size_t lenVert, 
    float scale, float x,float y,float deg)
{
    ENTITY *e = malloc(sizeof(*e));
    bErrorNull(e, "Out of memory creating entity\n");

    switch(type)
    {
        case TYPE_PLAY_MAIN:
            e->group = ALLY;
            break;
        case TYPE_ENEMY:
            e->group = OPPONENT;
            break;
        case TYPE_PLAY_PROJ:
            e->group = ALLY;
            break;  
        case TYPE_ENEMY_PROJ:
            e->group = OPPONENT;
            break;
        default:
            fprintf(stderr, "Invalid Entity Type\n");
            return NULL;
    }
    
    if(vertices == NULL)
    {
        fprintf(stderr, "Missing vertices arguments\n");
        return NULL;
    }
    POS_DATA *pos = &e->pos;
    e->type = type;
    pos->sizeVertices = lenVert;
    pos->vertices = vertices;
    pos->degree = deg;
    pos->prevDeg = deg;
    pos->xPos = x;
    pos->prevXPos = x;
    pos->yPos = y;
    pos->prevYPos = y;
    pos->scale = scale;
    
    glmc_quat_identity(pos->rotQuat);
    glmc_quat(pos->rotQuat,glm_rad(deg),0,0,1);
    glmc_quat_identity(pos->prevQuat);

    glmc_vec2_zero(pos->direction);
    vec3 dir;
    glmc_quat_rotatev(pos->rotQuat,(vec3){1,0,0},dir);
    pos->direction[0] = dir[0];
    pos->direction[1] = dir[1];

    for(size_t i = 0; i < lenVert; i++)
    {
        // glmc_vec2_zero(pos->vertices[i]);
        glmc_vec2_scale(pos->vertices[i],scale,pos->vertices[i]);
        vec2 vert;
        glmc_vec2_copy(pos->vertices[i],vert);
        vec3 dir;
        glmc_quat_rotatev(pos->rotQuat,(vec3){vert[0],vert[1],0},dir);
        vert[0] = dir[0];
        vert[1] = dir[1];
        glmc_vec2_copy(vert,pos->vertices[i]);
    }

    pos->velocity = 0;
    e->hp = 0;
    e->timeAlive = GLOB_GAME_TICK;
    e->timeLeft = 0;
    e->ai = NULL;
    return e;
}

//Frees the entity vertices, and itself
void ENTITY_delete(ENTITY *e)
{
    bErrorNull(e,"Attempted to free null entity\n" );

    if(e->ai != NULL)
        free(e->ai);
    
    free(e->pos.vertices);
    free(e);
}

int ENTITY_collide(ENTITY *e1, ENTITY *e2)
{
    bErrorNull(e1, "Attempted to collide check null e1 entites at %p",e1);
    bErrorNull(e2, "Attempted to collide check null e2 entites at %p",e2);
    
    POS_DATA *e1Pos = &e1->pos;
    for(size_t i = 0; i < e1Pos->sizeVertices; i++)
    {
        vec2 outerV1, outerV2;
        ENTITY_worldCords(e1,e1Pos->vertices[i],outerV1);
        if(i == e1Pos->sizeVertices - 1)
            ENTITY_worldCords(e1,e1Pos->vertices[0],outerV2);
        else
            ENTITY_worldCords(e1,e1Pos->vertices[i + 1],outerV2);
        
        POS_DATA *e2Pos = &e2->pos;
        for(size_t j = 0; j < e2Pos->sizeVertices; j++)
        {
            vec2 innerV1, innerV2;
            ENTITY_worldCords(e2,e2Pos->vertices[j],innerV1);
            if(j == e2Pos->sizeVertices - 1)
                ENTITY_worldCords(e2,e2Pos->vertices[0],innerV2);
            else
                ENTITY_worldCords(e2,e2Pos->vertices[j + 1],innerV2);
                // glm_vec2_copy(e2->vertices[j + 1], innerV2);
            // printf("X:%f    Y:%f\n",e2->xPos, e2->yPos);
            // printf("SUMCHECK2\n");
            // glm_vec2_print(innerV1, stdout);
            // glm_vec2_print(innerV2, stdout);
            if(intersect_2d(outerV1,outerV2,innerV1,innerV2))
            {
                // printf("COMPARED\n");
                // glm_vec2_print(outerV1, stdout);
                // glm_vec2_print(outerV2, stdout);
                // glm_vec2_print(innerV1, stdout);
                // glm_vec2_print(innerV2, stdout);
                return 1;
            }
        }
    }
    return 0;
}

int ENTITY_vertexDirection(ENTITY *e, vec2 res, float windHeight, float windWidth)
{
    if(e == NULL)
        return 0;

    vec2 v1,v2;
    int e1Detected = 0; 
    glmc_vec2_zero(v1);
    glmc_vec2_zero(v2);

    POS_DATA *ePos = &e->pos;
    vec2 castRayOrigin, castRayEnd;
    castRayOrigin[0] = ePos->xPos;
    castRayOrigin[1] = ePos->yPos;
    glmc_vec2_mul((vec2){windWidth,windHeight} ,ePos->direction,castRayEnd);
    glmc_vec2_add(castRayOrigin, castRayEnd, castRayEnd);

    // printf("RAY END\n");
    // glm_vec2_print(castRayEnd,stdout);
    // ENTITY_printLoc(e);
    for(size_t i = 0; i < ePos->sizeVertices; i++)
    {
        vec2 eV1, eV2;
        ENTITY_worldCords(e,ePos->vertices[i],eV1);
        if(i == (ePos->sizeVertices - 1) )
        {
            ENTITY_worldCords(e,ePos->vertices[0],eV2);
        }
        else
        {
            ENTITY_worldCords(e,ePos->vertices[i+1],eV2);

        }

        if(intersect_2d(castRayOrigin,castRayEnd,eV1,eV2))
        {
            if(!e1Detected)
            {
                glmc_vec2_copy(eV1,v1);
                glmc_vec2_copy(eV2,v2);
                e1Detected = 1;
            }
            else
            {
                //if previous edge is detected and collides
                //with next, vertex must be intersect
                glmc_vec2_copy(eV1,res);
                break;
            }
        }
        else
        {
            if(e1Detected)
                break;
        }
    }
    if(!e1Detected)
        return 0;

    res[0] = (v1[0] + v2[0]) / 2;
    res[1] = (v1[1] + v2[1]) / 2;
    
    return 1;
}

void ENTITY_updateDeg(ENTITY *e, float deg)
{
    bErrorNull(e,"Attempted to update deg null entry deg\n");
    POS_DATA *pos = &e->pos;
    
    pos->prevDeg = pos->degree;
    pos->degree = deg;

    versor q;
    glmc_quat(q,glm_rad(deg),0,0,1);
    glmc_quat_copy(e->pos.rotQuat,e->pos.prevQuat);
    glmc_quat_copy(q, e->pos.rotQuat);

    vec3 dir;
    glmc_quat_rotatev(q ,(vec3){1,0,0},dir);
    pos->direction[0] = dir[0];
    pos->direction[1] = dir[1];
    
}

void ENTITY_printLoc(ENTITY * e)
{
    bErrorNull(e, "Attempted to print null Entity\n");
    POS_DATA *pos = &e->pos;
    
    for(size_t i = 0; i < pos->sizeVertices; i++)
    {
        vec3 qLoc;
        glmc_quat_rotatev(pos->rotQuat,(vec3){pos->vertices[i][0],pos->vertices[i][1],0},qLoc);
        vec2 location;
        location[0] = qLoc[0];
        location[1] = qLoc[1];

        glm_vec2_add(location,(vec2){pos->xPos, pos->yPos}, location);
        glm_vec2_print(location,stdout);
    }
}



void ENTITY_worldCords(ENTITY *e, vec2 v, vec2 des)
{

    bErrorNull(e, "Attempted to calculate world cords on null entity\n");

    POS_DATA *pos = &e->pos;
    glm_vec2_add(v,(vec2){pos->xPos,pos->yPos}, des);
}


int intersect_2d(vec2 line1_0, vec2 line1_1, vec2 line2_0, vec2 line2_1)
{
    
    //-1 is on left,0 assumed collinear default , 1 is on right
    int p1Side = 0, p2Side = 0, res1 = 0;
    if(onLeft(line1_0, line1_1, line2_0))
        p1Side = -1;
    if(onRight(line1_0, line1_1, line2_0))
        p1Side = 1;
    if(collinear(line1_0, line1_1, line2_0))
        p1Side = 0;
    if(onLeft(line1_0, line1_1, line2_1))
        p2Side = -1;
    if(onRight(line1_0, line1_1, line2_1))
        p2Side = 1;
    if(collinear(line1_0, line1_1, line2_1))
        p2Side = 0;

    //Both points collinear
    float d1,d2,normD;
    if(p1Side == 0)
    {
        d1 = glmc_vec2_distance(line2_0, line1_0);
        d2 = glmc_vec2_distance(line2_0,line1_1);
        normD = glmc_vec2_distance(line1_0,line1_1);
        if( ((d1 + d2) <= (normD + EPSILON))  && ((d1 + d2) >= (normD - EPSILON)) )
            return 1;
    }
    if(p2Side == 0)
    {
        d1 = glmc_vec2_distance(line2_1, line1_0);
        d2 = glmc_vec2_distance(line2_1,line1_1);
        normD = glmc_vec2_distance(line1_0,line1_1);
        if((d1 + d2) <= (normD + EPSILON) && (d1 + d2) >= (normD - EPSILON))
            return 1;
    }
    if(res1 != 1)
        res1 = (p1Side == 1 && p2Side == -1) || (p1Side == -1 && p2Side == 1);
    

    int res2 = 0;
    if(onLeft(line2_0, line2_1, line1_0))
        p1Side = -1;
    if(onRight(line2_0, line2_1, line1_0))
        p1Side = 1;
    if(collinear(line2_0, line2_1, line1_0))
        p1Side = 0;
    if(onLeft(line2_0, line2_1, line1_1))
        p2Side = -1;
    if(onRight(line2_0, line2_1, line1_1))
        p2Side = 1;
    if(collinear(line2_0, line2_1, line1_1))
        p2Side = 0;

    if(p1Side == 0)
    {
        d1 = glmc_vec2_distance(line1_0, line2_0);
        d2 = glmc_vec2_distance(line1_0,line2_1);
        normD = glmc_vec2_distance(line2_0,line2_1);
        if((d1 + d2) <= (normD + EPSILON) && (d1 + d2) >= (normD - EPSILON))
            return 1;
    }
    if(p2Side == 0)
    {
        d1 = glmc_vec2_distance(line1_1, line2_0);
        d2 = glmc_vec2_distance(line1_1,line2_1);
        normD = glmc_vec2_distance(line2_0,line2_1);
        if((d1 + d2) <= (normD + EPSILON) && (d1 + d2) >= (normD - EPSILON))
            return 1;
    }
    if(res2 != 1)
        res2 = (p1Side == 1 && p2Side == -1) || (p1Side == -1 && p2Side == 1);


    return res1 && res2;
}




int onLeft(vec2 a1, vec2 a2, vec2 b)
{
    vec2 segDiff, pivotDiff;
    glmc_vec2_sub(a2,a1,segDiff);
    glmc_vec2_sub(b,a1,pivotDiff);
    
    float res = glmc_vec2_cross(segDiff,pivotDiff);
    return res > 0;
}

int onRight(vec2 a1, vec2 a2, vec2 b)
{
    vec2 segDiff, pivotDiff;
    glmc_vec2_sub(a2,a1,segDiff);
    glmc_vec2_sub(b,a1,pivotDiff);
    
    float res = glmc_vec2_cross(segDiff,pivotDiff);
    return res < 0;
}

int collinear(vec2 a1, vec2 a2, vec2 b)
{
    vec2 segDiff, pivotDiff;
    glmc_vec2_sub(a2,a1,segDiff);
    glmc_vec2_sub(b,a1,pivotDiff);
    
    float res = glmc_vec2_cross(segDiff,pivotDiff);
    unsigned int a = ( (res <= (0 + EPSILON))  && (res >= (0 - EPSILON)) );
    return a;
}

