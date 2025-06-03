#ifndef __unorderList_h__
#define __unorderList_h__

//List to store pointers
typedef struct unorderList
{
    size_t size;
    size_t max;
    void **elements;
}LIST_UNORD;


//Inits list
LIST_UNORD *LIST_UNORD_create(size_t defMax);

//Get pointer from index
void *LIST_UNORD_get(LIST_UNORD *list, size_t i);

//Adds e to end of list. Resizes if necessary
void *LIST_UNORD_add(LIST_UNORD *list, void *e);

//Deletes at index i and swaps position with last element
void *LIST_UNORD_del(LIST_UNORD *list, size_t i);

//Frees list, Does not free elements
void LIST_UNORD_clear(LIST_UNORD *list);

#endif