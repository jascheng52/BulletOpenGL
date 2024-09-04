#include <stdlib.h>
#include <stdio.h>
#include <entity.h>
#include <math.h>

#include <cglm/call.h>
#include <cglm/util.h>
#include <math.h>
float EPSILON = 0.001;

//0 degrees from up_dir would be (0,1)
vec2 DEF_UP_DIR = {0,1};

size_t ENTITY_maxSize  = DEF_MAX_ENTITY;
size_t eListSize = 0;
ENTITY **eList = NULL;

ENTITY *ENTITY_create(EN_TYPE type, vec2 *vertices, size_t lenVert, 
    float scale, float x,float y,float deg)
{
    switch(type)
    {
        case PLAYER:
        case ENEMY:
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

    ENTITY *entity = malloc(sizeof(*entity) );
    if(entity == NULL)
    {
        fprintf(stderr, "Out of memory creating entity\n");
        return NULL;
    }
    entity->type = type;
    entity->sizeVertices = lenVert;
    entity->vertices = vertices;
    entity->degree = deg;
    entity->xPos = x;
    entity->yPos = y;
    entity->velocity = 0;
    entity->scale = scale;
    gen_rot_mat_up(deg, entity->rotMat);
    glmc_mat2_mulv(entity->rotMat,DEF_UP_DIR,entity->direction);

    // printf("%lld\n", lenVert);

    for(size_t i = 0; i < lenVert; i++)
    {
        glmc_vec2_scale(entity->vertices[i],scale,entity->vertices[i]);
        glmc_mat2_mulv(entity->rotMat,entity->vertices[i],entity->vertices[i]);
        // printf("SCALED\n");
        // glm_vec2_print(entity->vertices[i],stdout);
    }
    // if(type == ENEMY)
    //     exit(EXIT_FAILURE);
    // printf("Calculated Vertices\n%f\n%f\n%f\n%f\n", *entity->vertices[0],*entity->vertices[1],*entity->vertices[2],*entity->vertices[3]);
    // exit(EXIT_FAILURE);
    

    return entity;
}

//Frees the entity vertices, and itself
void ENTITY_delete(ENTITY *entity)
{
    if(entity == NULL)
    {
        fprintf(stderr, "Attempted to free null entity\n");
        exit(EXIT_FAILURE);
    }

    free(entity->vertices);
    free(entity);
}

int ENTITY_collide(ENTITY *e1, ENTITY *e2)
{

    for(size_t i = 0; i < e1->sizeVertices; i++)
    {
        vec2 outerV1, outerV2;
        glm_vec2_copy(e1->vertices[i], outerV1);
        if(i == e1->sizeVertices - 1)
            glm_vec2_copy(e1->vertices[0], outerV2);
        else
            glm_vec2_copy(e1->vertices[i + 1], outerV2);
        // printf("PreCopy\n");
        // glm_vec2_print(outerV1, stdout);
        // glm_vec2_print(outerV2, stdout); 
        glmc_mat2_mulv(e1->rotMat,outerV1,outerV1);
        glmc_mat2_mulv(e1->rotMat,outerV2,outerV2);

        glm_vec2_add(outerV1,(vec2){e1->xPos,e1->yPos}, outerV1);
        glm_vec2_add(outerV2,(vec2){e1->xPos,e1->yPos}, outerV2);
          
        // printf("OUTER\n");
        // ENTITY_printLoc(e1);
        for(size_t j = 0; j < e2->sizeVertices; j++)
        {
            vec2 innerV1, innerV2;
            glm_vec2_copy(e2->vertices[j], innerV1);
            if(j == e2->sizeVertices - 1)
                glm_vec2_copy(e2->vertices[0], innerV2);
            else
                glm_vec2_copy(e2->vertices[j + 1], innerV2);
            glmc_mat2_mulv(e2->rotMat,innerV1,innerV1);
            glmc_mat2_mulv(e2->rotMat,innerV2,innerV2);
            // printf("X:%f    Y:%f\n",e1->xPos, e1->yPos);
            // printf("SUMCHECK1\n");
            // glm_vec2_print(outerV1, stdout);
            // glm_vec2_print(outerV2, stdout);

            // printf("VERTCHECK2\n");
            // glm_vec2_print(innerV1, stdout);
            // glm_vec2_print(innerV2, stdout);
            glm_vec2_add(innerV1,(vec2){e2->xPos,e2->yPos}, innerV1);
            glm_vec2_add(innerV2,(vec2){e2->xPos,e2->yPos}, innerV2);
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

    vec2 castRayOrigin, castRayEnd;
    castRayOrigin[0] = e->xPos;
    castRayOrigin[1] = e->yPos;
    glmc_vec2_mul((vec2){windWidth,windHeight} ,e->direction,castRayEnd);
    glmc_vec2_add(castRayOrigin, castRayEnd, castRayEnd);

    // printf("RAY END\n");
    // glm_vec2_print(castRayEnd,stdout);
    ENTITY_printLoc(e);
    for(size_t i = 0; i < e->sizeVertices; i++)
    {
        vec2 eV1, eV2;
        glm_mat2_mulv(e->rotMat,e->vertices[i],eV1);
        glm_vec2_add(eV1,(vec2){e->xPos, e->yPos}, eV1);

        if(i == (e->sizeVertices - 1) )
        {
            glm_mat2_mulv(e->rotMat,e->vertices[0],eV2);
            glm_vec2_add(eV2,(vec2){e->xPos, e->yPos}, eV2);
        }
        else
        {
            glm_mat2_mulv(e->rotMat,e->vertices[i+1],eV2);
            glm_vec2_add(eV2,(vec2){e->xPos, e->yPos}, eV2);
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

    ENTITY_printLoc(e);
    res[0] = (v1[0] + v2[0]) / 2;
    res[1] = (v1[1] + v2[1]) / 2;
    // glmc_mat2_mulv(e->rotMat,res,res);
    printf("Edge detected");
    glm_vec2_print(res,stdout);
    return 1;
}



void ENTITY_eListInit(size_t max_size)
{
    if(eList != NULL)
    {
        fprintf(stderr, "Tried to initialize enity list when already initialized\n");
        exit(EXIT_FAILURE);
    }
    eList = calloc(max_size, sizeof(ENTITY *));
    if(eList == NULL)
    {
        fprintf(stderr, "Failed to initialize enity list\n");
        exit(EXIT_FAILURE);
    }
    if(max_size > ENTITY_maxSize)
        ENTITY_maxSize = max_size;
}
void ENTITY_eListAdd(ENTITY *e)
{
    if(e == NULL)
    {
        fprintf(stderr, "Attempted to add Null Entity\n");
        exit(EXIT_FAILURE);
    }
    if(eList == NULL)
    {
        fprintf(stderr, "Entity List is not initialized\n");
        exit(EXIT_FAILURE);
    }

    if(eListSize == ENTITY_maxSize)
    {
        eList = realloc(eList,ENTITY_maxSize + DEF_MAX_ENTITY);
        if(eList == NULL)
        {
            fprintf(stderr, "Failed to resize enity list\n");
            exit(EXIT_FAILURE);
        }
        ENTITY_maxSize = ENTITY_maxSize + DEF_MAX_ENTITY;
    }
    eList[eListSize] = e;
    eListSize++;
}
void ENTITY_eListDelete(size_t index)
{
    if(index >= eListSize)
    {
        fprintf(stderr, "Attempted to access out of bound eList index:%lld\n", index);
        exit(EXIT_FAILURE);
    }
    ENTITY *delE = eList[index];
    eList[index] = eList[eListSize - 1];
    eList[eListSize - 1] = NULL;
    eListSize--;
    ENTITY_delete(delE);
}

void ENTITY_eListFree()
{
    if(eList == NULL)
    {
        fprintf(stderr, "Tried to free enity list when already initialized\n");
        exit(EXIT_FAILURE);
    }
    for(size_t i = 0; i < eListSize; i++)
    {
        free(eList[i]);
    }
    free(eList);
    eList = NULL;
    eListSize = 0;
    ENTITY_maxSize = DEF_MAX_ENTITY;
}

void ENTITY_updateDeg(ENTITY *e, float deg)
{
    if(e == NULL)
    {
        fprintf(stderr, "Attempted to update null entry deg\n");
        exit(EXIT_FAILURE);
    }
    e->degree =deg;
    e->direction[0]= sin(glm_deg(deg));
    e->direction[1]= cos(glm_deg(deg));
    gen_rot_mat_up(deg,e->rotMat);
    glmc_mat2_mulv(e->rotMat,DEF_UP_DIR,e->direction);

}

void ENTITY_printLoc(ENTITY * e)
{
    for(size_t i = 0; i < e->sizeVertices; i++)
    {
        vec2 location;
        glm_mat2_mulv(e->rotMat,e->vertices[i],location);
        glm_vec2_add(location,(vec2){e->xPos, e->yPos}, location);
        glm_vec2_print(location,stdout);
    }
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

void gen_rot_mat(vec2 rot, vec2 target, mat2 des)
{
    vec2 rotNorm,targNorm;
    glmc_vec2_copy(rot, rotNorm);
    glmc_vec2_copy(target,targNorm);
    glmc_vec2_normalize(rotNorm);
    // glmc_vec2_normalize(targNorm);
    //ned to fix
    des[0][0] = rotNorm[0]*targNorm[0] + rotNorm[1]*targNorm[1];
    des[0][1] = rotNorm[0]*targNorm[1] - rotNorm[1]*targNorm[0];
    des[1][0] = -(rotNorm[0]*targNorm[1] - rotNorm[1]*targNorm[0] );
    des[1][1] = rotNorm[0]*targNorm[0] + rotNorm[1]*targNorm[1];

}


void gen_rot_mat_up(float deg,mat2 des)
{
    if(deg == 0)
    {
        glmc_mat2_identity(des);
    }
    float sinVal = sin(glm_rad(deg));
    float cosVal = cos(glm_rad(deg));
    des[0][0] = cosVal;
    des[0][1] = -sinVal;
    des[1][0] = sinVal;
    des[1][1] = cosVal;
}
