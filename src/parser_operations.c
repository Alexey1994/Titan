#include "parser_operations.h"
#include "parser.h"
#include "parser_cmp.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

extern const char   ERROR,
                    OK;

extern Stack       *stack_functions;
extern RunData     *run_alloc, *run_tmp;
extern Tree        *fun;
extern Function    *cur_function,
                   *function_tmp;
extern String      *new_string;
extern Type        *type,
                   *op1,
                   *op2,
                   *op3;
extern String      *parser_string_code;
extern char         debug,
                    disasm;

int                 disasm_level=0;

Putc               *putc_alloc;

static Number      *var_alloc;
static Const       *const_alloc;
static Element     *element_alloc;
static Pointers    *ptrs_alloc;
static Array       *arr_alloc;
static ConstString *const_string_alloc;

ElementVar         *el_var_alloc;
ElementArray       *el_arr_alloc;
ElementConst       *el_const_alloc;
ElementConstString *el_const_string_alloc;
ElementPtr         *el_ptr_alloc;
ElementElement     *el_el_alloc;

VarElement         *var_el_alloc;
VarVar             *var_var_alloc;
VarArray           *var_arr_alloc;
VarConst           *var_const_alloc;
VarConstString     *var_const_string_alloc;
VarPtr             *var_ptr_alloc;

ArrayElement       *arr_el_alloc;
ArrayVar           *arr_var_alloc;
ArrayArray         *arr_arr_alloc;
ArrayConst         *arr_const_alloc;
ArrayConstString   *arr_const_string_alloc;
ArrayPtr           *arr_ptr_alloc;

PointerElement     *ptr_el_alloc;
PointerVar         *ptr_var_alloc;
PointerArray       *ptr_arr_alloc;
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

Increment          *INC_alloc;
Decrement          *DEC_alloc;

Add                *add_alloc;
Sub                *sub_alloc;
Mul                *mul_alloc;
Div                *div_alloc;

Shr                *shr_alloc;
Shl                *shl_alloc;

Xor                *xor_alloc;
And                *and_alloc;
Or                 *or_alloc;
Not                *not_alloc;

ElementAlloc       *el_alloc_alloc;
ArrayAlloc         *arr_alloc_alloc;
PointersAlloc      *ptrs_alloc_alloc;

static struct ListNode* add_body_element()
{
    if(cur_function->pos->begin)
    {
        run_tmp=(RunData*)cur_function->pos->begin->data;
        switch(run_tmp->type)
        {
        case LOOP:
            loop_tmp=(Loop*)run_tmp->data;
            list_push(loop_tmp->eval, (char*)run_alloc);
            return loop_tmp->eval->end;
            break;

        case IF:
            if_tmp=(If*)run_tmp->data;
            list_push(if_tmp->eval, (char*)run_alloc);
            return if_tmp->eval->end;
            break;
        }
    }
    else
    {
        list_push(cur_function->body, (char*)run_alloc);
        return cur_function->body->end;
    }
}

try parser_error()
{
    printf("file not valid\n");
    return ERROR;
}

try parser_putc()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
    {
        str_free(new_string);
        return ERROR;
    }

    putc_alloc=malloc(sizeof(Putc));
    putc_alloc->data=type;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("PRINT "); str_print(type->name); printf("\n");
    }

    run_alloc=new_run_data(PRINT, (char*)putc_alloc);
    add_body_element();
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
    if(new_string==0)
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
    if(new_string==0)
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

try parser_array_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(new_string==0)
    {
        str_free(new_string);
        return ERROR;
    }

    arr_alloc=malloc(sizeof(Array));
    arr_alloc->name=new_string;

    arr_alloc->data=0;
    arr_alloc->ilength=0;
    arr_alloc->isz=0;
    arr_alloc->uninitialized=1;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("ARRAY_INIT "); str_print(new_string); printf("\n");
    }

    add_type(cur_function->types, (char*)arr_alloc, new_string, ARRAY);
    return OK;
}

try parser_integer_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(new_string==0)
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
    if(new_string==0)
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
    if(new_string==0 || parser_string_code->length<BITS)
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
    if(new_string==0)
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
    List *expression;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("ASSIGNMENT ");
    }

    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
        return ERROR;

    if(disasm)
    {
        str_print(type->name);
        printf("=");
    }

    expression=get_parser_expression(parser_string_code, cur_function);
    if(!expression)
        return ERROR;

    assignment_data=malloc(sizeof(Assignment));
    assignment_data->expression=expression;
    assignment_data->left_operand=type;

/*
    switch(type->type)
    {
    case INTEGER:

        break;

    case REAL:
        break;

    case ELEMENT:
        break;

    case PTRS:
        break;
    }
*/

    run_alloc=(RunData*)new_run_data(ASSIGNMENT, (char*)assignment_data);
    add_body_element();
    printf("\n");
    return OK;
}

try parser_loop()
{
    loop_alloc=malloc(sizeof(Loop));
    loop_alloc->eval=list_init();
    run_alloc=(RunData*)new_run_data(LOOP, (char*)loop_alloc);

    add_body_element();

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

    run_alloc=(RunData*)new_run_data(IF, (char*)if_alloc);
    add_body_element();
    push(cur_function->pos, (char*)run_alloc);
    return OK;
}

try parser_break()
{
    break_alloc=malloc(sizeof(Break));

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("BREAK\n");
    }

    run_alloc=(RunData*)new_run_data(BREAK, break_alloc);
    break_alloc->loop=add_body_element();
    return OK;
}

try parser_continue()
{
    continue_alloc=malloc(sizeof(Continue));

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("CONTINUE\n");
    }

    run_alloc=(RunData*)new_run_data(CONTINUE, continue_alloc);
    continue_alloc->loop=add_body_element();
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
    int i;

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
        type=find_global_type(cur_function, stack_functions, new_string);
        if(type==0)
        {
            printf("argument '"); str_print(new_string); printf("' not found\n");
            str_free(new_string);
            return ERROR;
        }
        list_push(call_alloc->args, type);
        str_free(new_string);
        i--;
    }

    if(disasm)
    {
        for(i=0; i<disasm_level;i++) printf("    "); printf("CALL ");
        str_print(call_alloc->fun->name); printf("\n");
    }

    run_alloc=(RunData*)new_run_data(CALL, (char*)call_alloc);
    add_body_element();
    return OK;
}

try parser_inc()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
        return ERROR;

    INC_alloc=malloc(sizeof(Increment));

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("INC ");
        str_print(type->name); printf("\n");
    }

    INC_alloc->var=(Number*)type->data;
    run_alloc=(RunData*)new_run_data(INC, (char*)INC_alloc);
    add_body_element();
    return OK;
}

try parser_dec()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
        return ERROR;

    DEC_alloc=malloc(sizeof(Increment));

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("DEC ");
        str_print(type->name); printf("\n");
    }

    DEC_alloc->var=(Number*)type->data;
    run_alloc=(RunData*)new_run_data(DEC, (char*)DEC_alloc);
    add_body_element();
    return OK;
}

try parser_get_3op()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
        return ERROR;

    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
        return ERROR;

    op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op2==0)
        return ERROR;

    return OK;
}

try parser_add()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    add_alloc=malloc(sizeof(Add));

    add_alloc->var_rez=(Number*)type->data;
    add_alloc->var1=(Number*)op1->data;
    add_alloc->var2=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("ADD ");
        str_print(add_alloc->var_rez->name);
        printf("=");
        str_print(add_alloc->var1->name);
        printf("+");
        str_print(add_alloc->var2->name);
        printf("\n");
    }

    if(add_alloc->var_rez->type==INTEGER)
        run_alloc=(RunData*)new_run_data(ADD, (char*)add_alloc);
    else
        run_alloc=(RunData*)new_run_data(FADD, (char*)add_alloc);
    add_body_element();
    return OK;
}

try parser_sub()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    sub_alloc=malloc(sizeof(Sub));

    sub_alloc->var_rez=(Number*)type->data;
    sub_alloc->var1=(Number*)op1->data;
    sub_alloc->var2=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("SUB ");
        str_print(sub_alloc->var_rez->name);
        printf("=");
        str_print(sub_alloc->var1->name);
        printf("-");
        str_print(sub_alloc->var2->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(SUB, (char*)sub_alloc);
    add_body_element();
    return OK;
}

try parser_mul()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    mul_alloc=malloc(sizeof(Mul));

    mul_alloc->var_rez=(Number*)type->data;
    mul_alloc->var1=(Number*)op1->data;
    mul_alloc->var2=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("MUL ");
        str_print(mul_alloc->var_rez->name);
        printf("=");
        str_print(mul_alloc->var1->name);
        printf("*");
        str_print(mul_alloc->var2->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(MUL, (char*)mul_alloc);
    add_body_element();
    return OK;
}

try parser_div()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    div_alloc=malloc(sizeof(Div));

    div_alloc->var_rez=(Number*)type->data;
    div_alloc->var1=(Number*)op1->data;
    div_alloc->var2=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("DIV ");
        str_print(div_alloc->var_rez->name);
        printf("=");
        str_print(div_alloc->var1->name);
        printf("/");
        str_print(div_alloc->var2->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(DIV, (char*)div_alloc);
    add_body_element();
    return OK;
}

try parser_shr()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    shr_alloc=malloc(sizeof(Shr));

    shr_alloc->var_rez=(Number*)type->data;
    shr_alloc->var=(Number*)op1->data;
    shr_alloc->shift=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("SHR ");
        str_print(shr_alloc->var_rez->name);
        printf("=");
        str_print(shr_alloc->var->name);
        printf(">>");
        str_print(shr_alloc->shift->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(SHR, (char*)shr_alloc);
    add_body_element();
    return OK;
}

try parser_shl()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    shl_alloc=malloc(sizeof(Shl));

    shl_alloc->var_rez=(Number*)type->data;
    shl_alloc->var=(Number*)op1->data;
    shl_alloc->shift=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("SHL ");
        str_print(shl_alloc->var_rez->name);
        printf("=");
        str_print(shl_alloc->var->name);
        printf("<<");
        str_print(shl_alloc->shift->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(SHL, (char*)shl_alloc);
    add_body_element();
    return OK;
}

try parser_xor()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    xor_alloc=malloc(sizeof(Xor));

    xor_alloc->var_rez=(Number*)type->data;
    xor_alloc->var1=(Number*)op1->data;
    xor_alloc->var2=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("XOR ");
        str_print(xor_alloc->var_rez->name);
        printf("=");
        str_print(xor_alloc->var1->name);
        printf("^");
        str_print(xor_alloc->var2->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(XOR, (char*)xor_alloc);
    add_body_element();
    return OK;
}

try parser_and()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    and_alloc=malloc(sizeof(And));

    and_alloc->var_rez=(Number*)type->data;
    and_alloc->var1=(Number*)op1->data;
    and_alloc->var2=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("AND ");
        str_print(and_alloc->var_rez->name);
        printf("=");
        str_print(and_alloc->var1->name);
        printf("&");
        str_print(and_alloc->var2->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(AND, (char*)and_alloc);
    add_body_element();
    return OK;
}

try parser_or()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    or_alloc=malloc(sizeof(Or));

    or_alloc->var_rez=(Number*)type->data;
    or_alloc->var1=(Number*)op1->data;
    or_alloc->var2=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("OR ");
        str_print(or_alloc->var_rez->name);
        printf("=");
        str_print(or_alloc->var1->name);
        printf("|");
        str_print(or_alloc->var2->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(OR, (char*)or_alloc);
    add_body_element();
    return OK;
}

try parser_not()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
        return ERROR;

    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
        return ERROR;

    not_alloc=malloc(sizeof(Not));

    not_alloc->var_rez=(Number*)type->data;
    not_alloc->var=(Number*)op1->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("OR ");
        str_print(not_alloc->var_rez->name);
        printf("=!");
        str_print(not_alloc->var_rez->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(NOT, (char*)not_alloc);
    add_body_element();
    return OK;
}

try parser_alloc()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
        return ERROR;

    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
        return ERROR;

    switch(type->type)
    {
    case ELEMENT:
        el_alloc_alloc=malloc(sizeof(ElementAlloc));

        el_alloc_alloc->el=(Element*)type->data;
        el_alloc_alloc->sz=(Number*)op1->data;

        run_alloc=(RunData*)new_run_data(ELEMENT_ALLOC, (char*)el_alloc_alloc);
        break;

    case ARRAY:
        op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op2==0)
            return ERROR;

        arr_alloc_alloc=malloc(sizeof(ArrayAlloc));

        arr_alloc_alloc->arr=(Array*)type->data;
        arr_alloc_alloc->sz=(Number*)op1->data;
        arr_alloc_alloc->length=(Number*)op2->data;
        arr_alloc_alloc->arr->uninitialized=0;

        run_alloc=(RunData*)new_run_data(ARRAY_ALLOC, (char*)arr_alloc_alloc);
        break;

    case PTRS:
        ptrs_alloc_alloc=malloc(sizeof(PointersAlloc));

        ptrs_alloc_alloc->ptrs=(Pointers*)type->data;
        ptrs_alloc_alloc->length=(Number*)op1->data;
        ptrs_alloc_alloc->ptrs->uninitialized=0;

        run_alloc=(RunData*)new_run_data(POINTERS_ALLOC, (char*)ptrs_alloc_alloc);
        break;

    default:
        printf("ALLOC only for PTRS,ARRAY,ELEMENT\n");
        return ERROR;
    }

    add_body_element();
    return OK;
}
