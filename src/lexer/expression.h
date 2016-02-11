#ifndef EXPRESSION_H_INCLUDED
#define EXPRESSION_H_INCLUDED

#define OPERATION 314

#include "../parser/parser.h"

typedef struct
{
    char *data;
    int type;
}ListExpressionData;

Type* get_expression(Function *cur_function, Stack *stack_functions);
Type* calc_expression(List *l, Function *cur_function, Stack *stack_functions);

#endif // EXPRESSION_H_INCLUDED
