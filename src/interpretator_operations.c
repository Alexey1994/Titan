#include "interpretator_operations.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

void interpretator_init_local_vars(Type *type);

extern char *interpretator_stack;
extern char *interpretator_stack_head;

extern RunData *data;
extern Tree *interpretator_tree;

extern char loop_not_break;
extern struct ListNode *interpretator_next_op;
extern NodeStack *cur_loop;

extern Stack *interpretator_return_place;

extern char debug;

Loop *loop_data;
Increment *inc_data;
Decrement *dec_data;
Call *call_data;
If *if_data;

Add *add_data;
Sub *sub_data;
Mul *mul_data;
Div *div_data;

ElementVar *el_var_data;
ElementArray *el_arr_data;
ElementConst *el_const_data;
ElementConstString *el_const_string_data;
ElementPtr *el_ptr_data;
ElementElement *el_el_data;

VarElement *var_el_data;
VarVar *var_var_data;
VarArray *var_arr_data;
VarConst *var_const_data;
VarConstString *var_const_string_data;
VarPtr *var_ptr_data;

ArrayElement *arr_el_data;
ArrayVar *arr_var_data;
ArrayArray *arr_arr_data;
ArrayConst *arr_const_data;
ArrayConstString *arr_const_string_data;
ArrayPtr *arr_ptr_data;

PointerElement *ptr_el_data;
PointerVar *ptr_var_data;
PointerArray *ptr_arr_data;
PointerConst *ptr_const_data;
PointerConstString *ptr_const_string_data;
PointerPtr *ptr_ptr_data;

ElementAlloc *el_alloc_data;
ArrayAlloc *arr_alloc_data;
PointersAlloc *ptrs_alloc;

Equal *eq_data;
NotEqual *neq_data;
GreatherThan *gt_data;
LesserThan *lt_data;
GreatherThanOrEqual *ge_data;
LesserThanOrEqual *le_data;

Array *arr_tmp;
Const *const_tmp;

Type *putc_data;

Number *var;
Const *constant;
Element *element;
ConstString *const_string;
Pointers *ptrs;

void interpretator_loop()
{
    loop_data=(Loop*)data->data;
    if(debug)
        printf("<loop>\n");

    if(loop_data->eval->length)
    {
        if(loop_data->eval->end->next==0)
            loop_data->eval->end->next=loop_data->eval->begin;
        interpretator_next_op=loop_data->eval->begin;
    }
}

void interpretator_if()
{
    if_data=(If*)data->data;

    if(debug)
    {
        printf("<if "); str_print(if_data->cond->name); printf("(%d)>\n", if_data->cond->data);
    }

    if(if_data->cond->data && if_data->eval->length)
        interpretator_next_op=if_data->eval->begin;
    else
        interpretator_next_op=interpretator_next_op->next;
}

void interpretator_break()
{
    Break *break_data=data->data;
    interpretator_next_op=break_data->loop;
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_continue()
{
    Continue *continue_data=data->data;
    interpretator_next_op=continue_data->loop;
}

void interpretator_call()
{
    call_data=(Call*)data->data;

    if(debug)
    {
        printf("<call "); str_print(call_data->fun->name), printf("()>\n");
    }

    tree_print(call_data->fun->types, interpretator_init_local_vars);

    if(call_data->fun->body)
    {
        if(interpretator_next_op->next)
            push(interpretator_return_place, interpretator_next_op->next);
        interpretator_next_op=call_data->fun->body->begin;
    }
    else
        interpretator_next_op=interpretator_next_op->next;
}

int print_ptrs_level=0;

void interpretator_print(Type *putc_data)
{
    Number *var;
    Const *constant;
    Element *element;
    ConstString *const_string;
    Pointers *ptrs, *ptrs_tmp;
    Array *arr;
    Type rekurse_putc_data;

    char *data=putc_data->data;
    char type=putc_data->type;
    int j,k;

    print_ptrs_level++;

    switch(type)
    {
    case INTEGER:
        var=data;
        printf("%d", var->data);
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

    case ARRAY:
        arr=data;
        if(arr->uninitialized)
        {
            printf("undefined\n");
        }
        else
        {
            printf("[");
            for(j=0; j<arr->ilength-1; j++)
            {
                for(k=0; k<arr->isz; k++)
                    printf("%c",arr->data[j*arr->isz+k]);
                printf(", ");
            }
            for(k=0; k<arr->isz; k++)
                printf("%c",arr->data[(arr->ilength-1)*arr->isz+k]);
            printf("]");
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
    interpretator_print(((Putc*)data->data)->data);
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

    if(var_const_data->var->is_closure)
    {
        *(int*)var_const_data->var->data=var_const_data->in->data;
        if(debug)
        {
            printf("<"); str_print(var_const_data->var->name); printf("=");
            str_print(var_const_data->in->name); printf(" %d>\n", *(int*)var_const_data->var->data);
        }
    }
    else
    {
        var_const_data->var->data=var_const_data->in->data;
        if(debug)
        {
            printf("<"); str_print(var_const_data->var->name); printf("=");
            str_print(var_const_data->in->name); printf(" %d>\n", var_const_data->var->data);
        }
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

void interpretator_var_array()
{
    int j;
    var_arr_data=(VarArray*)data->data;

    if(var_arr_data->index->data>=var_arr_data->arr->ilength)
    {
        printf("\nvihod za granici massiva\n");
        return 0;
    }

    if(var_arr_data->arr->isz>=BITS)
        for(j=0; j<BITS; j++)
            ((char*)&var_arr_data->var->data)[j]=(var_arr_data->arr->data+(int)var_arr_data->arr->isz*(int)var_arr_data->index->data)[j];
    else
    {
        for(j=0; j<(int)var_arr_data->arr->isz; j++)
            ((char*)&var_arr_data->var->data)[j]=*(var_arr_data->arr->data+(int)var_arr_data->arr->isz*(int)var_arr_data->index->data+j);
        for(; j<BITS; j++)
            ((char*)&var_arr_data->var->data)[j]=0;
    }
    if(debug)
    {
        printf("<%s=%s[%d] %d>\n", var_arr_data->var->name, var_arr_data->arr->name, var_arr_data->index->data, var_arr_data->var->data);
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

void interpretator_el_array()
{
    int j;

    el_arr_data=(ElementArray*)data->data;

    free(el_arr_data->el->data);
    el_arr_data->el->isz=el_arr_data->arr->isz;
    el_arr_data->el->data=(char*)malloc(el_arr_data->arr->isz);

    for(j=0; j<el_arr_data->el->isz; j++)
        el_arr_data->el->data[j]=el_arr_data->arr->data[el_arr_data->arr->isz*(int)el_arr_data->index->data+j];
    if(debug)
    {
        printf("<element=array>\n");
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

void interpretator_ptrs_array()
{
    ptr_arr_data=(PointerArray*)data->data;

    if(ptr_arr_data->index->data>=ptr_arr_data->ptrs->ilength)
    {
        printf("vihod\n");
        return 0;
    }

    ptr_arr_data->ptrs->data[(int)ptr_arr_data->index->data].data=(char*)ptr_arr_data->arr;
    ptr_arr_data->ptrs->data[(int)ptr_arr_data->index->data].type=ARRAY;
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_ptrs_const_string()
{

}

void interpretator_array_var()
{
    int j;

    arr_var_data=(ArrayVar*)data->data;

    if(arr_var_data->arr->ilength<=arr_var_data->index->data)
    {
        printf("vihod\n");
        return 0;
    }

    if(arr_var_data->arr->isz>BITS)
    {
        for(j=0; j<BITS; j++)
            arr_var_data->arr->data[(int)arr_var_data->index->data * arr_var_data->arr->isz + j]=((char*)&arr_var_data->var->data)[j];
        for(; j<arr_var_data->arr->isz; j++)
            arr_var_data->arr->data[(int)arr_var_data->index->data * arr_var_data->arr->isz + j]=0;
    }
    else
    {
        for(j=0; j<arr_var_data->arr->isz; j++)
            arr_var_data->arr->data[(int)arr_var_data->index->data * arr_var_data->arr->isz + j]=((char*)&arr_var_data->var->data)[j];
    }
    if(debug)
    {
        printf("<%s[%d]=%s %d>\n", arr_var_data->arr->name, arr_var_data->index->data, arr_var_data->var->name, arr_var_data->var->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_array_const()
{
    int j;

    arr_const_data=(ArrayConst*)data->data;

    if(arr_const_data->index->data>=arr_const_data->arr->ilength)
    {
        printf("vihod\n");
        return 0;
    }

    if(arr_const_data->arr->isz>=BITS)
    {
        for(j=0; j<BITS; j++)
            arr_const_data->arr->data[(int)arr_const_data->index->data*arr_const_data->arr->isz+j]=(
                (char*)&arr_const_data->in->data)[j];
        for(; j<arr_const_data->arr->isz; j++)
            arr_const_data->arr->data[(int)arr_const_data->index->data*arr_const_data->arr->isz+j]=0;
    }
    else
    {
        for(j=0; j<arr_const_data->arr->isz; j++)
            arr_const_data->arr->data[(int)arr_const_data->index->data*arr_const_data->arr->isz+j]=((char*)&arr_const_data->in->data)[j];
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_array_el()
{
    int j;

    arr_el_data=(ArrayElement*)data->data;

    if(arr_el_data->arr->isz>=arr_el_data->in->isz)
    {
        for(j=0; j<arr_el_data->in->isz; j++)
            arr_el_data->arr->data[(int)arr_el_data->index->data*arr_el_data->arr->isz+j]=arr_el_data->in->data[j];
    }
    else
    {
        for(j=0; j<arr_el_data->arr->isz; j++)
            arr_el_data->arr->data[(int)arr_el_data->index->data*arr_el_data->arr->isz+j]=arr_el_data->in->data[j];
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_array_ptrs()
{
    arr_ptr_data=(ArrayPtr*)data->data;

    if(arr_ptr_data->ptrs->data[(int)arr_ptr_data->index->data].type!=ARRAY)
    {
        printf("not Array\n");
        return 0;
    }

    arr_tmp=(Array*)arr_ptr_data->ptrs->data[(int)arr_ptr_data->index->data].data;
    arr_ptr_data->arr->data=arr_tmp->data;
    arr_ptr_data->arr->ilength=arr_tmp->ilength;
    arr_ptr_data->arr->isz=arr_tmp->isz;

    if(debug)
    {
        printf("<%s=%s[%d]>\n", arr_ptr_data->arr->name, arr_ptr_data->ptrs->name, arr_ptr_data->index->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_array_array()
{
    int j;

    arr_arr_data=(ArrayArray*)data->data;

    if(arr_arr_data->index->data>=arr_arr_data->arr->ilength || arr_arr_data->index_in->data>=arr_arr_data->arr_in->ilength)
    {
        printf("vihod\n");
        return 0;
    }

    if(arr_arr_data->arr->isz>=arr_arr_data->arr_in->isz)
    {
        for(j=0; j<arr_arr_data->arr_in->isz; j++)
            arr_arr_data->arr->data[arr_arr_data->arr->isz*(int)arr_arr_data->index->data+j]=arr_arr_data->arr_in->data[arr_arr_data->arr_in->isz*(int)arr_arr_data->index_in->data+j];
    }
    else
    {
        for(j=0; j<arr_arr_data->arr->isz; j++)
            arr_arr_data->arr->data[arr_arr_data->arr->isz*(int)arr_arr_data->index->data+j]=arr_arr_data->arr_in->data[arr_arr_data->arr_in->isz*(int)arr_arr_data->index_in->data+j];
    }

    if(debug)
    {
        printf("<%s[%d]=%s[%d]>\n", arr_arr_data->arr->name, arr_arr_data->index->data, arr_arr_data->arr_in->name, arr_arr_data->index_in->data);
    }
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_array_const_string()
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
/*
    if(add_data->var_rez->is_closure)
        c=add_data->var_rez->data;
    else
        c=&add_data->var_rez->data;

    if(add_data->var1->is_closure)
        a=*(int*)add_data->var1->data;
    else
        a=add_data->var1->data;

    if(add_data->var2->is_closure)
        b=*(int*)add_data->var2->data;
    else
        b=add_data->var2->data;
*/
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

void interpretator_eq()
{
    eq_data=(Equal*)data->data;

    //((Equal*)data->data)->var->data=((Equal*)data->data)->left->data==((Equal*)data->data)->right->data;

    eq_data->var->data=(int)eq_data->left->data==(int)eq_data->right->data;
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_neq()
{
    neq_data=(NotEqual*)data->data;

    neq_data->var->data=(int)neq_data->left->data!=(int)neq_data->right->data;

    if(debug)
    {
        printf("<a!=b %d>\n", neq_data->var->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_gt()
{
    gt_data=(GreatherThan*)data->data;

    gt_data->var->data=(int)gt_data->left->data > (int)gt_data->right->data;

    if(debug)
    {
        printf("<a>b %d>\n", gt_data->var->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_lt()
{
    lt_data=(LesserThan*)data->data;

    lt_data->var->data=(int)lt_data->left->data < (int)lt_data->right->data;

    if(debug)
    {
        printf("<a<b %d>\n", lt_data->var->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_ge()
{
    ge_data=(GreatherThanOrEqual*)data->data;

    ge_data->var->data=(int)ge_data->left->data >= (int)ge_data->right->data;

    if(debug)
    {
        printf("<a>=b %d>\n", ge_data->var->data);
    }

    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_le()
{
    le_data=(LesserThanOrEqual*)data->data;

    le_data->var->data=(int)le_data->left->data <= (int)le_data->right->data;

    if(debug)
    {
        printf("<a<=b %d>\n", le_data->var->data);
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

void interpretator_array_alloc()
{
    int j;

    arr_alloc_data=(ArrayAlloc*)data->data;

    arr_alloc_data->arr->isz=arr_alloc_data->sz->data;
    arr_alloc_data->arr->ilength=arr_alloc_data->length->data;

    if(debug)
    {
        printf("<alloc length %d size %d>\n", arr_alloc_data->length->data, arr_alloc_data->sz->data);
    }

    arr_alloc_data->arr->data=malloc((int)arr_alloc_data->sz->data * (int)arr_alloc_data->length->data);
    for(j=0; j<(int)arr_alloc_data->sz->data * (int)arr_alloc_data->length->data; j++)
        arr_alloc_data->arr->data[j]=1;
    interpretator_next_op=interpretator_next_op->next;
}

void interpretator_pointers_alloc()
{
    int j;

    ptrs_alloc=(PointersAlloc*)data->data;

    ptrs_alloc->ptrs->ilength=ptrs_alloc->length->data;

    ptrs_alloc->ptrs->data=malloc(sizeof(PointerData) * ptrs_alloc->ptrs->ilength);
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
