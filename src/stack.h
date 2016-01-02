#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

typedef struct
{
    char *data;
    struct NodeStack *previouse;
}NodeStack;

typedef struct
{
    NodeStack *begin;
}Stack;

Stack *stack_init();
void push(Stack *s, char *data);
char *pop(Stack *s);

#endif // STACK_H_INCLUDED
