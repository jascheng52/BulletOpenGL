#ifndef __aiDataStructs_h__
#define __aiDataStructs_h__

#include <entity.h>
/*Types of ai*/
typedef enum ai_type
{
    AI_TIME_RAD
}AI_TYPE;


/*
    Stores ai data for an entity
    args and argSize MUST be set before 
    callback is called otherwise undefined behavior
*/
typedef struct ai_data
{
    AI_TYPE aiType;
    short argSize;
    void (*callback)(ENTITY *, void *);
    void *args;
}AI_DATA;

#endif