#ifndef __attacks_h__
#define __attacks_h__

#include <entity.h>
#include <cglm/call.h>

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

#endif 