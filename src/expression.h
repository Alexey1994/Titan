#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

#define OPERATION 314

#include "parser.h"

typedef struct
{
    char *data;
    int type;
}ListExpressionData;

int calc_expression(List *l, Type *in_var, Type *fast_var, Function *cur_function, Stack *stack_functions);

#endif // EXPRESSION_H_INCLUDED
