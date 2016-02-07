#include "parser_if.h"

extern char OK    ,
            ERROR ,
            disasm;

extern String   *parser_string_code;
extern Function *cur_function      ;

static Type *op1,
            *op2;

static char get_2_op()
{
    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
        return ERROR;

    op2=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op2==0)
        return ERROR;

    return OK;
}

static void free_cond_data(RunData *cond_data)
{
    if(cond_data->data)
        free(cond_data->data);
    free(cond_data);
}

static char error(List *if_cond)
{
    printf("cond error\n");
    return ERROR;
}

static char get_IZ(List *if_cond)
{
    IfZero *alloc;

    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
        return ERROR;

    alloc=malloc(sizeof(IfZero));
    alloc->var=op1;

    if(disasm)
    {
        printf(" IZ ");
        str_print(op1->name);
    }

    list_push(if_cond, new_run_data(IZ, alloc));
    return OK;
}

static char get_INZ(List *if_cond)
{
    IfNotZero *alloc;

    op1=(Type*)get_parser_op_all(parser_string_code, cur_function);
    if(op1==0)
        return ERROR;

    alloc=malloc(sizeof(IfNotZero));
    alloc->var=op1;

    list_push(if_cond, new_run_data(INZ, alloc));
    return OK;
}

static char get_IE(List *if_cond)
{
    IfEqual *alloc;

    if(get_2_op()==ERROR)
        return ERROR;

    alloc=malloc(sizeof(IfEqual));
    alloc->var_l=op1;
    alloc->var_r=op2;

    list_push(if_cond, new_run_data(IE, alloc));
    return OK;
}

static char get_INE(List *if_cond)
{
    IfNotEqual *alloc;

    if(get_2_op()==ERROR)
        return ERROR;

    alloc=malloc(sizeof(IfNotEqual));
    alloc->var_l=op1;
    alloc->var_r=op2;

    list_push(if_cond, new_run_data(INE, alloc));
    return OK;
}

static char get_ILT(List *if_cond)
{
    IfLesserTnan *alloc;

    if(get_2_op()==ERROR)
        return ERROR;

    alloc=malloc(sizeof(IfLesserTnan));
    alloc->var_l=op1;
    alloc->var_r=op2;

    list_push(if_cond, new_run_data(ILT, alloc));
    return OK;
}

static char get_IGT(List *if_cond)
{
    IfGreatherTnan *alloc;

    if(get_2_op()==ERROR)
        return ERROR;

    alloc=malloc(sizeof(IfGreatherTnan));
    alloc->var_l=op1;
    alloc->var_r=op2;

    list_push(if_cond, new_run_data(IGT, alloc));
    return OK;
}

static char get_ILE(List *if_cond)
{
    IfLesserTnanOrEqual *alloc;

    if(get_2_op()==ERROR)
        return ERROR;

    alloc=malloc(sizeof(IfLesserTnanOrEqual));
    alloc->var_l=op1;
    alloc->var_r=op2;

    list_push(if_cond, new_run_data(ILE, alloc));
    return OK;
}

static char get_IGE(List *if_cond)
{
    IfGreatherTnanOrEqual *alloc;

    if(get_2_op()==ERROR)
        return ERROR;

    alloc=malloc(sizeof(IfGreatherTnanOrEqual));
    alloc->var_l=op1;
    alloc->var_r=op2;

    list_push(if_cond, new_run_data(IGE, alloc));
    return OK;
}

static char get_OR(List *if_cond)
{
    if(disasm)
        printf(" OR");
    list_push(if_cond, new_run_data(OR, 0));
    return OK;
}

static char get_AND(List *if_cond)
{
    if(disasm)
        printf(" AND");
    list_push(if_cond, new_run_data(AND, 0));
    return OK;
}

static char (*cond_table[256])(List *if_cond);

void parser_init_cond_table()
{
    int i;
    for(i=0; i<256; i++)
        cond_table[i]=error;

    cond_table[IZ]=get_IZ;
    cond_table[INZ]=get_INZ;
    cond_table[IE]=get_IE;
    cond_table[INE]=get_INE;
    cond_table[ILT]=get_ILT;
    cond_table[IGT]=get_IGT;
    cond_table[ILE]=get_ILE;
    cond_table[IGE]=get_IGE;

    cond_table[OR]=get_OR;
    cond_table[AND]=get_AND;
}

static char verify_OPN(List *opn)
{
    struct ListNode *i=opn->begin;
    RunData *run_data;
    Stack *s=stack_init();

    while(i)
    {
        run_data=i->data;

        switch(run_data->type)
        {
        case OR: case AND:
            if(!s->begin)
                goto err;
            pop(s);
            if(!s->begin)
                goto err;
            pop(s);
            break;

        default:
            push(s, 0);
            break;
        }
        i=i->next;
    }
    if(s->begin)
        goto err;

    return OK;
err:
    stack_free(s);
    return ERROR;
}

char get_if_conditions(If *if_data)
{
    char cond_type;

    cond_type=str_pop(parser_string_code);
    while(cond_type)
    {
        if(cond_table[cond_type](if_data->cond)==ERROR)
        {
            list_free(if_data->cond, free_cond_data);
            return ERROR;
        }
        cond_type=str_pop(parser_string_code);
    }

    if(verify_OPN(if_data->cond)==ERROR)
    {
        printf("OPN in cond not good\n");
        list_free(if_data->cond, free_cond_data);
        return ERROR;
    }

    return OK;
}
