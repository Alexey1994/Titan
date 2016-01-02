#include "parser_operations.h"
#include "parser.h"
#include "parser_cmp.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

extern Stack *stack_functions;
extern RunData *run_alloc, *run_tmp;
extern Tree *fun;
extern Function *cur_function, *function_tmp;
extern String *new_string;
extern Type *type, *op1, *op2, *op3;
extern String *parser_string_code;
extern char debug;

Putc *putc_alloc;

Variable *var_alloc;
Const *const_alloc;
Element *element_alloc;
Pointers *ptrs_alloc;
Array *arr_alloc;
Pointers *ptrs_alloc;
Element *element_alloc;
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

char parser_putc()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
    {
        str_free(new_string);
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    putc_alloc=malloc(sizeof(Putc));

    putc_alloc->data=type;

    run_alloc=new_run_data(PUTC, (char*)putc_alloc);
    add_body_element();
    return 1;
}

char parser_element_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(new_string==0)
    {
        str_free(new_string);
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    element_alloc=malloc(sizeof(Element));

    element_alloc->name=new_string;
    element_alloc->uninitialized=1;
    element_alloc->isz=0;

    add_type(cur_function, (char*)element_alloc, new_string, ELEMENT);
    return 1;
}

char parser_ptrs_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(new_string==0)
    {
        str_free(new_string);
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    ptrs_alloc=malloc(sizeof(Pointers));
    ptrs_alloc->name=new_string;

    ptrs_alloc->uninitialized=1;
    ptrs_alloc->data=0;
    ptrs_alloc->ilength=0;

    add_type(cur_function, (char*)ptrs_alloc, new_string, PTRS);
    return 1;
}

char parser_array_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(new_string==0)
    {
        str_free(new_string);
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    arr_alloc=malloc(sizeof(Array));
    arr_alloc->name=new_string;

    arr_alloc->data=0;
    arr_alloc->ilength=0;
    arr_alloc->isz=0;
    arr_alloc->uninitialized=1;

    add_type(cur_function, (char*)arr_alloc, new_string, ARRAY);
    return 1;
}

char parser_var_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(new_string==0)
    {
        str_free(new_string);
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    var_alloc=malloc(sizeof(Variable));
    var_alloc->name=new_string;
    var_alloc->uninitialized=1;
    var_alloc->count=0;

    add_type(cur_function, (char*)var_alloc, new_string, VARIABLE);
    return 1;
}

char parser_const_init()
{
    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(new_string==0)
    {
        str_free(new_string);
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    const_alloc=malloc(sizeof(Const));

    const_alloc->name=new_string;
    const_alloc->data=get_num(parser_string_code);

    add_type(cur_function, (char*)const_alloc, new_string, CONST);
    return 1;
}

char parser_const_string_init()
{
    int j;

    new_string=get_parser_init_name(parser_string_code, cur_function);
    if(new_string==0)
    {
        str_free(new_string);
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    const_string_alloc=malloc(sizeof(ConstString));
    const_string_alloc->name=new_string;

    const_string_alloc->isz=get_num(parser_string_code);
    const_string_alloc->data=malloc(const_string_alloc->isz);

    for(j=0; j<const_string_alloc->isz; j++)
        const_string_alloc->data[j]=str_pop(parser_string_code);

    add_type(cur_function, (char*)const_string_alloc, new_string, CONST_STRING);
    return 1;
}

char parser_assignment()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    switch(type->type)
    {
    case ELEMENT:
    {
        op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op1==0)
        {
            parser_free_functions(fun);
            stack_free(stack_functions);
            return 0;
        }

        switch(op1->type)
        {
        case CONST:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            el_const_alloc=malloc(sizeof(ElementConst));

            el_const_alloc->el=(Element*)type->data;
            el_const_alloc->index=(Variable*)op2->data;
            el_const_alloc->in=(Const*)op1->data;

            run_alloc=(RunData*)new_run_data(ELEMENT_CONST, (char*)el_const_alloc);
            break;

        case CONST_STRING:

            el_const_string_alloc=malloc(sizeof(ElementConstString));

            el_const_string_alloc->el=(Element*)type->data;
            el_const_string_alloc->in=(String*)op1->data;

            run_alloc=(RunData*)new_run_data(ELEMENT_CONST_STRING, (char*)el_const_string_alloc);
            break;

        case VARIABLE:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            el_var_alloc=malloc(sizeof(ElementVar));

            el_var_alloc->el=(Element*)type->data;
            el_var_alloc->index=(Variable*)op2->data;
            el_var_alloc->in=(Variable*)op1->data;
            el_var_alloc->el->isz=BITS;

            run_alloc=(RunData*)new_run_data(ELEMENT_VAR, (char*)el_var_alloc);
            break;

        case ARRAY:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            el_arr_alloc=malloc(sizeof(ElementArray));

            el_arr_alloc->el=(Element*)type->data;
            el_arr_alloc->arr=(Array*)op1->data;
            el_arr_alloc->index=(Variable*)op2->data;
            el_arr_alloc->el->isz=el_arr_alloc->arr->isz;

            run_alloc=(RunData*)new_run_data(ELEMENT_ARRAY, (char*)el_arr_alloc);
            break;

        case PTRS:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            el_ptr_alloc=malloc(sizeof(ElementPtr));

            el_ptr_alloc->el=(Element*)type->data;
            el_ptr_alloc->ptrs=(Pointers*)op1->data;
            el_ptr_alloc->index=(Variable*)op2->data;
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

    case VARIABLE:
    {
        op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op1==0)
        {
            parser_free_functions(fun);
            stack_free(stack_functions);
            return 0;
        }

        switch(op1->type)
        {
        case CONST:
            var_const_alloc=malloc(sizeof(VarConst));

            var_const_alloc->var=(Variable*)type->data;
            var_const_alloc->var->uninitialized=0;
            var_const_alloc->in=(Const*)op1->data;

            run_alloc=(RunData*)new_run_data(VAR_CONST, (char*)var_const_alloc);
            break;

        case CONST_STRING:
            var_const_string_alloc=malloc(sizeof(VarConstString));

            var_const_string_alloc->var=(Variable*)type->data;
            var_const_string_alloc->var->uninitialized=0;
            var_const_string_alloc->in=(ConstString*)op1->data;

            run_alloc=(RunData*)new_run_data(VAR_CONST_STRING, (char*)var_const_string_alloc);
            break;

        case VARIABLE:
            var_var_alloc=malloc(sizeof(VarVar));

            var_var_alloc->var=(Variable*)type->data;
            var_var_alloc->in=(Variable*)op1->data;
            if(var_var_alloc->in->uninitialized==0)
                var_var_alloc->var->uninitialized=0;

            run_alloc=(RunData*)new_run_data(VAR_VAR, (char*)var_var_alloc);
            break;

        case ARRAY:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            var_arr_alloc=malloc(sizeof(VarArray));

            var_arr_alloc->var=(Variable*)type->data;
            var_arr_alloc->index=(Variable*)op2->data;
            var_arr_alloc->arr=(Array*)op1->data;
            if(var_arr_alloc->arr->uninitialized)
            {
                printf("array '%s' is uninitialized\n", var_arr_alloc->arr->name);
                parser_free_functions(fun);
                stack_free(stack_functions);
                free(var_arr_alloc);
                return 0;
            }

            var_arr_alloc->var->uninitialized=0;

            run_alloc=(RunData*)new_run_data(VAR_ARRAY, (char*)var_arr_alloc);
            break;

        case PTRS:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            var_ptr_alloc=malloc(sizeof(VarPtr));

            var_ptr_alloc->var=(Variable*)type->data;
            var_ptr_alloc->var->uninitialized=0;
            var_ptr_alloc->index=(Variable*)op2->data;
            var_ptr_alloc->ptrs=(Pointers*)op1->data;
            if(var_ptr_alloc->ptrs->uninitialized==0)
                var_ptr_alloc->var->uninitialized=0;

            run_alloc=(RunData*)new_run_data(VAR_PTR, (char*)var_ptr_alloc);
            break;

        case ELEMENT:
            op2=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op2==0)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            var_el_alloc=malloc(sizeof(ElementConst));

            var_el_alloc->var=(Variable*)type->data;
            var_el_alloc->in=(Element*)op1->data;
            var_el_alloc->index=(Variable*)op2->data;
            if(var_el_alloc->in->uninitialized==0)
                var_el_alloc->var->uninitialized=0;

            run_alloc=(RunData*)new_run_data(VAR_ELEMENT, (char*)var_el_alloc);
            break;
        }

        break;
    }

    case ARRAY:
    {
        op1=(Type*)get_parser_op(parser_string_code, cur_function);
        if(op1==0)
        {
            parser_free_functions(fun);
            stack_free(stack_functions);
            return 0;
        }

        op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op2==0)
        {
            parser_free_functions(fun);
            stack_free(stack_functions);
            return 0;
        }

        switch(op2->type)
        {
        case CONST:
            arr_const_alloc=malloc(sizeof(ArrayConst));

            arr_const_alloc->arr=(Array*)type->data;
            arr_const_alloc->index=(Variable*)op1->data;
            arr_const_alloc->in=(Const*)op2->data;

            run_alloc=(RunData*)new_run_data(ARRAY_CONST, (char*)arr_const_alloc);
            break;

        case CONST_STRING:
            arr_const_string_alloc=malloc(sizeof(ArrayConstString));

            arr_const_string_alloc->arr=(Array*)type->data;
            arr_const_string_alloc->index=(Variable*)op1->data;
            arr_const_string_alloc->in=(ConstString*)op2->data;

            run_alloc=(RunData*)new_run_data(ARRAY_CONST_STRING, (char*)arr_const_string_alloc);
            break;

        case VARIABLE:
            arr_var_alloc=malloc(sizeof(ArrayVar));

            arr_var_alloc->arr=(Array*)type->data;
            arr_var_alloc->index=(Variable*)op1->data;
            arr_var_alloc->var=(Variable*)op2->data;

            run_alloc=(RunData*)new_run_data(ARRAY_VAR, (char*)arr_var_alloc);
            break;

        case ARRAY:
            op3=(Type*)get_parser_op(parser_string_code, cur_function);
            if(op3==0)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            arr_arr_alloc=malloc(sizeof(ArrayArray));

            arr_arr_alloc->arr=(Array*)type->data;
            arr_arr_alloc->index=(Variable*)op1->data;
            arr_arr_alloc->arr_in=(Array*)op2->data;
            arr_arr_alloc->index_in=(Variable*)op3->data;

            run_alloc=(RunData*)new_run_data(ARRAY_ARRAY, (char*)arr_arr_alloc);
            break;

        case PTRS:
            arr_ptr_alloc=malloc(sizeof(ArrayPtr));

            arr_ptr_alloc->arr=(Array*)type->data;
            arr_ptr_alloc->ptrs=(Pointers*)op2->data;
            arr_ptr_alloc->index=(Variable*)op1->data;

            run_alloc=(RunData*)new_run_data(ARRAY_PTR, (char*)arr_ptr_alloc);
            break;

        case ELEMENT:
            arr_el_alloc=malloc(sizeof(ArrayElement));

            arr_el_alloc->arr=(Array*)type->data;
            arr_el_alloc->index=(Variable*)op1->data;
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
        {
            parser_free_functions(fun);
            stack_free(stack_functions);
            return 0;
        }

        op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op2==0)
        {
            parser_free_functions(fun);
            stack_free(stack_functions);
            return 0;
        }

        switch(op2->type)
        {
        case CONST:
            ptr_const_alloc=malloc(sizeof(PointerConst));

            ptr_const_alloc->ptrs=(Pointers*)type->data;
            ptr_const_alloc->index=(Variable*)op1->data;
            ptr_const_alloc->in=(Const*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_CONST, (char*)ptr_const_alloc);
            break;

        case CONST_STRING:
            ptr_const_string_alloc=malloc(sizeof(PointerConstString));

            ptr_const_string_alloc->ptrs=(Pointers*)type->data;
            ptr_const_string_alloc->index=(Variable*)op1->data;
            ptr_const_string_alloc->in=(ConstString*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_CONST_STRING, (char*)ptr_const_string_alloc);
            break;

        case VARIABLE:
            ptr_var_alloc=malloc(sizeof(PointerVar));

            ptr_var_alloc->ptrs=(Pointers*)type->data;
            ptr_var_alloc->index=(Variable*)op1->data;
            ptr_var_alloc->var=(Variable*)op2->data;

            ((Variable*)op2->data)->count++;

            run_alloc=(RunData*)new_run_data(PTR_VAR, (char*)ptr_var_alloc);
            break;

        case ARRAY:
            ptr_arr_alloc=malloc(sizeof(PointerArray));

            ptr_arr_alloc->ptrs=(Pointers*)type->data;
            ptr_arr_alloc->index=(Variable*)op1->data;
            ptr_arr_alloc->arr=(Array*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_ARRAY, (char*)ptr_arr_alloc);
            break;

        case PTRS:
            if(type->data==op2->data)
            {
                parser_free_functions(fun);
                stack_free(stack_functions);
                return 0;
            }

            ptr_ptr_alloc=malloc(sizeof(PointerPtr));

            ptr_ptr_alloc->ptrs=(Pointers*)type->data;
            ptr_ptr_alloc->index=(Variable*)op1->data;
            ptr_ptr_alloc->ptrs_in=(Pointers*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_PTR, (char*)ptr_ptr_alloc);
            break;

        case ELEMENT:
            ptr_el_alloc=malloc(sizeof(PointerElement));

            ptr_el_alloc->ptrs=(Pointers*)type->data;
            ptr_el_alloc->index=(Variable*)op1->data;
            ptr_el_alloc->in=(Element*)op2->data;

            run_alloc=(RunData*)new_run_data(PTR_ELEMENT, (char*)ptr_el_alloc);
            break;
        }

        break;
    }
    }

    add_body_element();
    return 1;
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

    if(debug)
    {
        printf("<loop>\n");
    }

    push(cur_function->pos, (char*)run_alloc);
    return 1;
}

char parser_if()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    if_alloc=malloc(sizeof(If));

    if_alloc->cond=(Variable*)type->data;
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

    push(cur_function->pos, (char*)run_alloc);
    return 1;
}

char parser_break()
{
    break_alloc=malloc(sizeof(Break));

    run_alloc=(RunData*)new_run_data(BREAK, break_alloc);
    break_alloc->loop=add_body_element();
    return 1;
}

char parser_continue()
{
    continue_alloc=malloc(sizeof(Continue));

    run_alloc=(RunData*)new_run_data(CONTINUE, continue_alloc);
    continue_alloc->loop=add_body_element();
    return 1;
}

char parser_function()
{
    new_string=next_token(parser_string_code);
    function_tmp=new_function(new_string);

    if(cur_function->functions)
    {
        if(tree_add(cur_function->functions, (char*)function_tmp, function_cmp))
        {
            printf("function ");
            str_print(new_string);
            printf(" defined\n");
            parser_free_functions(fun);
            stack_free(stack_functions);
            parser_free_function(function_tmp);
            return 0;
        }
    }
    else
    {
        cur_function->functions=tree_init((char*)function_tmp);
    }

    push(stack_functions, (char*)cur_function);
    cur_function=function_tmp;
    return 1;
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
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }
    return 1;
}

char parser_call()
{
    new_string=next_token(parser_string_code);

    call_alloc=malloc(sizeof(Call));
    call_alloc->fun=find_global_function(cur_function->functions, stack_functions, new_string);
    if(call_alloc->fun==0)
    {
        printf("function ");
        str_print(new_string);
        printf(" not found\n");
        stack_free(stack_functions);
        parser_free_functions(fun);
        str_free(new_string);
        free(call_alloc);
        return 0;
    }
    str_free(new_string);

    run_alloc=(RunData*)new_run_data(CALL, (char*)call_alloc);
    add_body_element();
    return 1;
}

char parser_inc()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    INC_alloc=malloc(sizeof(Increment));

    INC_alloc->var=(Variable*)type->data;
    run_alloc=(RunData*)new_run_data(INC, (char*)INC_alloc);
    add_body_element();
    return 1;
}

char parser_dec()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    DEC_alloc=malloc(sizeof(Increment));

    DEC_alloc->var=(Variable*)type->data;
    run_alloc=(RunData*)new_run_data(DEC, (char*)DEC_alloc);
    add_body_element();
    return 1;
}

char parser_get_3op()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
        return 0;

    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
        return 0;

    op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op2==0)
        return 0;
    return 1;
}

char parser_add()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    add_alloc=malloc(sizeof(Add));

    add_alloc->var_rez=(Variable*)type->data;
    add_alloc->var1=(Variable*)op1->data;
    add_alloc->var2=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(ADD, (char*)add_alloc);
    add_body_element();
    return 1;
}

char parser_sub()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    sub_alloc=malloc(sizeof(Sub));

    sub_alloc->var_rez=(Variable*)type->data;
    sub_alloc->var1=(Variable*)op1->data;
    sub_alloc->var2=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(SUB, (char*)sub_alloc);
    add_body_element();
    return 1;
}

char parser_mul()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    mul_alloc=malloc(sizeof(Mul));

    mul_alloc->var_rez=(Variable*)type->data;
    mul_alloc->var1=(Variable*)op1->data;
    mul_alloc->var2=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(MUL, (char*)mul_alloc);
    add_body_element();
    return 1;
}

char parser_div()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    div_alloc=malloc(sizeof(Div));

    div_alloc->var_rez=(Variable*)type->data;
    div_alloc->var1=(Variable*)op1->data;
    div_alloc->var2=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(DIV, (char*)div_alloc);
    add_body_element();
    return 1;
}

char parser_shr()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    shr_alloc=malloc(sizeof(Shr));

    shr_alloc->var_rez=(Variable*)type->data;
    shr_alloc->var=(Variable*)op1->data;
    shr_alloc->shift=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(SHR, (char*)shr_alloc);
    add_body_element();
    return 1;
}

char parser_shl()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    shl_alloc=malloc(sizeof(Shl));

    shl_alloc->var_rez=(Variable*)type->data;
    shl_alloc->var=(Variable*)op1->data;
    shl_alloc->shift=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(SHL, (char*)shl_alloc);
    add_body_element();
    return 1;
}

char parser_xor()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    xor_alloc=malloc(sizeof(Xor));

    xor_alloc->var_rez=(Variable*)type->data;
    xor_alloc->var1=(Variable*)op1->data;
    xor_alloc->var2=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(XOR, (char*)xor_alloc);
    add_body_element();
    return 1;
}

char parser_and()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    and_alloc=malloc(sizeof(And));

    and_alloc->var_rez=(Variable*)type->data;
    and_alloc->var1=(Variable*)op1->data;
    and_alloc->var2=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(AND, (char*)and_alloc);
    add_body_element();
    return 1;
}

char parser_or()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    or_alloc=malloc(sizeof(Or));

    or_alloc->var_rez=(Variable*)type->data;
    or_alloc->var1=(Variable*)op1->data;
    or_alloc->var2=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(OR, (char*)or_alloc);
    add_body_element();
    return 1;
}

char parser_not()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    not_alloc=malloc(sizeof(Not));

    not_alloc->var_rez=(Variable*)type->data;
    not_alloc->var=(Variable*)op1->data;

    run_alloc=(RunData*)new_run_data(NOT, (char*)not_alloc);
    add_body_element();
    return 1;
}

char parser_eq()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    eq_alloc=malloc(sizeof(Equal));

    eq_alloc->var=(Variable*)type->data;
    eq_alloc->left=(Variable*)op1->data;
    eq_alloc->right=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(EQ, (char*)eq_alloc);
    add_body_element();
    return 1;
}

char parser_neq()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    neq_alloc=malloc(sizeof(NotEqual));

    neq_alloc->var=(Variable*)type->data;
    neq_alloc->left=(Variable*)op1->data;
    neq_alloc->right=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(NEQ, (char*)neq_alloc);
    add_body_element();
    return 1;
}

char parser_gt()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    gt_alloc=malloc(sizeof(GreatherThan));

    gt_alloc->var=(Variable*)type->data;
    gt_alloc->left=(Variable*)op1->data;
    gt_alloc->right=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(GT, (char*)gt_alloc);
    add_body_element();
    return 1;
}

char parser_lt()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    lt_alloc=malloc(sizeof(LesserThan));

    lt_alloc->var=(Variable*)type->data;
    lt_alloc->left=(Variable*)op1->data;
    lt_alloc->right=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(LT, (char*)lt_alloc);
    add_body_element();
    return 1;
}

char parser_ge()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    ge_alloc=malloc(sizeof(GreatherThanOrEqual));

    ge_alloc->var=(Variable*)type->data;
    ge_alloc->left=(Variable*)op1->data;
    ge_alloc->right=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(GE, (char*)ge_alloc);
    add_body_element();
    return 1;
}

char parser_le()
{
    if(parser_get_3op()==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    le_alloc=malloc(sizeof(LesserThanOrEqual));

    le_alloc->var=(Variable*)type->data;
    le_alloc->left=(Variable*)op1->data;
    le_alloc->right=(Variable*)op2->data;

    run_alloc=(RunData*)new_run_data(LE, (char*)le_alloc);
    add_body_element();
    return 1;
}

char parser_alloc()
{
    type=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(type==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
    {
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    switch(type->type)
    {
    case ELEMENT:
        el_alloc_alloc=malloc(sizeof(ElementAlloc));

        el_alloc_alloc->el=(Element*)type->data;
        el_alloc_alloc->sz=(Variable*)op1->data;

        run_alloc=(RunData*)new_run_data(ELEMENT_ALLOC, (char*)el_alloc_alloc);
        break;

    case ARRAY:
        op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
        if(op2==0)
        {
            parser_free_functions(fun);
            stack_free(stack_functions);
            return 0;
        }

        arr_alloc_alloc=malloc(sizeof(ArrayAlloc));

        arr_alloc_alloc->arr=(Array*)type->data;
        arr_alloc_alloc->sz=(Variable*)op1->data;
        arr_alloc_alloc->length=(Variable*)op2->data;
        arr_alloc_alloc->arr->uninitialized=0;

        run_alloc=(RunData*)new_run_data(ARRAY_ALLOC, (char*)arr_alloc_alloc);
        break;

    case PTRS:
        ptrs_alloc_alloc=malloc(sizeof(PointersAlloc));

        ptrs_alloc_alloc->ptrs=(Pointers*)type->data;
        ptrs_alloc_alloc->length=(Variable*)op1->data;
        ptrs_alloc_alloc->ptrs->uninitialized=0;

        run_alloc=(RunData*)new_run_data(POINTERS_ALLOC, (char*)ptrs_alloc_alloc);
        break;

    default:
        printf("ALLOC only for PTRS,ARRAY,ELEMENT\n");
        parser_free_functions(fun);
        stack_free(stack_functions);
        return 0;
    }

    add_body_element();
    return 1;
}
