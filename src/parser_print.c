#include "parser_print.h"
#include <stdio.h>

int braces_count=0;

void print_variables(Tree *node)
{
    Type *data;
    int i;
    if(node!=0)
    {
        print_variables((Tree*)node->left);
        data=(Type*)node->data;
        for(i=0; i<braces_count; i++)
            printf("   ");
        printf("   ");
        str_print(data->name);
        printf("\n");
        print_variables((Tree*)node->right);
    }
}

void print_functions(Tree *node)
{
    Function *data;
    int i;
    if(node)
    {
        print_functions((Tree*)node->left);
        data=(Function*)node->data;
        braces_count++;

        for(i=0; i<braces_count; i++)
            printf("   ");
        str_print(data->name); printf(":\n");
        print_variables((Tree*)data->types);

        print_functions(data->functions);
        braces_count--;
        print_functions((Tree*)node->right);
    }
}
