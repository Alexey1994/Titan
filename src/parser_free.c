#include "parser_free.h"
#include "String.h"
#include "parser.h"
#include "List.h"
#include <stdlib.h>

static void parser_free_function_body(Data *data)
{
    switch(data->type)
    {
        case IF:
            list_free(((If*)data->data)->eval, parser_free_function_body);
            break;

        case LOOP:
            list_free(((Loop*)data->data)->eval, parser_free_function_body);
            break;
    }

    free(data->data);
    free(data);
}

void parser_free_type(Type *t)
{
    if(t->type==CONST_STRING)
        free(((ConstString*)t->data)->data);
    str_free(t->name);
    free(t->data);
    free(t);
}

void parser_free_function(Function *function)
{
    str_free(function->name);
    stack_free(function->pos);
    list_free(function->body, parser_free_function_body);
    tree_free(function->types, parser_free_type);
    list_free(function->args, parser_free_type);
    free(function);
}

void parser_free_functions(Tree *f)
{
    tree_free(f, parser_free_function);
}
