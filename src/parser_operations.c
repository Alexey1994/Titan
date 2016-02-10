#include "parser_operations.h"
#include "parser.h"
#include "parser_cmp.h"
#include "parser_expression.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

extern const char   ERROR,
                    OK;

extern Stack       *stack_functions;
extern Tree        *fun;
extern Function    *cur_function,
                   *function_tmp;
extern String      *new_string;

extern String      *parser_string_code;
extern char         debug,
                    disasm;

int                 disasm_level=0;

static Number      *var_alloc;
static Const       *const_alloc;
static Element     *element_alloc;
static Pointers    *ptrs_alloc;
static ConstString *const_string_alloc;

ElementVar         *el_var_alloc;
ElementConst       *el_const_alloc;
ElementConstString *el_const_string_alloc;
ElementPtr         *el_ptr_alloc;
ElementElement     *el_el_alloc;

VarElement         *var_el_alloc;
VarVar             *var_var_alloc;
VarConst           *var_const_alloc;
VarConstString     *var_const_string_alloc;
VarPtr             *var_ptr_alloc;

PointerElement     *ptr_el_alloc;
PointerVar         *ptr_var_alloc;
PointerConst       *ptr_const_alloc;
PointerConstString *ptr_const_string_alloc;
PointerPtr         *ptr_ptr_alloc;

Loop               *loop_alloc,
                   *loop_tmp;
Break              *break_alloc;
Continue           *continue_alloc;
If                 *if_alloc,
                   *if_tmp;

Call               *call_alloc;

ElementAlloc       *el_alloc_alloc;
PointersAlloc      *ptrs_alloc_alloc;

static void add_body_element(Data *run_alloc)
{
    Data *run_tmp;

    if(cur_function->pos->begin)
    {
        run_tmp=(Data*)cur_function->pos->begin->data;
        switch(run_tmp->type)
        {
        case LOOP:
            loop_tmp=(Loop*)run_tmp->data;
            list_push(loop_tmp->eval, (char*)run_alloc);
            break;

        case IF:
            if_tmp=(If*)run_tmp->data;
            list_push(if_tmp->eval, (char*)run_alloc);
            break;
        }
    }
    else
        list_push(cur_function->body, (char*)run_alloc);
}

try parser_error()
{
    printf("file not valid\n");
    return ERROR;
}

try parser_print()
{
    Type  *op;
    Print *print_alloc;

    op=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op==0)
    {
        str_free(new_string);
        return ERROR;
    }

    print_alloc=malloc(sizeof(Print));
    print_alloc->data=op;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("PRINT "); str_print(op->name); printf("\n");
    }

    add_body_element(new_data(PRINT, (char*)print_alloc));
    return OK;
}

try parser_function()
{
    new_string=next_token(parser_string_code);
    function_tmp=new_function(new_string);

    function_tmp->length_args=get_num(parser_string_code);

    tree_add(cur_function->functions, (char*)function_tmp, function_cmp);
    /*
    if(tree_add(cur_function->functions, (char*)function_tmp, function_cmp))
    {
        printf("function "); str_print(new_string); printf(" defined\n");
        parser_free_function(function_tmp);
        return ERROR;
    }*/

    if(disasm)
    {
        int i;
        printf("\n");
        for(i=0; i<disasm_level; i++) printf("    "); printf("FUNCTION ");
        for(i=0; i<disasm_level; i++) printf("    ");str_print(new_string);
        printf(" %d args\n", function_tmp->length_args);
        disasm_level++;
    }

    add_type(cur_function->types, (char*)function_tmp, function_tmp->name, FUNCTION);

    push(stack_functions, (char*)cur_function);
    cur_function=function_tmp;
    return OK;
}

try parser_element_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(!new_string)
    {
        str_free(new_string);
        return ERROR;
    }

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("ELEMENT_INIT "); str_print(new_string); printf("\n");
    }

    element_alloc=malloc(sizeof(Element));

    element_alloc->name=new_string;
    element_alloc->uninitialized=1;
    element_alloc->isz=0;

    add_type(cur_function->types, (char*)element_alloc, new_string, ELEMENT);
    return OK;
}

try parser_ptrs_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(!new_string)
    {
        str_free(new_string);
        return ERROR;
    }

    ptrs_alloc=malloc(sizeof(Pointers));
    ptrs_alloc->name=new_string;

    ptrs_alloc->uninitialized=1;
    ptrs_alloc->data=0;
    ptrs_alloc->ilength=0;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("PTRS_INIT "); str_print(new_string); printf("\n");
    }

    add_type(cur_function->types, (char*)ptrs_alloc, new_string, PTRS);
    return OK;
}

try parser_integer_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(!new_string)
    {
        str_free(new_string);
        return ERROR;
    }

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("INTEGER_INIT "); str_print(new_string); printf("\n");
    }

    var_alloc=malloc(sizeof(Number));
    var_alloc->name=new_string;
    var_alloc->uninitialized=1;
    var_alloc->count=0;
    var_alloc->is_closure=0;
    var_alloc->type=INTEGER;

    if(cur_function->length_args)
    {
        add_arg(cur_function->args, (char*)var_alloc, new_string, INTEGER);
        cur_function->length_args--;
    }
    else
        add_type(cur_function->types, (char*)var_alloc, new_string, INTEGER);
    return OK;
}

try parser_real_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(!new_string)
    {
        str_free(new_string);
        return ERROR;
    }

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("REAL_INIT "); str_print(new_string); printf("\n");
    }

    var_alloc=malloc(sizeof(Number));
    var_alloc->name=new_string;
    var_alloc->uninitialized=1;
    var_alloc->count=0;
    var_alloc->is_closure=0;
    var_alloc->type=REAL;

    if(cur_function->length_args)
    {
        add_arg(cur_function->args, (char*)var_alloc, new_string, REAL);
        cur_function->length_args--;
    }
    else
        add_type(cur_function->types, (char*)var_alloc, new_string, REAL);
    return OK;
}

try parser_const_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(!new_string || parser_string_code->length<BITS)
    {
        str_free(new_string);
        return ERROR;
    }

    const_alloc=malloc(sizeof(Const));

    const_alloc->name=new_string;
    const_alloc->data=get_num(parser_string_code);

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("CONST_INIT "); str_print(new_string); printf(" %d\n", const_alloc->data);
    }

    add_type(cur_function->types, (char*)const_alloc, new_string, CONST);
    return OK;
}

try parser_const_string_init()
{
    int j;

    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(!new_string)
    {
        str_free(new_string);
        return ERROR;
    }

    const_string_alloc=malloc(sizeof(ConstString));
    const_string_alloc->name=new_string;

    const_string_alloc->isz=get_num(parser_string_code);
    const_string_alloc->data=malloc(const_string_alloc->isz);

    for(j=0; j<const_string_alloc->isz; j++)
        const_string_alloc->data[j]=str_pop(parser_string_code);

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("CONST_STRING_INIT "); str_print(new_string);
        printf(" \"");
        for(i=0; i<const_string_alloc->isz; i++)
            printf("%c", const_string_alloc->data[i]);
        printf("\"\n");
    }

    add_type(cur_function->types, (char*)const_string_alloc, new_string, CONST_STRING);
    return OK;
}

try parser_assignment()
{
    Assignment *assignment_data;
    List       *expression;
    Type       *op;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("ASSIGNMENT ");
    }

    op=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op==0)
        return ERROR;

    switch(op->type)
    {
    case INTEGER: case REAL:
        if(disasm)
        {
            str_print(op->name);
            printf("=");
        }

        expression=get_parser_expression(parser_string_code, cur_function);
        if(!expression)
            return ERROR;
        if(debug)
        {
            list_print(expression, parser_print_expression);
            printf("\n");
        }

        assignment_data=malloc(sizeof(Assignment));
        assignment_data->expression=expression;
        assignment_data->left_operand=op;
    }
    add_body_element(new_data(ASSIGNMENT, (char*)assignment_data));
    return OK;
}

try parser_loop()
{
    Data *run_alloc;

    loop_alloc=malloc(sizeof(Loop));
    loop_alloc->eval=list_init();
    run_alloc=(Data*)new_data(LOOP, (char*)loop_alloc);

    add_body_element(run_alloc);

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("LOOP\n");
        disasm_level++;
    }

    push(cur_function->pos, (char*)run_alloc);
    return OK;
}

try parser_if()
{
    Data *run_alloc;

    if_alloc=malloc(sizeof(If));

    if_alloc->cond=list_init();
    if_alloc->eval=list_init();

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("IF");
        disasm_level++;
    }

    if(get_if_conditions(if_alloc)==ERROR)
        return ERROR;

    if(disasm)
        printf("\n");

    run_alloc=(Data*)new_data(IF, (char*)if_alloc);
    add_body_element(run_alloc);
    push(cur_function->pos, (char*)run_alloc);
    return OK;
}

static char is_in_loop()
{
    NodeStack *i;

    i=cur_function->pos->begin;
    while(i)
    {
        if(((Data*)i->data)->type==LOOP)
            return 1;
        i=i->previouse;
        printf("d");
    }

    return 0;
}

try parser_break()
{
    if(!is_in_loop())
        return ERROR;

    break_alloc=malloc(sizeof(Break));

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("BREAK\n");
    }

    add_body_element(new_data(BREAK, 0));
    return OK;
}

try parser_continue()
{
    if(!is_in_loop())
        return ERROR;

    continue_alloc=malloc(sizeof(Continue));

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("CONTINUE\n");
    }

    add_body_element(new_data(CONTINUE, 0));
    return OK;
}

try parser_end()
{
    if(cur_function->pos->begin)
    {
        pop(cur_function->pos);
    }
    else if(stack_functions->begin)
    {
        if(cur_function->length_args)
        {
            printf("arguments for '"); str_print(cur_function->name); printf("' not initialized\n");
            return ERROR;
        }
        cur_function=(Function*)pop(stack_functions);
    }
    else
    {
        printf("END without begin\n");
        return ERROR;
    }

    if(disasm)
    {
        int i;
        disasm_level--;
        for(i=0; i<disasm_level; i++) printf("    "); printf("END\n\n");
    }
    return OK;
}

try parser_call()
{
    Type *op;
    int   i;

    if(cur_function->length_args)
    {
        printf("arguments for "); str_print(cur_function->name); printf(" not initialized");
        return ERROR;
    }

    new_string=next_token(parser_string_code);

    call_alloc=malloc(sizeof(Call));
    call_alloc->fun=find_global_function(cur_function, cur_function->functions, stack_functions, new_string);

    if(call_alloc->fun==0)
    {
        printf("function "); str_print(new_string); printf(" not found\n");
        str_free(new_string);
        free(call_alloc);
        return ERROR;
    }
    str_free(new_string);

    call_alloc->args=list_init();

    i=call_alloc->fun->args->length;
    while(i)
    {
        new_string=next_token(parser_string_code);
        op=find_global_type(cur_function, stack_functions, new_string);
        if(!op)
        {
            printf("argument '"); str_print(new_string); printf("' not found\n");
            str_free(new_string);
            return ERROR;
        }
        list_push(call_alloc->args, op);
        str_free(new_string);
        i--;
    }

    if(disasm)
    {
        for(i=0; i<disasm_level;i++) printf("    "); printf("CALL ");
        str_print(call_alloc->fun->name); printf("\n");
    }

    add_body_element(new_data(CALL, (char*)call_alloc));
    return OK;
}

try parser_alloc()
{
    Type    *op;
    Data    *run_alloc;
    List    *expression;

    if(debug)
        printf("ALLOC ");

    op=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(!op)
        return ERROR;

    if(debug)
    {
        str_print(op->name);
        printf("=new ");
    }

    switch(op->type)
    {
    case ELEMENT:
        el_alloc_alloc=malloc(sizeof(ElementAlloc));

        el_alloc_alloc->el=(Element*)op->data;
        //el_alloc_alloc->sz=(Number*)op2->data;

        run_alloc=(Data*)new_data(ELEMENT_ALLOC, (char*)el_alloc_alloc);
        break;

    case PTRS:
        expression=get_parser_expression(parser_string_code, cur_function);
        if(!expression)
            return ERROR;
        if(debug)
        {
            list_print(expression, parser_print_expression);
            printf("\n");
        }

        ptrs_alloc_alloc=malloc(sizeof(PointersAlloc));

        ptrs_alloc_alloc->ptrs=(Pointers*)op->data;

        //ptrs_alloc_alloc->length=(Number*)op2->data;
        ptrs_alloc_alloc->ptrs->uninitialized=0;

        run_alloc=(Data*)new_data(POINTERS_ALLOC, (char*)ptrs_alloc_alloc);
        break;

    default:
        printf("ALLOC only for PTRS and ELEMENT\n");
        return ERROR;
    }

    add_body_element(run_alloc);
    return OK;
}
