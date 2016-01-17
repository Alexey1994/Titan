#include "parser_operations.h"
#include "parser.h"
#include "parser_cmp.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

extern const char ERROR, OK;

extern Stack *stack_functions;
extern RunData *run_alloc, *run_tmp;
extern Tree *fun;
extern Function *cur_function, *function_tmp;
extern String *new_string;
extern Type *type, *op1, *op2, *op3;
extern String *parser_string_code;
extern char debug, disasm;

int disasm_level=0;

Putc *putc_alloc;

Number *var_alloc;
Const *const_alloc;
Element *element_alloc;
Pointers *ptrs_alloc;
Array *arr_alloc;
ConstString *const_string_alloc;

ElementVar *el_var_alloc;
ElementArray *el_arr_alloc;
ElementConst *el_const_alloc;
ElementConstString *el_const_string_alloc;
ElementPtr *el_ptr_alloc;
ElementElement *el_el_alloc;

VarElement *var_el_alloc;
VarVar *var_var_alloc;
VarArray *var_arr_alloc;
VarConst *var_const_alloc;
VarConstString *var_const_string_alloc;
VarPtr *var_ptr_alloc;

ArrayElement *arr_el_alloc;
ArrayVar *arr_var_alloc;
ArrayArray *arr_arr_alloc;
ArrayConst *arr_const_alloc;
ArrayConstString *arr_const_string_alloc;
ArrayPtr *arr_ptr_alloc;

PointerElement *ptr_el_alloc;
PointerVar *ptr_var_alloc;
PointerArray *ptr_arr_alloc;
PointerConst *ptr_const_alloc;
PointerConstString *ptr_const_string_alloc;
PointerPtr *ptr_ptr_alloc;

Loop *loop_alloc, *loop_tmp;
Break *break_alloc;
Continue *continue_alloc;
If *if_alloc, *if_tmp;

Call *call_alloc;

Increment *INC_alloc;
Decrement *DEC_alloc;

Add *add_alloc;
Sub *sub_alloc;
Mul *mul_alloc;
Div *div_alloc;

Shr *shr_alloc;
Shl *shl_alloc;

Xor *xor_alloc;
And *and_alloc;
Or *or_alloc;
Not *not_alloc;

Equal *eq_alloc;
NotEqual *neq_alloc;
GreatherThan *gt_alloc;
LesserThan *lt_alloc;
GreatherThanOrEqual *ge_alloc;
LesserThanOrEqual *le_alloc;

ElementAlloc *el_alloc_alloc;
ArrayAlloc *arr_alloc_alloc;
PointersAlloc *ptrs_alloc_alloc;

struct ListNode* add_body_element()
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

char parser_error()
{
    printf("file not valid\n");
    return ERROR;
}

char parser_putc()
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
        for(i=0; i<disasm_level; i++) printf("    "); printf("PUTC "); str_print(type->name); printf("\n");
    }

    run_alloc=new_run_data(PUTC, (char*)putc_alloc);
    add_body_element();
    return OK;
}

char parser_element_init()
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

    add_type(cur_function, (char*)element_alloc, new_string, ELEMENT);
    return OK;
}

char parser_ptrs_init()
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

    add_type(cur_function, (char*)ptrs_alloc, new_string, PTRS);
    return OK;
}

char parser_array_init()
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

    add_type(cur_function, (char*)arr_alloc, new_string, ARRAY);
    return OK;
}

char parser_var_init()
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
        for(i=0; i<disasm_level; i++) printf("    "); printf("NUMBER_INIT "); str_print(new_string); printf("\n");
    }

    var_alloc=malloc(sizeof(Number));
    var_alloc->name=new_string;
    var_alloc->uninitialized=1;
    var_alloc->count=0;
    var_alloc->is_closure=0;

    add_type(cur_function, (char*)var_alloc, new_string, INTEGER);
    return OK;
}

char parser_const_init()
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

    add_type(cur_function, (char*)const_alloc, new_string, CONST);
    return OK;
}

char parser_const_string_init()
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
        for(i=0; i<disasm_level; i++) printf("    "); printf("CONST_STRING_INIT "); str_print(new_string); printf("\"%s\"\n", const_string_alloc->data);
    }

    add_type(cur_function, (char*)const_string_alloc, new_string, CONST_STRING);
    return OK;
}

char parser_assignment()
{
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

    switch(type->type)
    {
    case ELEMENT:
    {
        op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op1==0)
            return ERROR;

        if(disasm)
        {
            str_print(op1->name);
        }

        switch(op1->type)
        {
        case CONST:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
                return ERROR;

            el_const_alloc=malloc(sizeof(ElementConst));

            el_const_alloc->el=(Element*)type->data;
            el_const_alloc->index=(Number*)op2->data;
            el_const_alloc->in=(Const*)op1->data;

            run_alloc=(RunData*)new_run_data(ELEMENT_CONST, (char*)el_const_alloc);
            break;

        case CONST_STRING:

            el_const_string_alloc=malloc(sizeof(ElementConstString));

            el_const_string_alloc->el=(Element*)type->data;
            el_const_string_alloc->in=(String*)op1->data;

            run_alloc=(RunData*)new_run_data(ELEMENT_CONST_STRING, (char*)el_const_string_alloc);
            break;

        case INTEGER:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
                return ERROR;

            el_var_alloc=malloc(sizeof(ElementVar));

            el_var_alloc->el=(Element*)type->data;
            el_var_alloc->index=(Number*)op2->data;
            el_var_alloc->in=(Number*)op1->data;
            el_var_alloc->el->isz=BITS;

            run_alloc=(RunData*)new_run_data(ELEMENT_INT, (char*)el_var_alloc);
            break;

        case ARRAY:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
                return ERROR;

            el_arr_alloc=malloc(sizeof(ElementArray));

            el_arr_alloc->el=(Element*)type->data;
            el_arr_alloc->arr=(Array*)op1->data;
            el_arr_alloc->index=(Number*)op2->data;
            el_arr_alloc->el->isz=el_arr_alloc->arr->isz;

            run_alloc=(RunData*)new_run_data(ELEMENT_ARRAY, (char*)el_arr_alloc);
            break;

        case PTRS:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
                return ERROR;

            el_ptr_alloc=malloc(sizeof(ElementPtr));

            el_ptr_alloc->el=(Element*)type->data;
            el_ptr_alloc->ptrs=(Pointers*)op1->data;
            el_ptr_alloc->index=(Number*)op2->data;
            el_ptr_alloc->el->isz=1;

            run_alloc=(RunData*)new_run_data(ELEMENT_PTR, (char*)el_ptr_alloc);
            break;

        case ELEMENT:
            el_el_alloc=malloc(sizeof(ElementElement));

            el_el_alloc->el=(Element*)type->data;
            el_el_alloc->in=(Element*)op1->data;
            el_el_alloc->el->isz=el_el_alloc->in->isz;

            run_alloc=(RunData*)new_run_data(ELEMENT_ELEMENT, (char*)el_el_alloc);
            break;
        }

        break;
    }

    case INTEGER:
    {
        op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op1==0)
            return ERROR;

        if(disasm)
            str_print(op1->name);

        switch(op1->type)
        {
        case CONST:
            if(disasm)
                printf("\n");

            var_const_alloc=malloc(sizeof(VarConst));

            var_const_alloc->var=(Number*)type->data;
            var_const_alloc->var->uninitialized=0;
            var_const_alloc->in=(Const*)op1->data;

            run_alloc=(RunData*)new_run_data(INT_CONST, (char*)var_const_alloc);
            break;

        case CONST_STRING:
            var_const_string_alloc=malloc(sizeof(VarConstString));

            var_const_string_alloc->var=(Number*)type->data;
            var_const_string_alloc->var->uninitialized=0;
            var_const_string_alloc->in=(ConstString*)op1->data;

            run_alloc=(RunData*)new_run_data(INT_CONST_STRING, (char*)var_const_string_alloc);
            break;

        case INTEGER:
            if(disasm)
                printf("\n");

            var_var_alloc=malloc(sizeof(VarVar));

            var_var_alloc->var=(Number*)type->data;
            var_var_alloc->in=(Number*)op1->data;
            if(var_var_alloc->in->uninitialized==0)
                var_var_alloc->var->uninitialized=0;

            run_alloc=(RunData*)new_run_data(INT_INT, (char*)var_var_alloc);
            break;

        case ARRAY:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
                return ERROR;

            var_arr_alloc=malloc(sizeof(VarArray));

            var_arr_alloc->var=(Number*)type->data;
            var_arr_alloc->index=(Number*)op2->data;
            var_arr_alloc->arr=(Array*)op1->data;
            if(var_arr_alloc->arr->uninitialized)
            {
                printf("array '%s' is uninitialized\n", var_arr_alloc->arr->name);
                free(var_arr_alloc);
                return ERROR;
            }

            var_arr_alloc->var->uninitialized=0;

            run_alloc=(RunData*)new_run_data(INT_ARRAY, (char*)var_arr_alloc);
            break;

        case PTRS:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
                return ERROR;

            var_ptr_alloc=malloc(sizeof(VarPtr));

            var_ptr_alloc->var=(Number*)type->data;
            var_ptr_alloc->var->uninitialized=0;
            var_ptr_alloc->index=(Number*)op2->data;
            var_ptr_alloc->ptrs=(Pointers*)op1->data;
            if(var_ptr_alloc->ptrs->uninitialized==0)
                var_ptr_alloc->var->uninitialized=0;

            run_alloc=(RunData*)new_run_data(INT_PTR, (char*)var_ptr_alloc);
            break;

        case ELEMENT:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
                return ERROR;

            var_el_alloc=malloc(sizeof(ElementConst));

            var_el_alloc->var=(Number*)type->data;
            var_el_alloc->in=(Element*)op1->data;
            var_el_alloc->index=(Number*)op2->data;
            if(var_el_alloc->in->uninitialized==0)
                var_el_alloc->var->uninitialized=0;

            run_alloc=(RunData*)new_run_data(INT_ELEMENT, (char*)var_el_alloc);
            break;
        }

        break;
    }

    case ARRAY:
    {
        op1=(Type*)get_parser_op(parser_string_code, cur_function);
        if(op1==0)
            return ERROR;

        op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op2==0)
            return ERROR;

        switch(op2->type)
        {
        case CONST:
            arr_const_alloc=malloc(sizeof(ArrayConst));

            arr_const_alloc->arr=(Array*)type->data;
            arr_const_alloc->index=(Number*)op1->data;
            arr_const_alloc->in=(Const*)op2->data;

            run_alloc=(RunData*)new_run_data(ARRAY_CONST, (char*)arr_const_alloc);
            break;

        case CONST_STRING:
            arr_const_string_alloc=malloc(sizeof(ArrayConstString));

            arr_const_string_alloc->arr=(Array*)type->data;
            arr_const_string_alloc->index=(Number*)op1->data;
            arr_const_string_alloc->in=(ConstString*)op2->data;

            run_alloc=(RunData*)new_run_data(ARRAY_CONST_STRING, (char*)arr_const_string_alloc);
            break;

        case INTEGER:
            arr_var_alloc=malloc(sizeof(ArrayVar));

            arr_var_alloc->arr=(Array*)type->data;
            arr_var_alloc->index=(Number*)op1->data;
            arr_var_alloc->var=(Number*)op2->data;

            run_alloc=(RunData*)new_run_data(ARRAY_INT, (char*)arr_var_alloc);
            break;

        case ARRAY:
            op3=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op3==0)
                return ERROR;

            arr_arr_alloc=malloc(sizeof(ArrayArray));

            arr_arr_alloc->arr=(Array*)type->data;
            arr_arr_alloc->index=(Number*)op1->data;
            arr_arr_alloc->arr_in=(Array*)op2->data;
            arr_arr_alloc->index_in=(Number*)op3->data;

            run_alloc=(RunData*)new_run_data(ARRAY_ARRAY, (char*)arr_arr_alloc);
            break;

        case PTRS:
            arr_ptr_alloc=malloc(sizeof(ArrayPtr));

            arr_ptr_alloc->arr=(Array*)type->data;
            arr_ptr_alloc->ptrs=(Pointers*)op2->data;
            arr_ptr_alloc->index=(Number*)op1->data;

            run_alloc=(RunData*)new_run_data(ARRAY_PTR, (char*)arr_ptr_alloc);
            break;

        case ELEMENT:
            arr_el_alloc=malloc(sizeof(ArrayElement));

            arr_el_alloc->arr=(Array*)type->data;
            arr_el_alloc->index=(Number*)op1->data;
            arr_el_alloc->in=(Element*)op2->data;

            run_alloc=(RunData*)new_run_data(ARRAY_ELEMENT, (char*)arr_el_alloc);
            break;
        }

        break;
    }

    case PTRS:
    {
        op1=(Type*)get_parser_op(parser_string_code, cur_function);
        if(op1==0)
            return ERROR;

        op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op2==0)
            return ERROR;

        switch(op2->type)
        {
        case CONST:
            ptr_const_alloc=malloc(sizeof(PointerConst));

            ptr_const_alloc->ptrs=(Pointers*)type->data;
            ptr_const_alloc->index=(Number*)op1->data;
            ptr_const_alloc->in=(Const*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_CONST, (char*)ptr_const_alloc);
            break;

        case CONST_STRING:
            ptr_const_string_alloc=malloc(sizeof(PointerConstString));

            ptr_const_string_alloc->ptrs=(Pointers*)type->data;
            ptr_const_string_alloc->index=(Number*)op1->data;
            ptr_const_string_alloc->in=(ConstString*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_CONST_STRING, (char*)ptr_const_string_alloc);
            break;

        case INTEGER:
            ptr_var_alloc=malloc(sizeof(PointerVar));

            ptr_var_alloc->ptrs=(Pointers*)type->data;
            ptr_var_alloc->index=(Number*)op1->data;
            ptr_var_alloc->var=(Number*)op2->data;

            ((Number*)op2->data)->count++;

            run_alloc=(RunData*)new_run_data(PTR_INT, (char*)ptr_var_alloc);
            break;

        case ARRAY:
            ptr_arr_alloc=malloc(sizeof(PointerArray));

            ptr_arr_alloc->ptrs=(Pointers*)type->data;
            ptr_arr_alloc->index=(Number*)op1->data;
            ptr_arr_alloc->arr=(Array*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_ARRAY, (char*)ptr_arr_alloc);
            break;

        case PTRS:
            if(type->data==op2->data)
                return ERROR;

            ptr_ptr_alloc=malloc(sizeof(PointerPtr));

            ptr_ptr_alloc->ptrs=(Pointers*)type->data;
            ptr_ptr_alloc->index=(Number*)op1->data;
            ptr_ptr_alloc->ptrs_in=(Pointers*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_PTR, (char*)ptr_ptr_alloc);
            break;

        case ELEMENT:
            ptr_el_alloc=malloc(sizeof(PointerElement));

            ptr_el_alloc->ptrs=(Pointers*)type->data;
            ptr_el_alloc->index=(Number*)op1->data;
            ptr_el_alloc->in=(Element*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_ELEMENT, (char*)ptr_el_alloc);
            break;
        }

        break;
    }
    }

    add_body_element();
    return OK;
}

char parser_loop()
{
    loop_alloc=malloc(sizeof(Loop));
    loop_alloc->eval=list_init();
    run_alloc=(RunData*)new_run_data(LOOP, (char*)loop_alloc);

    if(cur_function->pos->begin)
    {
        run_tmp=(RunData*)cur_function->pos->begin->data;
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

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("LOOP\n");
        disasm_level++;
    }

    push(cur_function->pos, (char*)run_alloc);
    return OK;
}

char parser_if()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
        return ERROR;

    if_alloc=malloc(sizeof(If));

    if_alloc->cond=(Number*)type->data;
    if_alloc->eval=list_init();

    run_alloc=(RunData*)new_run_data(IF, (char*)if_alloc);

    if(cur_function->pos->begin)
    {
        run_tmp=(RunData*)cur_function->pos->begin->data;
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

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("IF");
        str_print(type->name); printf("\n");
        disasm_level++;
    }

    push(cur_function->pos, (char*)run_alloc);
    return OK;
}

char parser_break()
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

char parser_continue()
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

char parser_function()
{
    new_string=next_token(parser_string_code);
    function_tmp=new_function(new_string);

    if(cur_function->functions)
    {
        if(tree_add(cur_function->functions, (char*)function_tmp, function_cmp))
        {
            printf("function "); str_print(new_string); printf(" defined\n");
            parser_free_function(function_tmp);
            return ERROR;
        }
    }
    else
    {
        cur_function->functions=tree_init((char*)function_tmp);
    }

    if(disasm)
    {
        int i;
        printf("\n");
        for(i=0; i<disasm_level; i++) printf("    "); printf("FUNCTION ");
        for(i=0; i<disasm_level; i++) printf("    ");str_print(new_string);
        printf("\n");
        disasm_level++;
    }

    push(stack_functions, (char*)cur_function);
    cur_function=function_tmp;
    return OK;
}

char parser_end()
{
    if(cur_function->pos->begin)
    {
        pop(cur_function->pos);
    }
    else if(stack_functions->begin)
    {
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

char parser_call()
{
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

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level;i++) printf("    "); printf("CALL ");
        str_print(call_alloc->fun->name); printf("\n");
    }

    run_alloc=(RunData*)new_run_data(CALL, (char*)call_alloc);
    add_body_element();
    return OK;
}

char parser_inc()
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

char parser_dec()
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

char parser_get_3op()
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

char parser_add()
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

    run_alloc=(RunData*)new_run_data(ADD, (char*)add_alloc);
    add_body_element();
    return OK;
}

char parser_sub()
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

char parser_mul()
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

char parser_div()
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

char parser_shr()
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

char parser_shl()
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

char parser_xor()
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

char parser_and()
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

char parser_or()
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

char parser_not()
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

char parser_eq()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    eq_alloc=malloc(sizeof(Equal));

    eq_alloc->var=(Number*)type->data;
    eq_alloc->left=(Number*)op1->data;
    eq_alloc->right=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("EQ ");
        str_print(eq_alloc->var->name);
        printf("=");
        str_print(eq_alloc->left->name);
        printf("==");
        str_print(eq_alloc->right->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(EQ, (char*)eq_alloc);
    add_body_element();
    return OK;
}

char parser_neq()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    neq_alloc=malloc(sizeof(NotEqual));

    neq_alloc->var=(Number*)type->data;
    neq_alloc->left=(Number*)op1->data;
    neq_alloc->right=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("NEQ ");
        str_print(neq_alloc->var->name);
        printf("=");
        str_print(neq_alloc->left->name);
        printf("!=");
        str_print(neq_alloc->right->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(NEQ, (char*)neq_alloc);
    add_body_element();
    return OK;
}

char parser_gt()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    gt_alloc=malloc(sizeof(GreatherThan));

    gt_alloc->var=(Number*)type->data;
    gt_alloc->left=(Number*)op1->data;
    gt_alloc->right=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("GT ");
        str_print(gt_alloc->var->name);
        printf("=");
        str_print(gt_alloc->left->name);
        printf(">");
        str_print(gt_alloc->right->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(GT, (char*)gt_alloc);
    add_body_element();
    return OK;
}

char parser_lt()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    lt_alloc=malloc(sizeof(LesserThan));

    lt_alloc->var=(Number*)type->data;
    lt_alloc->left=(Number*)op1->data;
    lt_alloc->right=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("LT ");
        str_print(lt_alloc->var->name);
        printf("=");
        str_print(lt_alloc->left->name);
        printf("<");
        str_print(lt_alloc->right->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(LT, (char*)lt_alloc);
    add_body_element();
    return OK;
}

char parser_ge()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    ge_alloc=malloc(sizeof(GreatherThanOrEqual));

    ge_alloc->var=(Number*)type->data;
    ge_alloc->left=(Number*)op1->data;
    ge_alloc->right=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("GE ");
        str_print(ge_alloc->var->name);
        printf("=");
        str_print(ge_alloc->left->name);
        printf(">=");
        str_print(ge_alloc->right->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(GE, (char*)ge_alloc);
    add_body_element();
    return OK;
}

char parser_le()
{
    if(parser_get_3op()==ERROR)
        return ERROR;

    le_alloc=malloc(sizeof(LesserThanOrEqual));

    le_alloc->var=(Number*)type->data;
    le_alloc->left=(Number*)op1->data;
    le_alloc->right=(Number*)op2->data;

    if(disasm)
    {
        int i;
        for(i=0; i<disasm_level; i++) printf("    "); printf("LE ");
        str_print(le_alloc->var->name);
        printf("=");
        str_print(le_alloc->left->name);
        printf("<=");
        str_print(le_alloc->right->name);
        printf("\n");
    }

    run_alloc=(RunData*)new_run_data(LE, (char*)le_alloc);
    add_body_element();
    return OK;
}

char parser_alloc()
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
