#include "expression.h"
#include "parser.h"
#include "List.h"
#include "stack.h"
#include "lexer.h"
#include "parser_cmp.h"
#include <string.h>
#include <stdlib.h>

extern String *buf;
extern String *lexer_out_data;
extern Function *lexer_constants;
extern Id *lexer_id;
extern char debug;

void copy_data_token(String *tok);

int prior(char c)
{
    switch(c)
    {
        case '(': return 0;
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
    }
}

void lexer_strcpy(char *s)
{
    int i=0;

    while(s[i])
    {
        str_push_back(lexer_out_data, s[i]);
        i++;
    }
    str_push_back(lexer_out_data, '\0');
}

void lexer_memcpy(void *data, int sz)
{
    int i;

    for(i=0; i<sz; i++)
        str_push_back(lexer_out_data, ((char*)data)[i]);
}

void lexer_update_constants(String *const_name)
{
    int num;

    if(find_type(lexer_constants->types, const_name)==0)
    {
        num=str_to_int(const_name);

        str_push_back(lexer_out_data, CONST_INIT);
        copy_data_token(const_name);
        lexer_memcpy(&num, BITS);
    }
}

void new_assembler_operation3(char type, Type *in_var, Type *o1, Type *o2)
{
    char *id;
    int num;
    Type *tmp;

    if(o1->type==CONST)
    {
        if(o2->type==CONST)
        {
            //lexer_update_constants(o1->name);
//
            str_push_back(lexer_out_data, ASSIGNMENT);

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'a');
            str_push_back(lexer_out_data, '\0');

            copy_data_token(o1->name);
//
            //lexer_update_constants(o2->name);
//
            str_push_back(lexer_out_data, ASSIGNMENT);

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'd');
            str_push_back(lexer_out_data, '\0');

            copy_data_token(o2->name);
//
            str_push_back(lexer_out_data, type);

            copy_data_token(in_var->name);

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'a');
            str_push_back(lexer_out_data, '\0');

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'd');
            str_push_back(lexer_out_data, '\0');
        }
        else
        {
            //lexer_update_constants(o1->name);
//
            str_push_back(lexer_out_data, ASSIGNMENT);

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'a');
            str_push_back(lexer_out_data, '\0');

            copy_data_token(o1->name);
//
            str_push_back(lexer_out_data, type);

            copy_data_token(in_var->name);

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'a');
            str_push_back(lexer_out_data, '\0');

            copy_data_token(o2->name);
        }
    }
    else
    {
        if(o2->type==CONST)
        {
            //lexer_update_constants(o2->name);
//
            str_push_back(lexer_out_data, ASSIGNMENT);

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'a');
            str_push_back(lexer_out_data, '\0');

            copy_data_token(o2->name);
//
            str_push_back(lexer_out_data, type);

            copy_data_token(in_var->name);
            copy_data_token(o1->name);

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'a');
            str_push_back(lexer_out_data, '\0');
        }
        else
        {
            str_push_back(lexer_out_data, type);

            copy_data_token(in_var->name);
            copy_data_token(o1->name);
            copy_data_token(o2->name);
        }
    }

    /*strcpy(out+*ind, in_var->id);
    *ind+=strlen(in_var->id)+1;

    strcpy(out+*ind, o1->id);
    *ind+=strlen(o1->id)+1;

    strcpy(out+*ind, o2->id);
    *ind+=strlen(o2->id)+1;*/
}

void new_assembler_operation2(char type, Type *in_var, Type *o)
{
    int num;
    int i;
    char *id;

    if(o->type==CONST)
    {
        //lexer_update_constants(o->name);

        str_push_back(lexer_out_data, type);

        copy_data_token(in_var->name);
        copy_data_token(o->name);
    }
    else
    {
        str_push_back(lexer_out_data, type);

        copy_data_token(in_var->name);
        copy_data_token(o->name);
    }
}

Type* calc_expression(List *l, Function *cur_function, Stack *stack_functions)
{
    struct ListNode *i=l->begin;
    Stack *s=stack_init();
    ListExpressionData *data;
    Type *o1, *o2;
    char *num;

    Type *var_alloc;
    String *var_name;

    while(i)
    {
        data=(ListExpressionData*)i->data;
        if(data->type==OPERATION)
        {
            o2=(Type*)pop(s);
            if(s->begin==0)
            {
                printf("%d: expected ')'\n", line);
                return 0;
            }
            else
                o1=(Type*)pop(s);

            var_name=str_init(generate_id(lexer_id));
            printf("<");str_print(var_name);printf(">");
            var_alloc=new_type(var_name, INTEGER, 0);

            str_push_back(lexer_out_data, INT_INIT);
            copy_data_token(var_name);
            //tree_add(lexer_constants->types, (char*)var_alloc, tree_type_cmp);

            switch((int)data->data)
            {
                case '+':
                    if(debug)
                    {
                        str_print(var_alloc->name);printf("=");
                        str_print(o2->name); printf("+");
                        str_print(o1->name); printf("\n");
                    }
                    new_assembler_operation3(ADD, var_alloc, o1, o2);
                    break;

                case '-':
                    if(debug)
                    {
                        str_print(var_alloc->name); printf("=");
                        str_print(o2->name); printf("-");
                        str_print(o1->name); printf("\n");
                    }
                    new_assembler_operation3(SUB, var_alloc, o1, o2);
                    break;

                case '/':
                    if(debug)
                    {
                        str_print(var_alloc->name); printf("=");
                        str_print(o2->name); printf("/");
                        str_print(o1->name); printf("\n");
                    }
                    new_assembler_operation3(DIV, var_alloc, o1, o2);
                    break;

                case '*':
                    if(debug)
                    {
                        str_print(var_alloc->name); printf("=");
                        str_print(o2->name); printf("*");
                        str_print(o1->name); printf("\n");
                    }
                    new_assembler_operation3(MUL, var_alloc, o1, o2);
                    break;
            }

            push(s, var_alloc);
        }
        else if(data->type==INTEGER)
        {
            push(s, data->data);
        }
        else if(data->type==CONST)
        {
            push(s, data->data);
        }
        else if(data->type==CALL)
        {
        }

        i=i->next;
    }

    o1=pop(s);
    /*
    if(debug)
    {
        str_print(in_var->name); printf("=");
        str_print(o1->name); printf("\n");
    }
    new_assembler_operation2(ASSIGNMENT, in_var, o1);
*/
    stack_free(s);
    return o1;
}

ListExpressionData *new_expr(char *data, int type)
{
    ListExpressionData *alloc=malloc(sizeof(ListExpressionData));

    alloc->data=data;
    alloc->type=type;

    return alloc;
}

void print_OPN(List *l)
{
    ListExpressionData *expr;
    Type *tree_data;

    struct ListNode *k=l->begin;
    while(k)
    {
        expr=k->data;
        if(expr->type==INTEGER)
        {
            tree_data=expr->data;
            str_print(tree_data->name);
            printf(" ");
        }
        else if(expr->type==OPERATION)
        {
            printf("%c ", expr->data);
        }
        else if(expr->type==CONST)
        {
            tree_data=expr->data;
            str_print(tree_data->name);
            printf(" ");
        }

        k=k->next;
    }
    printf("\n");
}

char add_expr_var(String *out, Tree *types, List *l)
{
    Type *type;

    if(out->length)
    {
        type=find_type(types, out);
        if(type==0)
        {
            printf("%d: variable '"); str_print(out); printf("' not found\n", line, out);
            return 0;
        }
        list_push(l, new_expr(type, INTEGER));
    }
    str_clear(out);
}

void add_expr_const(String *out, List *l)
{
    Type *type;
    String *s;

    if(out->length)
    {
        type=find_type(lexer_constants->types, out);

        if(type==0)
        {
            s=str_init("");
            str_copy(s, out);
            lexer_update_constants(s);
            add_type(lexer_constants, 0, s, CONST);
            type=find_type(lexer_constants->types, s);
        }
        list_push(l, new_expr(type, CONST));
    }
    str_clear(out);
}

#define EXPRESSION 0x99

void add_expr(String *out, char type, Function *cur_function, List *l)
{
    switch(type)
    {
    case INTEGER:
        add_expr_var(out, cur_function->types, l);
        break;

    case CONST:
        add_expr_const(out, l);
        break;
    }
}

Type* get_expression(Function *cur_function, Stack *stack_functions)
{
    char expr_type=UNDEFINED;

    String *out=str_init("");
    Stack *op=stack_init();
    char c, is_operation=0, is_expr=1, *operation, is_integer=0, is_const=0;
    int number;
    List *l=list_init();

    Type *type;

    char is_gap=0;

    while(buf->end && is_expr)
    {
        if(!is_integer && is_number(buf->end->data))
        {
            str_push(out, str_pop(buf));

            expr_type=CONST;

            is_operation=1;
            is_gap=0;
        }
        else if(is_symbol(buf->end->data) || (buf->end->data=='-' && !is_operation))
        {
            if((out->length==0 || is_gap) && is_operation)
            {
                is_expr=0;
                break;
            }

            str_push(out, str_pop(buf));

            expr_type=INTEGER;

            is_operation=1;
            is_gap=0;
        }
        else if(buf->end->data==' ' || buf->end->data=='\t' || buf->end->data==10)
        {
            add_expr(out, expr_type, cur_function, l);

            str_pop(buf);

            is_gap=1;
        }
        else if(buf->end->data==13)
        {
            line++;
            str_pop(buf);
        }
        else
        {
            add_expr(out, expr_type, cur_function, l);

            switch(buf->end->data)
            {
                case '(':
                    push(op, '(');
                    str_pop(buf);
                    break;

                case ')':
                    if(op->begin==0)
                    {
                        printf("%d: '(' expected\n", line);
                        return 0;
                    }

                    if(op->begin->data=='(')
                    {
                        printf("%d: braces '()' not contains expression\n");
                        return 0;
                    }

                    while(op->begin->data!='(')
                    {
                        list_push(l, new_expr(pop(op), OPERATION));
                        if(op->begin==0)
                        {
                            printf("%d: '(' expected\n", line);
                            return 0;
                        }
                    }
                    pop(op);
                    str_pop(buf);
                    break;

                case '+': case '-': case '*': case '/':
                    if(is_operation==0)
                    {
                        printf("%d: invalid expression\n", line);
                        return 0;
                    }
                    is_operation=0;

                    if(op->begin==0 || prior(op->begin->data)<prior(buf->end->data))
                        push(op, buf->end->data);
                    else
                    {
                        while(op->begin && prior(op->begin->data) >= prior(buf->end->data))
                            list_push(l, new_expr(pop(op), OPERATION));

                        push(op, buf->end->data);
                    }
                    str_pop(buf);
                    break;

                default:
                    is_expr=0;
                    break;
            }

            is_gap=0;
            expr_type=UNDEFINED;
        }
    }

    if(out->length)
        add_expr(out, expr_type, cur_function, l);

    while(op->begin)
        list_push(l, new_expr(pop(op), OPERATION));

    if(debug)
        print_OPN(l);

    return calc_expression(l, cur_function, stack_functions);
}
