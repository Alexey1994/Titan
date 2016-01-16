#include "parser_print.h"
#include <stdio.h>

int braces_count=0;

static void print_variables_data(Type *type)
{
    int i;

    for(i=0; i<braces_count; i++)
        printf("   ");
    printf("   ");
    str_print(type->name);
    printf("\n");
}

static void print_functions_data(Function *function)
{
    int i;

    braces_count++;

    for(i=0; i<braces_count; i++)
        printf("   ");

    str_print(function->name); printf(":\n");
    tree_print(function->types, print_variables_data);
    print_functions(function->functions);

    braces_count--;
}

void print_functions(Tree *tree)
{
    tree_print(tree, print_functions_data);
}
