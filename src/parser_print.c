#include "parser_print.h"
#include <stdio.h>

static int braces_count=0;
static int args_count, args_length;

static void print_arg(Type *type)
{
    args_count++;
    str_print(type->name);
    if(args_count!=args_length)
        printf(",");
}

static void print_variable(Type *type)
{
    int i;

    for(i=0; i<braces_count; i++)
        printf("   ");
    printf("   ");
    str_print(type->name);
    printf("\n");
}

void print_function(Function *function)
{
    int i;

    braces_count++;

    for(i=0; i<braces_count; i++)
        printf("   ");

    args_count=0;
    args_length=function->args->length;
    str_print(function->name);
    printf("(");
    list_print(function->args, print_arg);
    printf("):\n");

    list_print(function->types, print_variable);
    print_functions(function->functions);

    braces_count--;
}

void print_functions(Tree *tree)
{
    tree_print(tree, print_function);
}
