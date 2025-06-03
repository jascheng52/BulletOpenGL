#include <stdlib.h>
#include <unorderList.h>
#include <string.h>


LIST_UNORD *LIST_UNORD_create(size_t defMax)
{
    LIST_UNORD *newList = malloc(sizeof(*newList));
    if(newList == NULL)
        return NULL;
    newList->size = 0;
    newList->max = defMax;
    void **elementList = malloc(sizeof(void *) * defMax);
    if(elementList == NULL)
        return NULL; 
    newList->elements = elementList;
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
        void **temp = malloc(sizeof(void *) *(list->max));
        memcpy(temp,list->elements, sizeof(void *) *(list->max));

        void **newElements;
        newElements = realloc(list->elements,sizeof(void *) *(list->max) * 2);
        if(newElements == NULL)
            return NULL;
        list->elements = newElements;
        list->max = list->max * 2;
        memcpy(newElements,temp, sizeof(void *) * list->size);
        free(temp);
    }

    list->elements[list->size] = e;
    list->size++;
    return list;
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