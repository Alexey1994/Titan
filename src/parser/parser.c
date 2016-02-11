#include "parser.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "../Id.h"
#include "parser_print.h"
#include "parser_cmp.h"
#include "parser_free.h"
#include "parser_operations.h"

extern const char ERROR, OK;
extern char debug;

Stack *stack_functions;
Type *new_type(String *name, int type, char *data);

char (*parser_table[256])();

void parser_table_init()
{
    int i;
    for(i=0; i<256; i++)
        parser_table[i]=parser_error;

    parser_table[PRINT]=parser_print;

    parser_table[ELEMENT_INIT]=parser_element_init;
    parser_table[PTRS_INIT]=parser_ptrs_init;
    parser_table[INT_INIT]=parser_integer_init;
    parser_table[REAL_INIT]=parser_real_init;
    parser_table[CONST_INIT]=parser_const_init;
    parser_table[CONST_STRING_INIT]=parser_const_string_init;

    parser_table[ASSIGNMENT]=parser_assignment;
    parser_table[LOOP]=parser_loop;
    parser_table[IF]=parser_if;

    parser_table[BREAK]=parser_break;
    parser_table[CONTINUE]=parser_continue;
    parser_table[FUNCTION]=parser_function;

    parser_table[END]=parser_end;
    parser_table[CALL]=parser_call;

    parser_table[ALLOC]=parser_alloc;
}

String *next_token(String *s)
{
    String *ret;
    char t;

    if(s->length)
    {
        ret=str_init("");

        while(s->length)
        {
            t=str_pop(s);
            if(t=='\0')
                break;
            str_push(ret, t);
        }
        return ret;
    }
    return 0;
}

int get_num(String *s)
{
    int i,ret=1;

    for(i=0; i<BITS; i++)
        ((char*)&ret)[i]=str_pop(s);
    return ret;
}

Type *get_parser_op(String *s, Function *cur_function)
{
    String *new_string;
    Type *op;

    new_string=next_token(s);
    op=find_global_type(cur_function, stack_functions, new_string);

    if(op==0 || op->type!=INTEGER)
    {
        printf("variable ");
        str_print(new_string);
        printf(" not found\n");
        str_free(new_string);
        return 0;
    }
/*
    if(((Variable*)op->data)->uninitialized)
    {
        printf("variable ");
        str_print(new_string);
        printf(" not initialized\n");
        str_free(new_string);
        return 0;
    }*/
    str_free(new_string);

    return op;
}

Type *get_parser_op_all(String *s, Function *cur_function)
{
    String *new_string;
    Type *op;

    new_string=next_token(s);
    op=find_global_type(cur_function, stack_functions, new_string);

    if(op==0)
    {
        //printf("variable '"); str_print(new_string); printf("' not found\n");
        str_free(new_string);
        return 0;
    }
    str_free(new_string);

    return op;
}

String *get_parser_init_name(String *s, Function *cur_function)
{
    String *new_string;

    new_string=next_token(s);
    if(tree_contains(cur_function, new_string))
    {
        str_print(new_string);
        printf(" defined\n");
        str_free(new_string);
        return 0;
    }

    return new_string;
}

void new_run_data(){}

Data *new_data(char type, char *data)
{
    Data *ret=malloc(sizeof(Data));

    ret->data=data;
    ret->type=type;

    return ret;
}

Function* find_function(Tree *tree_cur_function, String *s)
{
    TreeNode *i=tree_cur_function->root;
    int comparision;

    while(i)
    {
        comparision=str_comparision(s, ((Function*)i->data)->name);
        if(comparision==0)
            return (Function*)i->data;
        else if(comparision<0)
            i=(Tree*)i->left;
        else
            i=(Tree*)i->right;
    }

    return 0;
}

Function *find_global_function(Function *cur_function, Tree *tree_cur_function, Stack *functions, String *s)
{
    NodeStack *i=functions->begin;
    Function *data;

    if(str_comparision(cur_function->name, s)==0)
        return cur_function;

    data=find_function(tree_cur_function, s);
    if(data)
        return data;

    while(i)
    {
        data=i->data;
        if(find_function(data->functions, s))
            return data;
        i=i->previouse;
    }

    return 0;
}

Type *find_arg(List *args, String *name)
{
    struct ListNode *i=args->begin;
    Type *ret;

    while(i)
    {
        ret=i->data;
        if(str_comparision(ret->name, name)==0)
            return ret;
        i=i->next;
    }

    return 0;
}

Type *find_type(Tree *types, String *name)
{
    int cmp;
    TreeNode *i=types->root;

    while(i)
    {
        cmp=str_comparision(name, ((Type*)i->data)->name);

        if(cmp==0)
            return (Type*)i->data;
        else if(cmp<0)
            i=(TreeNode*)i->left;
        else
            i=(TreeNode*)i->right;
    }

    return 0;
}

Type *find_global_type(Function *cur_function, Stack *functions, String *s)
{
    NodeStack *i=functions->begin;
    Function *data;
    Type *type;

    type=find_type(cur_function->types, s);
    if(type)
        return type;

    type=find_arg(cur_function->args, s);
    if(type)
        return type;

    while(i)
    {
        data=i->data;

        type=find_type(data->types, s);
        if(type==0)
            type=find_arg(data->args, s);

        if(type)
        {
            switch(type->type)
            {
                case INTEGER: ((Number*)type->data)->is_closure=1; break;
            }
            return type;
        }
        i=i->previouse;
    }

    return 0;
}

Type *new_type(String *name, int type, char *data)
{
    Type *ret=(Type*)malloc(sizeof(Type));

    ret->data=data;
    ret->name=name;
    ret->type=type;

    return ret;
}

Number* new_variable(String *name, int data)
{
    Number *ret=malloc(sizeof(Number));

    ret->data=data;
    ret->name=name;

    return ret;
}

Function* new_function(String *name)
{
    Function *ret=malloc(sizeof(Function));

    ret->name=name;
    ret->body=list_init();
    ret->pos=stack_init();
    ret->args=list_init();
    ret->types=tree_init();
    ret->functions=tree_init();

    return ret;
}

void add_type(Tree *types, char *data, String *name_data, int type)
{
    tree_add(types, (char*)new_type(name_data, type, data), tree_type_cmp);
}

void add_arg(List *args, char *data, String *name, int type)
{
    list_push(args, (char*)new_type(name, type, data));
}

char tree_contains(Function *cur_function, String *name)
{
    if(find_function(cur_function->functions, name) || find_type(cur_function->types, name) || find_arg(cur_function->args, name))
        return 1;

    return 0;
}

Data *run_alloc, *run_tmp;
Tree *fun;

Function *cur_function, *function_tmp;
String *new_string, *parser_string_code;

Type *type, *op1, *op2, *op3;

Tree *parse(String *s)
{
    parser_string_code=s;

    stack_functions=stack_init();
    fun=tree_init();

    tree_add(fun, (char*)new_function(str_init("")), function_cmp);
    cur_function=fun->root->data;
    cur_function->length_args=0;

    parser_table_init();
    parser_init_cond_table();

    while(s->length>0)
    {
        if(parser_table[str_pop(s)]()==ERROR)
        {
            parser_free_functions(fun);
            stack_free(stack_functions);
            return 0;
        }
    }

    if(stack_functions->begin)
    {
        printf("expected END\n");
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    if(debug)
        print_functions(fun);
    stack_free(stack_functions);

    return fun;
}
