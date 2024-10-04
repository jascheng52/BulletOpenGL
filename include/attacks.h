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

//Creates amount entities proj and shoots from e
// Spaced evenly based on widthAgnle and the radius 
// and moves move forward
void ATTACKS_radiusShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount, float radius);


//Starts moving in radius circle defined by width angle
//Then moves  forward based on the dispTickTIme
void ATTACKS_timedRadShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float widthAngle, size_t amount, 
    size_t dispTickTime);

//Creates amount entities proj and shoots from e
// Moves constanly by degDelta until period time and then reverses in opposite
// ditection. starts at period/2. Deg delta left is postive and negative is right
void ATTACKS_helixShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, size_t period, float degDelta);

void ATTACKS_spiralShot(ENTITY *e, float *projShape, size_t numVerts, 
    float scale, size_t velocity, float amp, float freq);


EN_TYPE assignProj_EN(EN_TYPE type);
#endif 