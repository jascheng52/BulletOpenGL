#ifndef __attacks_h__
#define __attacks_h__

#include <cglm/call.h>
#include <entity.h>

//Shoots a single proj from e and inserts proj in elist
void ATTACKS_singleStraight(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity);


//Creates amount entities proj and shoots from e
//the width angle is radius of attack and each proj is spread evenly
//taking up the width. inserts into elist from leftmost clockwise direction
//of e.
void ATTACKS_spreadShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount);

void ATTACKS_radiusShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount, float radius);


void ATTACKS_timedRadShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount, 
    size_t dispTickTime);

void ATTACKS_helixShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, size_t period, float degDelta);


#endif 