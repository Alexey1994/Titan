#include "parser_free.h"
#include "String.h"
#include "parser.h"
#include <stdlib.h>

void parser_free_type(Type *t)
{
    if(t->type==CONST_STRING)
        free(((ConstString*)t->data)->data);
    str_free(t->name);
    free(t->data);
    free(t);
}

void parser_free_types(Tree *t)
{
    if(t)
    {
        parser_free_types((Tree*)t->left);
        parser_free_type((Type*)t->data);
        parser_free_types((Tree*)t->right);
        free(t);
    }
}

void parser_free_function_body(List *body)
{
    struct ListNode *i=body->begin, *del;
    RunData *del_data;

    while(i)
    {
        del=i;
        i=i->next;

        del_data=del->data;
        switch(del_data->type)
        {
            case IF: parser_free_function_body(((If*)del_data->data)->eval); break;
            case LOOP: parser_free_function_body(((Loop*)del_data->data)->eval); break;
        }

        free(del_data->data);
        free(del->data);
        free(del);
    }

    free(body);
}

void parser_free_function(Function *f)
{
    //str_print(f->name);printf(": ");

    str_free(f->name);
    stack_free(f->pos);
    parser_free_function_body(f->body);
    parser_free_types((Tree*)f->types);
    free(f);
}

void parser_free_functions(Tree *f)
{
    Function *data;

    if(f)
    {
        parser_free_functions((Tree*)f->left);
        data=((Function*)f->data)->functions;
        parser_free_function((Function*)f->data);
        //printf("\n");
        parser_free_functions(data);
        parser_free_functions((Tree*)f->right);
        free(f);
    }
}
