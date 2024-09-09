#include <stdlib.h>
#include <unorderList.h>



LIST_UNORD *LIST_UNORD_create(size_t defMax)
{
    LIST_UNORD *newList = malloc(sizeof(size_t)*2 + sizeof(void *) * defMax);
    if(newList == NULL)
        return NULL;
    newList->size = 0;
    newList->max = defMax;
    return newList; 
}

void *LIST_UNORD_get(LIST_UNORD *list, size_t i)
{
    return list->elements[i];
}


void *LIST_UNORD_add(LIST_UNORD *list, void *e)
{
    if(e == NULL)
        return NULL;
    if(list == NULL)
        return NULL;

    if(list->size == list->max)
    {
        list->elements = realloc(list->elements,(list->max*2) * sizeof(void *));
        if(list->elements == NULL)
            return NULL;
        list->max = list->max * 2;
    }

    list->elements[list->size] = e;
    list->size++;
    return e;
}

void *LIST_UNORD_del(LIST_UNORD *list, size_t i)
{
    if(i >= list->size)
        return NULL;

    void *delE = list->elements[i];
    list->elements[i] = list->elements[list->size - 1];
    list->elements[list->size - 1] = NULL;
    list->size --;
    return delE;
}

void LIST_UNORD_clear(LIST_UNORD *list)
{
    free(list);
}