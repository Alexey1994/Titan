#include "C_operations.h"
#include "String.h"
#include "parser.h"
#include <stdio.h>

extern Data *c_data;

extern FILE *c_file;

void str_fprint(String *s)
{
    struct StringNode *i=s->begin;
    int k;

    while(i)
    {
        /*
        if(i!=s->end)
            for(k=0; k<STRING_BUF_SZ; k++)
                fputc(i->data[k], c_file);
        else
            for(k=0; k<s->pos_end; k++)*/
                //fputc(i->data, c_file);

        //i=i->next;
    }
}

void c_loop()
{

}

void c_if()
{

}

void c_break()
{

}

void c_continue()
{

}

void c_call()
{

}

void c_putc()
{
    Print *print_data=(Print*)c_data->data;
    Type *data=print_data->data;

    Number *var;

    switch(data->type)
    {
        case INTEGER:
            var=(Number*)data->data;
            fprintf(c_file, "print_var(");
            str_fprint(var->name);
            fprintf(c_file, ");");
            break;
    }
}

void c_var_var()
{

}

void c_var_const()
{
    VarConst *var_const_data=(VarConst*)c_data->data;

    str_fprint(var_const_data->var->name);
    fprintf(c_file, "=%d;", var_const_data->in->data);
}

void c_var_el()
{

}

void c_var_ptrs()
{

}

void c_var_array()
{

}

void c_var_const_string()
{

}

void c_el_var()
{

}

void c_el_const()
{

}

void c_el_el()
{

}

void c_el_ptrs()
{

}

void c_el_array()
{

}

void c_el_const_string()
{

}

void c_ptrs_var()
{

}

void c_ptrs_const()
{

}

void c_ptrs_el()
{

}

void c_ptrs_ptrs()
{

}

void c_ptrs_array()
{

}

void c_ptrs_const_string()
{

}

void c_array_var()
{

}

void c_array_const()
{

}

void c_array_el()
{

}

void c_array_ptrs()
{

}

void c_array_array()
{

}

void c_array_const_string()
{

}

void c_inc()
{
    Increment *inc_data=(Increment*)c_data->data;

    str_fprint(inc_data->var->name);
    fprintf(c_file, "++;");
}

void c_dec()
{
    Decrement *dec_data=(Decrement*)c_data->data;

    str_fprint(dec_data->var->name);
    fprintf(c_file, "--;");
}

void c_add()
{
    Add *add_data=(Add*)c_data->data;

    str_fprint(add_data->var_rez->name);
    fprintf(c_file, "=");
    str_fprint(add_data->var1->name);
    fprintf(c_file, "+");
    str_fprint(add_data->var2->name);
    fprintf(c_file, ";");
}

void c_sub()
{
    Sub *sub_data=(Sub*)c_data->data;

    str_fprint(sub_data->var_rez->name);
    fprintf(c_file, "=");
    str_fprint(sub_data->var1->name);
    fprintf(c_file, "-");
    str_fprint(sub_data->var2->name);
    fprintf(c_file, ";");
}

void c_mul()
{
    Mul *mul_data=(Mul*)c_data->data;

    str_fprint(mul_data->var_rez->name);
    fprintf(c_file, "=");
    str_fprint(mul_data->var1->name);
    fprintf(c_file, "*");
    str_fprint(mul_data->var2->name);
    fprintf(c_file, ";");
}

void c_div()
{
    Div *div_data=(Div*)c_data->data;

    str_fprint(div_data->var_rez->name);
    fprintf(c_file, "=");
    str_fprint(div_data->var1->name);
    fprintf(c_file, "/");
    str_fprint(div_data->var2->name);
    fprintf(c_file, ";");
}

void c_shr()
{

}

void c_shl()
{

}

void c_xor()
{

}

void c_and()
{

}

void c_or()
{

}

void c_not()
{

}

void c_eq()
{

}

void c_neq()
{

}

void c_gt()
{

}

void c_lt()
{

}

void c_ge()
{

}

void c_le()
{

}

void c_element_alloc()
{

}

void c_array_alloc()
{

}

void c_pointers_alloc()
{

}
