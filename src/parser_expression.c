#include "parser_expression.h"

extern const char ERROR,
                  OK;

static void parser_free_expression(Data *data)
{
    if(data->data)
        free(data->data);
    free(data);
}

static void parser_print_args(List *args)
{
    struct ListNode *i;

    i=args->begin;

    while(i)
    {
        list_print(i->data, parser_print_expression);
        if(i->next)
            printf(",");
        i=i->next;
    }
}

void parser_print_expression(Data *data)
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
            printf("(");
            parser_print_args(call->args);
            printf(") ");
            break;
    }
}

static void add_expr_op(List *expressions, char type, String *in)
{
    list_push(expressions, new_data(type, 0));
    str_pop(in);
}

static void add_expr_var(List *expressions, Type *op)
{
    list_push(expressions, new_data(op->type, op->data));
}

static void add_expr_call(List *expressions, Call *call_data)
{
    list_push(expressions, new_data(CALL, call_data));
}

static try verify_OPN(List *expressions)
{
    struct ListNode *i;
    Data            *data;
    int              count;

    i=expressions->begin;
    count=0;

    while(i)
    {
        data=i->data;
        switch(data->type)
        {
        case ADD: case SUB: case MUL: case DIV:
            if(!count)
                return ERROR;
            count--;
            if(!count)
                return ERROR;
            break;

        case INTEGER: case CALL:
            count++;
            break;
        }
        i=i->next;
    }

    if(count>1)
        return ERROR;

    return OK;
}

static void get_args(List *args, int args_length, List **cur_arg, Stack *function_positions)
{
    int k;

    for(k=0; k<args_length; k++)
    {
        push(function_positions, *cur_arg);
        *cur_arg=list_init();
        list_push_front(args, *cur_arg);
    }
}

static void add_call(Function *function, List **cur_expression, Stack *function_positions)
{
    Call *call_data;

    call_data=malloc(sizeof(Call));

    call_data->fun=function;
    call_data->args=list_init();

    add_expr_call(*cur_expression, call_data);
    get_args(call_data->args, call_data->fun->args->length, cur_expression, function_positions);
}

static try add_variable(String *in, Function *cur_function, List **cur_expression, Stack *function_positions)
{
    Type *op;

    op=(Type*)get_parser_op_all(in, cur_function);
    if(!op)
        return ERROR;

    switch(op->type)
    {
    case FUNCTION:
        add_call(op->data, cur_expression, function_positions);
        break;

    case INTEGER:
        add_expr_var(*cur_expression, op);
        break;
    }

    return OK;
}

static try end_expr(List **cur_expression, Stack *function_positions, char *break_OPN, String *in)
{
    str_pop(in);
    if(verify_OPN(*cur_expression)==ERROR)
        return ERROR;
    *cur_expression=pop(function_positions);

    if(!function_positions->begin)
        *break_OPN=1;

    return OK;
}

List *get_parser_expression(String *in, Function *cur_function)
{
    Stack    *function_positions;
    List     *ret;
    char      is_break;

    ret=list_init();
    function_positions=stack_init();
    push(function_positions, ret);
    is_break=0;

    while(!is_break)
    {
        switch(in->end[-1])
        {
            case ADD: case SUB: case MUL: case DIV:
                add_expr_op(ret, in->end[-1], in);
                break;

            case '\0':
                if(end_expr(&ret, function_positions, &is_break, in)==ERROR)
                    goto err;
                break;

            default:
                if(add_variable(in, cur_function, &ret, function_positions)==ERROR)
                    goto err;
                break;
        }
    }

    return ret;
err:
    list_free(ret, parser_free_expression);
    return 0;
}
