#include "interpretator.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "String.h"
#include "interpretator_operations.h"
#include "interpretator_compiler.h"

char      *interpretator_stack;
char      *interpretator_stack_head;

char       loop_not_break;

Stack     *interpretator_return_place;
NodeStack *cur_loop;

void     (*interpretator_table[256])();

char run_function(List *list);
void interpretator_print(Type *putc_data);
void interpretator_element_var();

void interpretator_table_init()
{
    interpretator_table[JMP]=interpretator_jmp;
    interpretator_table[JZ]=interpretator_jz;
    interpretator_table[JNZ]=interpretator_jnz;
    interpretator_table[JE]=interpretator_je;
    interpretator_table[JNE]=interpretator_jne;
    interpretator_table[JLT]=interpretator_jlt;
    interpretator_table[JGT]=interpretator_jgt;
    interpretator_table[JLE]=interpretator_jle;
    interpretator_table[JGE]=interpretator_jge;

    interpretator_table[BREAK]=interpretator_break;
    interpretator_table[CONTINUE]=interpretator_continue;

    interpretator_table[CALL]=interpretator_call;

    interpretator_table[PRINT]=interpretator_putc;

    interpretator_table[INT_INT]=interpretator_var_var;
    interpretator_table[INT_CONST]=interpretator_var_const;
    interpretator_table[INT_ELEMENT]=interpretator_var_el;
    interpretator_table[INT_PTR]=interpretator_var_ptrs;
    interpretator_table[INT_ARRAY]=interpretator_var_array;
    interpretator_table[INT_CONST_STRING]=interpretator_var_const_string;

    interpretator_table[ELEMENT_INT]=interpretator_el_var;
    interpretator_table[ELEMENT_CONST]=interpretator_el_const;
    interpretator_table[ELEMENT_ELEMENT]=interpretator_el_el;
    interpretator_table[ELEMENT_PTR]=interpretator_el_ptrs;
    interpretator_table[ELEMENT_ARRAY]=interpretator_el_array;
    interpretator_table[ELEMENT_CONST_STRING]=interpretator_el_const_string;

    interpretator_table[PTR_INT]=interpretator_ptrs_var;
    interpretator_table[PTR_CONST]=interpretator_ptrs_const;
    interpretator_table[PTR_ELEMENT]=interpretator_ptrs_el;
    interpretator_table[PTR_PTR]=interpretator_ptrs_ptrs;
    interpretator_table[PTR_ARRAY]=interpretator_ptrs_array;
    interpretator_table[PTR_CONST_STRING]=interpretator_ptrs_const_string;

    interpretator_table[ARRAY_INT]=interpretator_array_var;
    interpretator_table[ARRAY_ARRAY]=interpretator_array_const;
    interpretator_table[ARRAY_ELEMENT]=interpretator_array_el;
    interpretator_table[ARRAY_PTR]=interpretator_array_ptrs;
    interpretator_table[ARRAY_ARRAY]=interpretator_array_array;
    interpretator_table[ARRAY_CONST_STRING]=interpretator_array_const_string;

    interpretator_table[INC]=interpretator_inc;
    interpretator_table[DEC]=interpretator_dec;

    interpretator_table[ADD]=interpretator_add;
    interpretator_table[SUB]=interpretator_sub;
    interpretator_table[MUL]=interpretator_mul;
    interpretator_table[DIV]=interpretator_div;

    interpretator_table[FADD]=interpretator_fadd;
    interpretator_table[FSUB]=interpretator_fsub;
    interpretator_table[FMUL]=interpretator_fmul;
    interpretator_table[FDIV]=interpretator_fdiv;

    interpretator_table[SHR]=interpretator_shr;
    interpretator_table[SHL]=interpretator_shl;

    interpretator_table[XOR]=interpretator_xor;
    interpretator_table[AND]=interpretator_and;
    interpretator_table[OR]=interpretator_or;
    interpretator_table[NOT]=interpretator_not;

    interpretator_table[ELEMENT_ALLOC]=interpretator_element_alloc;
    interpretator_table[ARRAY_ALLOC]=interpretator_array_alloc;
    interpretator_table[POINTERS_ALLOC]=interpretator_pointers_alloc;
}

void interpretator_init_local_vars(Type *type)
{
    Number *num;

    switch(type->type)
    {
        case INTEGER: case REAL:
            num=type->data;

            if(num->is_closure)
            {
                num->data=malloc(BITS);
                printf("[%d ", num->data); str_print(num->name); printf(" malloc]");
            }
            else
            {
                printf("[%d ", num->data); str_print(num->name); printf(" stack]");
                num->data=interpretator_stack_head;
                interpretator_stack_head+=BITS;
            }
            break;
    }
}

static void interpretator_closure_functions_init(Function *function)
{
    tree_print(function->types, interpretator_init_local_vars);
    tree_print(function->functions, interpretator_closure_functions_init);
}

Tree            *interpretator_tree   ;
struct ListNode *interpretator_next_op;
RunData         *data                 ;

void run(Tree *fun)
{
    Function *f;

    f=fun->root->data;

    interpretator_table_init();
    interpretator_return_place=stack_init();
    push(interpretator_return_place, 0);

    interpretator_tree=fun;
    interpretator_next_op=f->body->begin;

    interpretator_stack=malloc(STACK_LENGTH);
    interpretator_stack_head=interpretator_stack;

    tree_print(fun, interpretator_closure_functions_init);

    NodeStack *cur_pos;

    while(interpretator_next_op)
    {
        data=(RunData*)interpretator_next_op->data;
        interpretator_table[data->type]();

        if(interpretator_next_op==0)
            interpretator_next_op=pop(interpretator_return_place);
    }
}

void pure_run(Tree *fun)
{
    List *l;

    l=get_fast_run(fun->root->data);
    interpretator_table_init();
    interpretator_tree=fun;
    interpretator_next_op=l->begin;

    interpretator_stack=malloc(STACK_LENGTH);
    interpretator_stack_head=interpretator_stack;

    tree_print(fun, interpretator_closure_functions_init);

    NodeStack *cur_pos;

    while(interpretator_next_op)
    {
        data=(RunData*)interpretator_next_op->data;
        interpretator_table[data->type]();
    }
}
