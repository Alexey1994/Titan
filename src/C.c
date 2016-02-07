#include "C.h"
#include "parser.h"
#include "C_operations.h"
#include <stdio.h>

void str_fprint(String *s);

FILE *c_file;

Number *c_var;

void c_define_all_variables(TreeNode *types)
{
    Type *data;

    if(types)
    {
        c_define_all_variables((TreeNode*)types->left);

        data=(Type*)types->data;
        switch(data->type)
        {
            case INTEGER:
                c_var=(Number*)data->data;

                fprintf(c_file, "int ");
                str_fprint(c_var->name);
                fprintf(c_file, ";");
                break;
        }

        c_define_all_variables((Tree*)types->right);
    }
}

void (*c_table[256])();

void c_table_init()
{
    c_table[LOOP]=c_loop;
    c_table[IF]=c_if;

    c_table[BREAK]=c_break;
    c_table[CONTINUE]=c_continue;

    c_table[CALL]=c_call;

    c_table[PRINT]=c_putc;

    c_table[INT_INT]=c_var_var;
    c_table[INT_CONST]=c_var_const;
    c_table[INT_ELEMENT]=c_var_el;
    c_table[INT_PTR]=c_var_ptrs;
    c_table[INT_ARRAY]=c_var_array;
    c_table[INT_CONST_STRING]=c_var_const_string;

    c_table[ELEMENT_INT]=c_el_var;
    c_table[ELEMENT_CONST]=c_el_const;
    c_table[ELEMENT_ELEMENT]=c_el_el;
    c_table[ELEMENT_PTR]=c_el_ptrs;
    c_table[ELEMENT_ARRAY]=c_el_array;
    c_table[ELEMENT_CONST]=c_el_const_string;

    c_table[PTR_INT]=c_ptrs_var;
    c_table[PTR_CONST]=c_ptrs_const;
    c_table[PTR_ELEMENT]=c_ptrs_el;
    c_table[PTR_PTR]=c_ptrs_ptrs;
    c_table[PTR_ARRAY]=c_ptrs_array;
    c_table[PTR_CONST_STRING]=c_ptrs_const_string;

    c_table[ARRAY_INT]=c_array_var;
    c_table[ARRAY_ARRAY]=c_array_const;
    c_table[ARRAY_ELEMENT]=c_array_el;
    c_table[ARRAY_PTR]=c_array_ptrs;
    c_table[ARRAY_ARRAY]=c_array_array;
    c_table[ARRAY_CONST_STRING]=c_array_const_string;

    c_table[INC]=c_inc;
    c_table[DEC]=c_dec;

    c_table[ADD]=c_add;
    c_table[SUB]=c_sub;
    c_table[MUL]=c_mul;
    c_table[DIV]=c_div;

    c_table[SHR]=c_shr;
    c_table[SHL]=c_shl;

    c_table[XOR]=c_xor;
    c_table[AND]=c_and;
    c_table[OR]=c_or;
    c_table[NOT]=c_not;

    c_table[ELEMENT_ALLOC]=c_element_alloc;
    c_table[ARRAY_ALLOC]=c_array_alloc;
    c_table[POINTERS_ALLOC]=c_pointers_alloc;
}

Tree *c_tree;
struct ListNode *c_next_op;
RunData *c_data;

void translate_C(Tree *functions)
{
    Function *f=(Function*)functions->root->data;

    c_table_init();
    c_tree=functions;
    c_next_op=f->body->begin;

    c_file=fopen("Test C/out.c", "wb");

    fprintf(c_file, "void print_var(int v){printf(\"%%d\\n\", v);}");

    fprintf(c_file, "int main(){");
    c_define_all_variables(f->types);
    while(c_next_op)
    {
        c_data=(RunData*)c_next_op->data;
        c_table[(int)c_data->type]();
        c_next_op=c_next_op->next;
    }
    fprintf(c_file, "}");

    fclose(c_file);
}
