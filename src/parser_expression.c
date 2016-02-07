#include "parser_expression.h"

extern const char ERROR,
                  OK;

static void parser_free_expression(RunData *data)
{
    if(data->data)
        free(data->data);
    free(data);
}

static void parser_print_expression(RunData *data)
{
    Number *num;
    Call   *call;

    switch(data->type)
    {
        case ADD: printf("+ "); break;
        case SUB: printf("- "); break;
        case MUL: printf("* "); break;
        case DIV: printf("/ "); break;

        case INTEGER:
            num=data->data;
            str_print(num->name);
            printf(" ");
            break;

        case CALL:
            call=data->data;
            str_print(call->fun->name);
            printf("() ");
            break;
    }
}

static void add_expr_op(List *expressions, char type, String *in)
{
    list_push(expressions, new_run_data(type, 0));
    str_pop(in);
}

static void add_expr_var(List *expressions, Type *op)
{
    list_push(expressions, new_run_data(op->type, op->data));
}

static void add_expr_call(List *expressions, Call *call_data)
{
    list_push(expressions, new_run_data(CALL, call_data));
}

static try verify_OPN(List *expressions)
{
    Stack           *s;
    struct ListNode *i;
    RunData         *data;

    s=stack_init();
    i=expressions->begin;

    while(i)
    {
        data=i->data;
        switch(data->type)
        {
        case ADD: case SUB: case MUL: case DIV:
            if(!s->begin)
                goto err;
            pop(s);
            if(!s->begin)
                goto err;
            pop(s);
            break;

        case INTEGER: case CALL:
            push(s, 0);
            break;
        }
        i=i->next;
    }
    if(s->begin)
        goto err;

    stack_free(s);
    return OK;
err:
    printf("ERROR");
    stack_free(s);
    return ERROR;
}

static Call* get_function_call(Function *f, String *in, Function *cur_function)
{
    Call *ret;
    List *arg;
    int   i;

    ret=malloc(sizeof(Call));
    ret->fun=f;

    for(i=0; i<f->args->length; i++)
    {
        arg=get_parser_expression(in, cur_function);
        if(arg==0)
            goto err;
        list_push(ret->args, arg);
    }

    return ret;
err:
    free(ret);
    return 0;
}

List *get_parser_expression(String *in, Function *cur_function)
{
    List     *ret,
             *args;
    String   *new_string;
    Type     *op;
    Function *function;
    Call     *call_data;

    ret=list_init();

    while(in->length && in->end[-1])
    {
        switch(in->end[-1])
        {
            case ADD: case SUB: case MUL: case DIV:
                add_expr_op(ret, ADD, in);
                break;

            default:
                op=(Type*)get_parser_op_all(in, cur_function);
                if(!op)
                    goto err;

                switch(op->type)
                {
                case FUNCTION:
                    call_data=get_function_call(op->data, in, cur_function);
                    if(!call_data)
                        goto err;
                    add_expr_call(ret, call_data);
                    break;

                case INTEGER:
                    add_expr_var(ret, op);
                    break;
                }

                break;
        }
    }

    list_print(ret, parser_print_expression);
    if(verify_OPN(ret)==ERROR)
        goto err;

    str_pop(in);
    return ret;
err:
    list_free(ret, parser_free_expression);
        return 0;
}
