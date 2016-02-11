#include "interpretator_operations.h"
#include "interpretator_compiler.h"
#include "../parser/parser.h"
#include "interpretator.h"
#include <stdio.h>
#include <stdlib.h>

void interpretator_init_local_vars(Type *type);

extern char        *interpretator_stack;
extern char        *interpretator_stack_head;

extern Data        *data;
extern Tree        *interpretator_tree;

extern char         loop_not_break;
extern struct       ListNode *interpretator_next_op;
extern NodeStack   *cur_loop;

extern Stack       *interpretator_return_place;

extern char         debug;

Loop               *loop_data;
Increment          *inc_data;
Decrement          *dec_data;
Call               *call_data;
If                 *if_data;

Add                *add_data;
Sub                *sub_data;
Mul                *mul_data;
Div                *div_data;

ElementVar         *el_var_data;
ElementConst       *el_const_data;
ElementConstString *el_const_string_data;
ElementPtr         *el_ptr_data;
ElementElement     *el_el_data;

VarElement         *var_el_data;
VarVar             *var_var_data;
VarConst           *var_const_data;
VarConstString     *var_const_string_data;
VarPtr             *var_ptr_data;

PointerElement     *ptr_el_data;
PointerVar         *ptr_var_data;
PointerConst       *ptr_const_data;
PointerConstString *ptr_const_string_data;
PointerPtr         *ptr_ptr_data;

ElementAlloc       *el_alloc_data;
PointersAlloc      *ptrs_alloc;

Const              *const_tmp;

Type               *putc_data;

Number             *var;
Const              *constant;
Element            *element;
ConstString        *const_string;
Pointers           *ptrs;

void interpretator_jmp()
{
    Jmp *jmp_data=data->data;
    if(debug)
        printf("<JMP %d>\n", jmp_data->place);
    interpretator_next_op=jmp_data->place;
}

void interpretator_jz()
{
    Jz *jz_data=data->data;

    if(debug)
    {
        printf("<JZ ");
        str_print(jz_data->var->name);
        printf(" %d>\n", jz_data->place);
    }

    if(!(*(int*)jz_data->var->data))
        interpretator_next_op=jz_data->place;
    else
        interpretator_next_op=interpretator_next_op->next;
}

void interpretator_jnz()
{
    Jnz *jnz_data=data->data;

    if(debug)
    {
        printf("<JZ ");
        str_print(jnz_data->var->name);
        printf(" %d>\n", jnz_data->place);
    }

    if(*(int*)jnz_data->var->data)
        interpretator_next_op=jnz_data->place;
    else
        interpretator_next_op=interpretator_next_op->next;
}

void interpretator_je()
{

}

void interpretator_jne()
{

}

void interpretator_jlt()
{

}

void interpretator_jgt()
{

}

void interpretator_jle()
{

}

void interpretator_jge()
{

}

void interpretator_break()
{
    Break *break_data=data->data;
    interpretator_next_op=break_data->loop;
}

void interpretator_continue()
{
    Continue *continue_data=data->data;
    interpretator_next_op=continue_data->loop;
}

void update_args()
{

}

void interpretator_call()
{
    struct ListNode *i,
                    *j;
    Number          *num,
                    *num2;
    Type            *type,
                    *type2;

    call_data=(Call*)data->data;

    i=call_data->args->begin;
    j=call_data->fun->args->begin;
    while(i)
    {
        type=j->data;
        type2=i->data;

        switch(type->type)
        {
            case INTEGER: case REAL:
                num=type->data;

                switch(type2->type)
                {
                case INTEGER: case REAL:
                    num2=type2->data;
                    num->data=interpretator_stack_head;
                    interpretator_stack_head+=BITS;
                    *(int*)num->data=*(int*)num2->data;
                }

                break;
        }

        i=i->next;
        j=j->next;
    }

    //tree_print(call_data->fun->types, interpretator_init_local_vars);

    if(call_data->fun->body)
    {
        if(interpretator_next_op->next)
            push(interpretator_return_place, interpretator_next_op->next);
        interpretator_next_op=call_data->fun->body->begin;
    }
    else
        interpretator_next_op=interpretator_next_op->next;

    if(debug)
    {
        printf("<call "); str_print(call_data->fun->name), printf("()>\n");
    }
}

int print_ptrs_level=0;

void interpretator_print(Type *putc_data)
{
    Number      *var;
    Const       *constant;
    Element     *element;
    ConstString *const_string;
    Pointers    *ptrs,
                *ptrs_tmp;
    Type         rekurse_putc_data;

    char        *data=putc_data->data;
    char         type=putc_data->type;
    int          j,
                 k;

    print_ptrs_level++;

    switch(type)
    {
    case REAL:
        var=data;
        printf("%f", *(float*)var->data);
        break;

    case INTEGER:
        var=data;
        printf("%d", *(int*)var->data);
        break;

    case CONST:
        constant=data;
        printf("%d", constant->data);
        break;

    case CONST_STRING:
        const_string=data;
        for(j=0; j<const_string->isz; j++)
        {
            if(const_string->data[j]=='\\' && j!=const_string->isz-1)
            {
                switch(const_string->data[j+1])
                {
                case '\\':
                    printf("\\");
                    j++;
                    break;
                case 'n':
                    printf("\n");
                    j++;
                    break;
                case 't':
                    printf("\t");
                    j++;
                    break;
                case 'b':
                    printf("\b");
                    j++;
                    break;
                }
            }
            else
                printf("%c", const_string->data[j]);
        }
        break;

    case ELEMENT:
        element=data;
        if(element->uninitialized)
        {
            printf("undefined\n");
        }
        else
        {
            for(j=0; j<element->isz; j++)
            {
                if(element->data[j]=='\\' && j!=element->isz-1)
                {
                    switch(element->data[j+1])
                    {
                    case '\\':
                        printf("\\");
                        j++;
                        break;
                    case 'n':
                        printf("\n");
                        j++;
                        break;
                    case 't':
                        printf("\t");
                        j++;
                        break;
                    case 'b':
                        printf("\b");
                        j++;
                        break;
                    }
                }
                else
                    printf("%c", element->data[j]);
            }
        }
        break;

    case PTRS:
        ptrs=data;
        if(ptrs->uninitialized)
        {
            printf("undefined\n");
        }
        else
        {
            printf("{\n");
            for(k=0; k<print_ptrs_level-1; k++)
                printf("   ");

            for(j=0; j<ptrs->ilength-1; j++)
            {
                for(k=0; k<print_ptrs_level; k++)
                    printf("   ");

                switch(ptrs->data[j].type)
                {
                case CONST:
                    printf("const");
                    break;

                case CONST_STRING:
                    printf("string");
                    break;

                case INTEGER:
                    var=ptrs->data[j].data;
                    str_print(var->name); printf(": %d", var->data);
                    break;

                case UNDEFINED:
                    printf("undefined");
                    break;

                case PTRS:
                    rekurse_putc_data.data=ptrs->data[j].data;
                    rekurse_putc_data.type=PTRS;

                    ptrs_tmp=rekurse_putc_data.data;
                    printf("%s:", ptrs_tmp->name);
                    interpretator_print(&rekurse_putc_data);
                    break;
                }
                printf(",\n");
            }
            if(ptrs->ilength)
            {
                for(k=0; k<print_ptrs_level; k++)
                    printf("   ");
                switch(ptrs->data[ptrs->ilength-1].type)
                {
                case CONST:
                    printf("const");
                    break;
                case CONST_STRING:
                    printf("string");
                    break;

                case INTEGER:
                    var=ptrs->data[ptrs->ilength-1].data;
                    str_print(var->name); printf(": %d", var->data);
                    break;

                case UNDEFINED:
                    printf("undefined");
                    break;

                case PTRS:
                    rekurse_putc_data.data=ptrs->data[j].data;
                    rekurse_putc_data.type=PTRS;

                    ptrs_tmp=rekurse_putc_data.data;
                    printf("%s:", ptrs_tmp->name);
                    interpretator_print(&rekurse_putc_data);
                    break;
                }
            }
            printf("\n");
            for(k=0; k<print_ptrs_level-1; k++)
                printf("   ");
            printf("}");
        }
        break;
    }

    print_ptrs_level--;
}

void interpretator_putc()
{
    interpretator_print(((Print*)data->data)->data);
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_var_var()
{
    var_var_data=(VarVar*)data->data;
/*
    if(var_var_data->var->is_closure)
    {
        if(var_var_data->in->is_closure)
            *(int*)var_var_data->var->data=var_var_data->in->data;
        else
            *(int*)var_var_data->var->data=*(int*)var_var_data->in->data;
    }
    else
        var_var_data->var->data=var_var_data->in->data;*/
    *(int*)var_var_data->var->data=*(int*)var_var_data->in->data;

    if(debug)
    {
        printf("<"); str_print(var_var_data->var->name); printf("=");
        str_print(var_var_data->in->name); printf(" %d>\n", *(int*)var_var_data->var->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_var_const()
{
    var_const_data=(VarConst*)data->data;

    *(int*)var_const_data->var->data=var_const_data->in->data;

    if(debug)
    {
        printf("<"); str_print(var_const_data->var->name); printf("=");
        str_print(var_const_data->in->name); printf(" %d>\n", *(int*)var_const_data->var->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_var_el()
{
    int j;

    var_el_data=(VarElement*)data->data;

    if((int)var_el_data->in->isz-(int)var_el_data->index->data < BITS)
    {
        for(j=0; j<var_el_data->in->isz; j++)
            ((char*)(&var_el_data->var->data))[j]=var_el_data->in->data[j+(int)var_el_data->index->data];
        for(; j<BITS; j++)
            ((char*)(&var_el_data->var->data))[j]=var_el_data->in->data[j+(int)var_el_data->index->data];
    }
    else
    {
        for(j=0; j<BITS; j++)
            ((char*)(&var_el_data->var->data))[j]=var_el_data->in->data[j+(int)var_el_data->index->data];
    }
    if(debug)
    {
        printf("<%s=%s %d>\n", var_el_data->var->name, var_el_data->in->name, var_el_data->var->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_var_ptrs()
{
    var_ptr_data=(VarPtr*)data->data;
    if(var_ptr_data->ptrs->data->type!=INTEGER)
    {
        printf("this not variable");
        return 0;
    }
    var_ptr_data->var->data=((Number*)var_ptr_data->ptrs->data->data)->data;
    if(debug)
    {
        printf("<%s=%s[%d] %d>\n", var_ptr_data->var->name, var_ptr_data->ptrs->name, var_ptr_data->index->data, var_ptr_data->var->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_var_const_string()
{
    int j;

    var_const_string_data=(VarConstString*)data->data;
    if(var_const_string_data->in->isz<BITS)
    {
        var_const_string_data->var->data=0;
        for(j=0; j<var_const_string_data->in->isz; j++)
            ((char*)&var_const_string_data->var->data)[j]=var_const_string_data->in->data[j];
    }
    else
        var_const_string_data->var->data=*(int*)var_const_string_data->in->data;
    if(debug)
    {
        printf("<%s=%s %d>\n", var_const_string_data->var->name, var_const_string_data->in->name, var_const_string_data->var->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_el_var()
{
    int j;

    el_var_data=(ElementVar*)data->data;
    if(el_var_data->index->data>=el_var_data->el->isz)
        printf("warning:out of range, Element:%s(length:%d), index:%d\n", el_var_data->el->name, el_var_data->el->isz, el_var_data->index->data);

    if(el_var_data->el->uninitialized)
    {
        printf("error:uninitialized Element %s\n", el_var_data->el->name);
    }
    else
    {
        if((int)el_var_data->el->isz-(int)el_var_data->index->data >= BITS)
        {
            *(int*)(el_var_data->el->data+(int)el_var_data->index->data)=el_var_data->in->data;
        }
        else
        {
            for(j=0; j<el_var_data->el->isz-(int)el_var_data->index->data; j++)
                el_var_data->el->data[j+(int)el_var_data->index->data]=((char*)&el_var_data->in->data)[j];
        }
    }

    if(debug)
    {
        printf("<element=var %d index:%d>\n", *(int*)(el_var_data->el->data+(int)el_var_data->index->data), el_var_data->index->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_el_const()
{
    int j;

    el_const_data=(ElementConst*)data->data;

    if(el_const_data->el->isz-(int)el_const_data->index->data >= BITS)
    {
        for(j=0; j<BITS; j++)
            el_const_data->el->data[j+(int)el_const_data->index->data]=((char*)&el_const_data->in->data)[j];
    }
    else
    {
        for(j=0; j<el_const_data->el->isz-(int)el_const_data->index->data; j++)
            el_const_data->el->data[j+(int)el_const_data->index->data]=((char*)&el_const_data->in->data)[j];
    }
    if(debug)
    {
        printf("<element=const %d>\n", *(int*)(el_const_data->el->data+(int)el_const_data->index->data));
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_el_el()
{
    int j;

    el_el_data=(ElementElement*)data->data;

    free(el_el_data->el->data);
    el_el_data->el->data=malloc(el_el_data->in->isz);
    el_el_data->el->isz=el_el_data->in->isz;

    for(j=0; j<el_el_data->el->isz; j++)
        el_el_data->el->data[j]=el_el_data->in->data[j];
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_el_ptrs()
{
    el_ptr_data=(ElementPtr*)data->data;

    if(el_ptr_data->index->data>=el_ptr_data->ptrs->ilength || el_ptr_data->ptrs->data[(int)el_ptr_data->index->data].type!=ELEMENT)
        return 0;

    free(el_ptr_data->el->data);
    el_ptr_data->el=(Element*)el_ptr_data->ptrs->data[(int)el_ptr_data->index->data].data;

    if(debug)
    {
        printf("<element=ptr>\n");
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_el_const_string()
{
    int j;

    el_const_string_data=(ElementConstString*)data->data;

    if(el_const_string_data->el->uninitialized==0)
        free(el_const_string_data->el->data);

    el_const_string_data->el->data=malloc(el_const_string_data->in->isz);
    el_const_string_data->el->isz=el_const_string_data->in->isz;
    for(j=0; j<el_const_string_data->in->isz; j++)
        el_const_string_data->el->data[j]=el_const_string_data->in->data[j];

    el_const_string_data->el->uninitialized=0;
    if(debug)
    {
        printf("<element=string>\n");
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_ptrs_var()
{
    ptr_var_data=(PointerVar*)data->data;
    //for(j=0; j<BITS; j++)
    //ptr_var_data->ptrs->data[ptr_var_data->index->data].data[j]=((char*)&ptr_var_data->var)[j];
    ptr_var_data->ptrs->data[(int)ptr_var_data->index->data].type=INTEGER;
    ptr_var_data->ptrs->data[(int)ptr_var_data->index->data].data=ptr_var_data->var;
    if(debug)
    {
        printf("<"); str_print(ptr_var_data->ptrs->name);
        printf("[%d]=", ptr_var_data->index->data); str_print(ptr_var_data->var->name);
        printf(" %d>\n", ptr_var_data->var->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_ptrs_const()
{
    ptr_const_data=(PointerConst*)data->data;

    if(ptr_const_data->index->data>=ptr_const_data->ptrs->ilength)
    {
        printf("vihod\n");
        return 0;
    }

    ptr_const_data->ptrs->data[(int)ptr_const_data->index->data].data=(char*)ptr_const_data->in;
    ptr_const_data->ptrs->data[(int)ptr_const_data->index->data].type=CONST;
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_ptrs_el()
{
    ptr_el_data=(PointerElement*)data->data;

    if(ptr_el_data->index->data>=ptr_el_data->ptrs->ilength)
    {
        printf("vihod\n");
        return 0;
    }

    ptr_el_data->ptrs->data[(int)ptr_el_data->index->data].data=(char*)ptr_el_data->in;
    ptr_el_data->ptrs->data[(int)ptr_el_data->index->data].type=ELEMENT;
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_ptrs_ptrs()
{
    ptr_ptr_data=(PointerPtr*)data->data;

    if(ptr_ptr_data->index->data>=ptr_ptr_data->ptrs->ilength)
    {
        printf("vihod\n");
        return 0;
    }

    ptr_ptr_data->ptrs->data[(int)ptr_ptr_data->index->data].data=(char*)ptr_ptr_data->ptrs_in;
    ptr_ptr_data->ptrs->data[(int)ptr_ptr_data->index->data].type=PTRS;
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_ptrs_const_string()
{

}

void interpretator_inc()
{
    inc_data=(Increment*)data->data;
    inc_data->var->data++;
    if(debug)
    {
        printf("<i++ %d>\n",inc_data->var->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_dec()
{
    dec_data=(Decrement*)data->data;
    dec_data->var->data--;
    if(debug)
    {
        printf("<"); str_print(dec_data->var->name); printf("-- %d>\n",dec_data->var->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_add()
{
    add_data=(Add*)data->data;

    int a, b, *c;

    c=add_data->var_rez->data;
    a=*(int*)add_data->var1->data;
    b=*(int*)add_data->var2->data;
    *c=a+b;

    if(debug)
    {
        printf("<"); str_print(add_data->var_rez->name); printf("=");
        str_print(add_data->var1->name); printf("+");
        str_print(add_data->var2->name); printf("=%d>\n", *c);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_sub()
{
    sub_data=(Sub*)data->data;
    sub_data->var_rez->data=(int)sub_data->var1->data-(int)sub_data->var2->data;
    if(debug)
    {
        printf("<"); str_print(sub_data->var_rez->name); printf("=");
        str_print(sub_data->var1->name); printf("-");
        str_print(sub_data->var2->name); printf("=%d>\n", sub_data->var_rez->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_mul()
{
    mul_data=(Mul*)data->data;
    mul_data->var_rez->data=(int)mul_data->var1->data*(int)mul_data->var2->data;
    if(debug)
    {
        printf("<"); str_print(mul_data->var_rez->name); printf("=");
        str_print(mul_data->var1->name); printf("*");
        str_print(mul_data->var2->name); printf("=%d>\n", mul_data->var_rez->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_div()
{
    div_data=(Div*)data->data;
    div_data->var_rez->data=(int)div_data->var1->data/(int)div_data->var2->data;
    if(debug)
    {
        printf("<"); str_print(div_data->var_rez->name); printf("=");
        str_print(div_data->var1->name); printf("/");
        str_print(div_data->var2->name); printf("=%d>\n", div_data->var_rez->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_fadd()
{
    add_data=(Add*)data->data;

    float a, b, *c;

    c=add_data->var_rez->data;
    a=*(float*)add_data->var1->data;
    b=*(float*)add_data->var2->data;
    *c=a+b;

    if(debug)
    {
        printf("<"); str_print(add_data->var_rez->name); printf("=");
        str_print(add_data->var1->name); printf("+");
        str_print(add_data->var2->name); printf("=%f>\n", *c);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_fsub()
{

}

void interpretator_fmul()
{

}

void interpretator_fdiv()
{

}

void interpretator_shr()
{
    Shr *shr_data=(Shr*)data->data;

    shr_data->var_rez->data=(int)shr_data->var->data >> (int)shr_data->shift->data;
    if(debug)
    {
        printf("<"); str_print(shr_data->var_rez->name); printf("=");
        str_print(shr_data->var->name); printf(">>");
        str_print(shr_data->shift->name); printf("=%d>\n", shr_data->var_rez->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_shl()
{
    Shl *shl_data=(Shl*)data->data;

    shl_data->var_rez->data=(int)shl_data->var->data << (int)shl_data->shift->data;

    if(debug)
    {
        printf("<"); str_print(shl_data->var_rez->name); printf("=");
        str_print(shl_data->var->name); printf("<<");
        str_print(shl_data->shift->name); printf("=%d>\n", shl_data->var_rez->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_xor()
{
    Xor *xor_data=(Xor*)data->data;

    xor_data->var_rez->data=(int)xor_data->var1->data ^ (int)xor_data->var2->data;

    if(debug)
    {
        printf("<"); str_print(xor_data->var_rez->name); printf("=");
        str_print(xor_data->var1->name); printf("^");
        str_print(xor_data->var2->name); printf("=%d>\n", xor_data->var_rez->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_and()
{
    And *and_data=(And*)data->data;

    and_data->var_rez->data=(int)and_data->var1->data & (int)and_data->var2->data;

    if(debug)
    {
        printf("<"); str_print(and_data->var_rez->name); printf("=");
        str_print(and_data->var1->name); printf("&");
        str_print(and_data->var2->name); printf("=%d>\n", and_data->var_rez->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_or()
{
    Or *or_data=(Or*)data->data;

    or_data->var_rez->data=(int)or_data->var1->data & (int)or_data->var2->data;

    if(debug)
    {
        printf("<"); str_print(or_data->var_rez->name); printf("=");
        str_print(or_data->var1->name); printf("|");
        str_print(or_data->var2->name); printf("=%d>\n", or_data->var_rez->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_not()
{
    Not *not_data=(Not*)data->data;

    not_data->var_rez->data=!(int)not_data->var->data;

    if(debug)
    {
        printf("<"); str_print(not_data->var_rez->name); printf("=!");
        str_print(not_data->var->name); printf("=%d>\n", not_data->var_rez->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_element_alloc()
{
    el_alloc_data=(ElementAlloc*)data->data;

    free(el_alloc_data->el->data);
    el_alloc_data->el->isz=el_alloc_data->sz->data;
    el_alloc_data->el->data=malloc(el_alloc_data->el->isz);
    el_alloc_data->el->uninitialized=0;

    if(debug)
    {
        printf("<alloc %s size %d>\n", el_alloc_data->el->name, el_alloc_data->sz->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_pointers_alloc()
{
    int j;

    ptrs_alloc=(PointersAlloc*)data->data;

    ptrs_alloc->ptrs->ilength=ptrs_alloc->length->data;

    ptrs_alloc->ptrs->data=malloc(sizeof(Data) * ptrs_alloc->ptrs->ilength);
    for(j=0; j<ptrs_alloc->ptrs->ilength; j++)
    {
        ptrs_alloc->ptrs->data[j].data=malloc(BITS);
        ptrs_alloc->ptrs->data[j].type=UNDEFINED;
    }

    ptrs_alloc->ptrs->uninitialized=0;

    if(debug)
    {
        printf("<alloc ptrs length %d>\n", ptrs_alloc->ptrs->ilength);
    }

    interpretator_next_op=interpretator_next_op->next;
}
