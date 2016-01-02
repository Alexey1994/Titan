#include "parser.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "Id.h"
#include "parser_print.h"
#include "parser_cmp.h"
#include "parser_free.h"
#include "parser_operations.h"

extern char debug;

Stack *stack_functions;
Type *new_type(String *name, int type, char *data);

char (*parser_table[256])();

char parser_error()
{
    printf("file not valid\n");
    return 0;
}

void parser_table_init()
{
    int i;
    for(i=0; i<256; i++)
        parser_table[i]=parser_error;

    parser_table[PUTC]=parser_putc;

    parser_table[ELEMENT_INIT]=parser_element_init;
    parser_table[PTRS_INIT]=parser_ptrs_init;
    parser_table[ARRAY_INIT]=parser_array_init;
    parser_table[VAR_INIT]=parser_var_init;
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

    parser_table[INC]=parser_inc;
    parser_table[DEC]=parser_dec;

    parser_table[ADD]=parser_add;
    parser_table[SUB]=parser_sub;
    parser_table[MUL]=parser_mul;
    parser_table[DIV]=parser_div;

    parser_table[SHR]=parser_shr;
    parser_table[SHL]=parser_shl;

    parser_table[XOR]=parser_xor;
    parser_table[AND]=parser_and;
    parser_table[OR]=parser_or;
    parser_table[NOT]=parser_not;

    parser_table[EQ]=parser_eq;
    parser_table[NEQ]=parser_neq;
    parser_table[GT]=parser_gt;
    parser_table[LT]=parser_lt;
    parser_table[GE]=parser_ge;
    parser_table[LE]=parser_le;

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
            if(!t) break;
            str_push(ret, t);
        }

        return ret;
    }
    return 0;
}

int get_num(String *s)
{
    int i,ret=1;

    if(s->length<BITS)
    {
        str_push(s, 0);
        printf("size of constant lesser BITS\n");
        return 0;
    }

    for(i=0; i<BITS; i++)
        ((char*)&ret)[i]=str_pop(s);
    return ret;
}

Type *get_parser_op(String *s, Function *cur_function)
{
    String *new_string;
    Type *op;

    new_string=next_token(s);
    op=find_global_type(cur_function->types, stack_functions, new_string);

    if(op==0 || op->type!=VARIABLE)
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
        printf("variable '");
        str_print(new_string);
        printf("' not found\n");
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

RunData *new_run_data(char type, char *data)
{
    RunData *ret=malloc(sizeof(RunData));

    ret->data=data;
    ret->type=type;

    return ret;
}

Function* find_function(Tree *tree_cur_function, String *s)
{
    Tree *i=tree_cur_function;
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

Function *find_global_function(Tree *tree_cur_function, Stack *functions, String *s)
{
    NodeStack *i=functions->begin;
    Function *data;

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

Type *find_global_type(Function *cur_function, Stack *functions, String *s)
{
    NodeStack *i=functions->begin;
    Function *data;
    Type *type;

    type=find_type(cur_function->types, s);
    if(type)
        return type;

    while(i)
    {
        data=i->data;
        type=find_type(data->types, s);
        if(type)
            return data;
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

Variable* new_variable(String *name, int data)
{
    Variable *ret=malloc(sizeof(Variable));

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
    ret->types=0;
    ret->functions=0;

    return ret;
}

Type *find_type(Tree *types, String *name)
{
    int cmp;

    while(types)
    {
        cmp=str_comparision(name, ((Type*)types->data)->name);

        if(cmp==0)
            return (Type*)types->data;
        else if(cmp<0)
            types=(Tree*)types->left;
        else
            types=(Tree*)types->right;
    }

    return 0;
}

void add_type(Function *cur_function, char *data, String *name_data, int type)
{
    if(cur_function->types)
        tree_add(cur_function->types, (char*)new_type(name_data, type, data), tree_type_cmp);
    else
        cur_function->types=tree_init((char*)new_type(name_data, type, data));
}

char tree_contains(Function *cur_function, String *name)
{
    if(find_function(cur_function->functions, name) || find_type(cur_function->types, name))
        return 1;

    return 0;
}

RunData *run_alloc, *run_tmp;
Tree *fun;

Function *cur_function, *function_tmp;
String *new_string, *parser_string_code;

Type *type, *op1, *op2, *op3;


Tree *parse(String *s)
{
    parser_string_code=s;

    stack_functions=stack_init();
    fun=tree_init((char*)new_function(str_init("")));
    cur_function=fun->data;

    parser_table_init();

    while(s->length>0)
        if(parser_table[str_pop(s)]()==0)
            return 0;

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