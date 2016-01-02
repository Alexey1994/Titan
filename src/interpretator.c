#include "interpretator.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "String.h"
#include "interpretator_operations.h"

char run_function(List *list);

void interpretator_print(Type *putc_data);
void interpretator_element_var();

char loop_not_break;

Stack *interpretator_return_place;
NodeStack *cur_loop;

void (*interpretator_table[256])();

void interpretator_table_init()
{
    interpretator_table[LOOP]=interpretator_loop;
    interpretator_table[IF]=interpretator_if;

    interpretator_table[BREAK]=interpretator_break;
    interpretator_table[CONTINUE]=interpretator_continue;

    interpretator_table[CALL]=interpretator_call;

    interpretator_table[PUTC]=interpretator_putc;

    interpretator_table[VAR_VAR]=interpretator_var_var;
    interpretator_table[VAR_CONST]=interpretator_var_const;
    interpretator_table[VAR_ELEMENT]=interpretator_var_el;
    interpretator_table[VAR_PTR]=interpretator_var_ptrs;
    interpretator_table[VAR_ARRAY]=interpretator_var_array;
    interpretator_table[VAR_CONST_STRING]=interpretator_var_const_string;

    interpretator_table[ELEMENT_VAR]=interpretator_el_var;
    interpretator_table[ELEMENT_CONST]=interpretator_el_const;
    interpretator_table[ELEMENT_ELEMENT]=interpretator_el_el;
    interpretator_table[ELEMENT_PTR]=interpretator_el_ptrs;
    interpretator_table[ELEMENT_ARRAY]=interpretator_el_array;
    interpretator_table[ELEMENT_CONST_STRING]=interpretator_el_const_string;

    interpretator_table[PTR_VAR]=interpretator_ptrs_var;
    interpretator_table[PTR_CONST]=interpretator_ptrs_const;
    interpretator_table[PTR_ELEMENT]=interpretator_ptrs_el;
    interpretator_table[PTR_PTR]=interpretator_ptrs_ptrs;
    interpretator_table[PTR_ARRAY]=interpretator_ptrs_array;
    interpretator_table[PTR_CONST_STRING]=interpretator_ptrs_const_string;

    interpretator_table[ARRAY_VAR]=interpretator_array_var;
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

    interpretator_table[SHR]=interpretator_shr;
    interpretator_table[SHL]=interpretator_shl;

    interpretator_table[XOR]=interpretator_xor;
    interpretator_table[AND]=interpretator_and;
    interpretator_table[OR]=interpretator_or;
    interpretator_table[NOT]=interpretator_not;

    interpretator_table[EQ]=interpretator_eq;
    interpretator_table[NEQ]=interpretator_neq;
    interpretator_table[GT]=interpretator_gt;
    interpretator_table[LT]=interpretator_lt;
    interpretator_table[GE]=interpretator_ge;
    interpretator_table[LE]=interpretator_le;

    interpretator_table[ELEMENT_ALLOC]=interpretator_element_alloc;
    interpretator_table[ARRAY_ALLOC]=interpretator_array_alloc;
    interpretator_table[POINTERS_ALLOC]=interpretator_pointers_alloc;
}

Tree *interpretator_tree;
struct ListNode *interpretator_next_op;
RunData *data;

void run(Tree *fun)
{
    Function *f=fun->data;
    interpretator_table_init();
    interpretator_return_place=stack_init();

    interpretator_tree=fun;
    interpretator_next_op=f->body->begin;

    NodeStack *cur_pos;

    while(1)
    {
        while(interpretator_next_op==0)
        {
            if(interpretator_return_place->begin==0)
            {
                stack_free(interpretator_return_place);
                return;
            }
            interpretator_next_op=pop(interpretator_return_place);
        }

        data=(RunData*)interpretator_next_op->data;
        interpretator_table[data->type]();
    }
}
