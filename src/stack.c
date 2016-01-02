#include "stack.h"
#include <stdlib.h>

Stack *stack_init()
{
    Stack *s=malloc(sizeof(Stack));

    s->begin=0;

    return s;
}

void push(Stack *s, char *data)
{
    NodeStack *alloc=malloc(sizeof(NodeStack));
    alloc->data=data;
    alloc->previouse=s->begin;
    s->begin=alloc;
}

char *pop(Stack *s)
{
    char *ret=s->begin->data;
    NodeStack *del=s->begin;
    s->begin=s->begin->previouse;
    free(del);

    return ret;
}

void stack_clean(Stack *s)
{
    NodeStack *i=s->begin, *del=i;

    while(i)
    {
        free(del->data);
        free(del);
        i=i->previouse;
        del=i;
    }
}

void stack_free(Stack *s)
{
    NodeStack *i=s->begin, *del=i;

    while(i)
    {
        i=i->previouse;
        free(del->data);
        free(del);
        del=i;
    }
    free(s);
}
