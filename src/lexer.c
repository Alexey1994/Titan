#include "lexer.h"
#include "tree.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "parser_cmp.h"

String *buf;
String *lexer_out_data;

void lexer_strcpy(char *s);
void lexer_memcpy(void *data, int sz);

Function *lexer_constants;

Id *lexer_id;

int size_data=0,
    j=0,
    line;

char c=0;

static void lexer_print_args(List *args)
{
    struct ListNode *i=args->begin;
    Type *data;

    printf("(");
    while(i)
    {
        data=i->data;
        str_print(data->name);
        if(i->next)
            printf(",");
        i=i->next;
    }
    printf(")");
}

int lexer_braces_count=0;

static void lexer_print_functions_node(Function *function)
{
    int i;

    lexer_braces_count++;

    for(i=0; i<lexer_braces_count; i++)
        printf("   ");

    str_print(function->name);
    lexer_print_args(function->args);

    printf(":\n");
    //print_variables((Tree*)data->types);
    lexer_print_functions(function->functions);
    lexer_braces_count--;
}

void lexer_print_functions(Tree *tree)
{
    tree_print(tree, lexer_print_functions_node);
}

char is_number(char c)
{
    if(c>='0' && c<='9')
        return 1;
    return 0;
}

char is_symbol(char c)
{
    if((c>='a' && c<='z') || (c>='A' && c<='Z'))
        return 1;
    return 0;
}

String *get_word()
{
    String *ret_alloc=str_init("");

    while(buf->length>0 && is_symbol(buf->end->data))
        str_push(ret_alloc, str_pop(buf));

    return ret_alloc;
}

char *get_number()
{
    String *ret_alloc=str_init("");

    while(buf->length>0 && is_number(buf->end->data))
        str_push(ret_alloc, str_pop(buf));

    return ret_alloc;
}

void skip()
{
    while(1)
    {
        if(buf->length==0)
            return;
        switch(buf->end->data)
        {
        case ' ': case '\t': case 10:
            break;
        case 13:
            line++;
            break;
        default: return;
        }
        str_pop(buf);
    }
}

void new_assembler_var(String *name)
{
    str_push_back(lexer_out_data, INT_INIT);
    copy_data_token(name);
}

void new_assembler_assignment2(String *out, String *in)
{
    str_push_back(lexer_out_data, ASSIGNMENT);
    copy_data_token(out);
    copy_data_token(in);
}

void new_assembler_alloc2(String *out, String *sz)
{
    str_push_back(lexer_out_data, ALLOC);
    copy_data_token(out);
    copy_data_token(sz);
}

void new_assembler_assignment3(String *out, String *ind, String *in)
{
    str_push_back(lexer_out_data, ASSIGNMENT);
    copy_data_token(out);
    copy_data_token(ind);
    copy_data_token(in);
}

void new_assembler_inc(String *name)
{
    str_push_back(lexer_out_data, INC);
    copy_data_token(name);
}

void new_assembler_ptrs_init(char *name, int *i, char *data)
{
    str_push_back(lexer_out_data, PTRS_INIT);
    copy_data_token(name);
}

PtrsMapping *new_ptrs_map_element(String *name, Type *element, unsigned int index)
{
    PtrsMapping *ret=malloc(sizeof(PtrsMapping));
    ret->index=index;
    ret->name_element=name;
    ret->element=element;
    return ret;
}

Type *new_tree_data(char *name, char *data, char type)
{
    Type *ret=malloc(sizeof(Type));
    ret->data=data;
    ret->name=name;
    ret->type=type;
    return ret;
}

void get_pointers(String *name_ptrs, Function *cur_function, Stack *stack_functions)
{
    String *register_0v=str_init("0v"),
           *token_empty=str_init(""),
           *register_0c=str_init("0c"),
           *const_0=str_init("0"),
           *const_1=str_init("11");

    Type *tree_data_tmp, *tree_data_tmp2;

    Tree *ptrs_ret=tree_init(new_ptrs_map_element("Alexey", new_tree_data("0a", 0, INTEGER), 0));
    char *s;
    Type *expr;

    skip();
    if(buf->end==0 || buf->end->data!='{')
    {
        printf("line %d: excepted '{'", line);
        return 0;
    }
    str_pop(buf);

    new_assembler_assignment2(register_0c, const_1);
    new_assembler_alloc2(name_ptrs, register_0c);
    //new_assembler_assignment2("0a", "0", i, data);

    new_assembler_assignment2(register_0c, const_0);

    while(buf->end->data!='}')
    {
get_ptrs:
        skip();

        switch(buf->end->data)
        {
        default:
            {
                expr=get_expression(cur_function, stack_functions);
                new_assembler_assignment3(name_ptrs, register_0c, expr->name);
                new_assembler_inc(register_0c);
            }
            break;

        case 'Z':
            {
                printf("%c", buf->end->data);
                str_pop(buf);
                if(buf->end==0)
                {
                    printf("%d: excepted }", line);
                    return 0;
                }
ptrs_get_z:
                skip();
                s=get_word();
                if(buf->end==0)
                {
                    printf("%d: excepted }", line);
                    return 0;
                }

                printf(s);
                skip();

                s=generate_id(lexer_id);

//                new_assembler_var(s, i, data);
//                new_assembler_assignment3(ptrs, "0a", s, i, data);
//                new_assembler_inc("0a", i, data);
/*
                if(buf[j]=='=')
                {
                    j++;
                    skip();
                    s=get_number();
                    printf(s);
                    skip();
                }
                if(buf[j]==',')
                {
                    j++;
                    goto ptrs_get_z;
                }
                break;*/
            }
        }
    }

    str_pop(buf);
    printf("\n\n");
}

void init_constants_and_registers(String *data, Function *cur_function)
{
    add_type(cur_function, 0, str_init(""), INTEGER);
    str_push_back(data, INT_INIT);
    str_push_back(data, '\0');

    add_type(cur_function, 0, str_init("0"), CONST);
    str_push_back(data, CONST_INIT);
    str_push_back(data, '0');
    str_push_back(data, '\0');
    str_push_back(data, 0);
    str_push_back(data, 0);
    str_push_back(data, 0);
    str_push_back(data, 0);
/*
    str_push_back(data, ASSIGNMENT);
    str_push_back(data, '\0');
    str_push_back(data, '0');
    str_push_back(data, '\0');*/

    add_type(cur_function, 0, str_init("1"), CONST);
    str_push_back(data, CONST_INIT);
    str_push_back(data, '1');
    str_push_back(data, '\0');
    str_push_back(data, 1);
    str_push_back(data, 0);
    str_push_back(data, 0);
    str_push_back(data, 0);

    str_push_back(data, CONST_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'b');
    str_push_back(data, '\0');
    str_push_back(data, BITS);
    str_push_back(data, 0);
    str_push_back(data, 0);
    str_push_back(data, 0);

    add_type(cur_function, 0, str_init("0v"), INTEGER);
    str_push_back(data, INT_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'v');
    str_push_back(data, '\0');

    add_type(cur_function, 0, str_init("0s"), INTEGER);
    str_push_back(data, INT_INIT);
    str_push_back(data, '0');
    str_push_back(data, 's');
    str_push_back(data, '\0');

    add_type(cur_function, 0, str_init("0i"), INTEGER);
    str_push_back(data, INT_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'i');
    str_push_back(data, '\0');

    str_push_back(data, INT_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'a');
    str_push_back(data, '\0');

    str_push_back(data, INT_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'd');
    str_push_back(data, '\0');

    str_push_back(data, INT_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'c');
    str_push_back(data, '\0');
}

void copy_data_token(String *tok)
{
    struct StringNode *i=tok->begin;

    while(i)
    {
        str_push_back(lexer_out_data, i->data);
        i=i->next;
    }
    str_push_back(lexer_out_data, '\0');
}

String *lexer(char *name)
{
    String *token_while=str_init("while"),
           *token_if=str_init("if"),
           *token_const=str_init("Const"),
           *token_end=str_init("end"),
           *token_integer=str_init("Z"),
           *token_empty=str_init(""),
           *token_print=str_init("print"),
           *token_function=str_init("function"),
           *token_break=str_init("break"),
           *token_new=str_init("new"),
           *token_element=str_init("Element"),
           *token_pointers=str_init("Pointers"),

           *register_0v=str_init("0v");

    Type *expr1, *expr2, *type_tmp;

    Number *t_var;

    FILE *f=fopen(name, "rb");

    lexer_out_data=str_init("");

    String *tmp, *tmp2, *args=str_init("");
    Function *cur_function=new_function(str_init("")), *tmp_function, *function_alloc;
    Tree *fun=tree_init();//(char*)cur_function);
    tree_add(fun, (char*)cur_function, function_cmp);

    Stack *stack_functions=stack_init();

    Type *tree_data_tmp, *tree_data_tmp2, *tree_data_tmp3, *tree_data_tmp4;
    int i, k, l;

    int num, delta_line;
    char *id;

    cur_function->args=list_init();
    line=1;
    lexer_id=id_init(128, 255);
    Const *constant;

    char t;

    lexer_constants=cur_function;
    //push(stack_functions, cur_function);

    buf=str_init("");
    while(1)
    {
        t=fgetc(f);
        if(feof(f))
            break;
        str_push_back(buf, t);
    }
    fclose(f);

    init_constants_and_registers(lexer_out_data, cur_function);

    while(buf->length>0)
    {
        skip();
        tmp=get_word();

        if(str_comparision(tmp, token_function)==0)
        {
            skip();
            tmp=get_word();

            if(tree_contains(cur_function, tmp))
            {
                printf("function '%s' is defined\n", tmp);
                return 0;
            }

            push(stack_functions, cur_function);
            function_alloc=new_function(tmp);
            function_alloc->args=list_init();
            if(cur_function->functions)
                tree_add(cur_function->functions, function_alloc, function_cmp);
            else
                cur_function->functions=tree_init(function_alloc);
            cur_function=function_alloc;

            str_push_back(lexer_out_data, FUNCTION);
            copy_data_token(tmp);

            skip();
            if(buf->end==0 || buf->end->data!='(')
            {
                printf("%d :excepted (\n", line);
                return 0;
            }
            str_pop(buf);

get_args:
            skip();
            args=get_word();

            if(args->length)
            {
                if(str_comparision(token_integer, args)==0)
                {
                    skip();
                    tmp=get_word();
                    add_type(cur_function, 0, tmp, INTEGER);
                    type_tmp=find_type(cur_function->types, tmp);
                    list_push(cur_function->args, type_tmp);
                    new_assembler_var(tmp);
                }
                else
                {
                    printf("%d: undefined type ", line);
                    str_print(args);
                    printf("\n");
                    return 0;
                }

                skip();
                if(buf->end && buf->end->data==',')
                {
                    str_pop(buf);
                    goto get_args;
                }

                if(buf->end==0)
                {
                    printf("%d: excepted )\n", line);
                    return 0;
                }
            }

            str_pop(buf);
        }
        else if(str_comparision(tmp, token_end)==0)
        {
            str_push_back(lexer_out_data, END);
        }
        else if(str_comparision(tmp, token_integer)==0)
        {
get_var:
            str_push_back(lexer_out_data, INT_INIT);

            skip();
            tmp=get_word();

            if(tree_contains(cur_function, tmp))
            {
                printf("variable '%s' is defined\n", tmp);
                return 0;
            }

            add_type(cur_function, 0, tmp, INTEGER);
            copy_data_token(tmp);

            skip();
            if(buf->end && buf->end->data=='=')
            {
                str_pop(buf);
                expr1=get_expression(cur_function, stack_functions);
                new_assembler_assignment2(tmp, expr1->name);
            }

            if(buf->end && buf->end->data==',')
            {
                str_pop(buf);
                goto get_var;
            }
        }
        else if(str_comparision(tmp, token_const)==0)
        {
get_const:
            skip();
            tmp=get_word();

            if(tree_contains(cur_function, tmp))
            {
                printf("constant '");
                str_print(tmp);
                printf("' is defined\n");
                return 0;
            }

            add_type(cur_function, 0, tmp, CONST);

            str_push_back(lexer_out_data, CONST_INIT);

            while(tmp->length>0)
                str_push_back(lexer_out_data, str_pop_back(tmp));
            str_push_back(lexer_out_data, '\0');

            skip();

            if(buf->end && buf->end->data=='=')
            {
                str_pop(buf);
                skip();

                if(buf->end && !is_number(buf->end->data))
                {
                    printf("%d: not found number for constant '%s'\n", line, tmp);
                    return 0;
                }
                tmp=get_number();

                num=atoi(tmp);

                for(i=0; i<BITS; i++)
                    str_push_back(lexer_out_data, ((char*)&num)[i]);
            }
            else
            {
                printf("%d: uninitialized constant '%s'\n", line, tmp);
                return 0;
            }

            skip();
            if(buf->end && buf->end->data==',')
            {
                str_pop(buf);
                goto get_const;
            }
        }
        else if(str_comparision(tmp, token_element)==0)
        {
get_element:
            str_push_back(lexer_out_data, ELEMENT_INIT);

            skip();
            tmp=get_word();

            if(tree_contains(cur_function, tmp))
            {
                printf("variable '%s' is defined\n", tmp);
                return 0;
            }

            add_type(cur_function, 0, tmp, ELEMENT);

            copy_data_token(tmp);

            skip();
            if(buf->end && buf->end->data=='=')
            {
                str_pop(buf);
                skip();

                if(buf->end && buf->end->data=='"')
                {
                    str_pop(buf);
                    tmp2=str_init("");

                    delta_line=0;
                    while(buf->end && buf->end->data!='"')
                    {
                        if(buf->end->data=='\n')
                            delta_line++;
                        str_push(tmp2, str_pop(buf));
                    }

                    if(buf->end==0)
                    {
                        printf("%d: expected \"\n", line);
                        return 0;
                    }
                    str_pop(buf);
                    line+=delta_line;

                    id=generate_id(lexer_id);
                    num=tmp2->length+1;

                    str_push_back(lexer_out_data, CONST_STRING_INIT);
                    lexer_strcpy(id);
                    lexer_memcpy(&num, BITS);
                    copy_data_token(tmp2);

                    str_push_back(lexer_out_data, ASSIGNMENT);
                    copy_data_token(tmp);
                    lexer_strcpy(id);
                }
                else
                {
                    tmp2=get_word();

                    if(str_comparision(tmp2, token_new)==0)
                    {
                        expr1=get_expression(cur_function, stack_functions);
                        new_assembler_assignment2(token_empty, expr1->name);

                        str_push_back(lexer_out_data, ALLOC);
                        copy_data_token(tmp);
                        str_push_back(lexer_out_data, '\0');
                    }
                    else
                    {
                        printf("%d: ozhidalosj new ili strokovaja konstanta\n", line);
                        return 0;
                    }
                }
            }

            if(buf->end && buf->end->data==',')
            {
                str_pop(buf);
                goto get_element;
            }
        }
        else if(str_comparision(tmp, token_pointers)==0)
        {
get_pointer:
            str_push_back(lexer_out_data, PTRS_INIT);

            skip();
            tmp=get_word();

            if(tree_contains(cur_function, tmp))
            {
                printf("'%s' is defined\n", tmp);
                return 0;
            }

            add_type(cur_function, 0, tmp, PTRS);
            copy_data_token(tmp);

            skip();
            if(buf->end && buf->end->data=='=')
            {
                str_pop(buf);
                get_pointers(tmp, cur_function, stack_functions);

                //tree_data_tmp=find_global_and_local_element(cur_function, global_types, tmp);
                //tree_data_tmp2=find_global_and_local_element(cur_function, global_types, "");
                //get_expression(cur_function, global_types, tree_data_tmp, tree_data_tmp2, data, &i);

            }

            if(buf->end && buf->end->data==',')
            {
                str_pop(buf);
                goto get_pointer;
            }
        }
        else if(str_comparision(tmp, token_while)==0)
        {
            expr1=get_expression(cur_function, stack_functions);

            str_push_back(lexer_out_data, LOOP);

            str_push_back(lexer_out_data, ASSIGNMENT);
            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'v');
            str_push_back(lexer_out_data, '\0');

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, '\0');

            str_push_back(lexer_out_data, EQ);
            copy_data_token(expr1->name);
            copy_data_token(expr1->name);

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'v');
            str_push_back(lexer_out_data, '\0');

            str_push_back(lexer_out_data, IF);
            copy_data_token(expr1->name);
            str_push_back(lexer_out_data, BREAK);
            str_push_back(lexer_out_data, END);
        }
        else if(str_comparision(tmp, token_break)==0)
        {
            str_push_back(lexer_out_data, BREAK);
        }
        else if(str_comparision(tmp, token_if)==0)
        {
            expr1=get_expression(cur_function, stack_functions);
            str_push_back(lexer_out_data, IF);
            copy_data_token(expr1->name);
        }
        else if(str_comparision(tmp, token_print)==0)
        {
get_print:
            skip();

            if(buf->end && buf->end->data=='"')
            {
                str_pop(buf);

                str_clear(tmp);
                delta_line=0;
                while(buf->end && buf->end->data!='"')
                {
                    if(buf->end->data=='\n')
                        delta_line++;
                    str_push(tmp, str_pop(buf));
                }

                if(buf->end==0)
                {
                    printf("%d: expected \"\n", line);
                    return 0;
                }
                line+=delta_line;
                str_pop(buf);

                num=tmp->length+1;
                id=generate_id(lexer_id);

                str_push_back(lexer_out_data, CONST_STRING_INIT);
                lexer_strcpy(id);
                lexer_memcpy(&num, BITS);
                copy_data_token(tmp);

                str_push_back(lexer_out_data, PUTC);
                lexer_strcpy(id);
            }
            else
            {
                tmp=get_word();

                tree_data_tmp=find_global_type(cur_function, stack_functions, tmp);
                if(tree_data_tmp==0)
                {
                    printf("%d: '%s' not found\n", tmp);
                    return 0;
                }

                str_push_back(lexer_out_data, PUTC);
                copy_data_token(tmp);
            }

            skip();
            if(buf->end && buf->end->data==',')
            {
                str_pop(buf);
                goto get_print;
            }
        }
        else if(buf->end && buf->end->data=='#')
        {
            while(buf->end && buf->end->data!='\n')
                str_pop(buf);
            line++;
        }
        else
        {
            tree_data_tmp=find_global_type(cur_function, stack_functions, tmp);
            if(tree_data_tmp==0)
            {
                tmp_function=find_global_function(cur_function, cur_function->functions, stack_functions, tmp);
                if(tmp_function)
                {
                    skip();

                    if(buf->end && buf->end->data=='(')
                    {
                        str_pop(buf);
                        skip();

                        while(buf->end && buf->end->data!=')')
                        {
                            expr1=get_expression(cur_function, stack_functions);
                        }

                        if(buf->end && buf->end->data==')')
                            str_pop(buf);
                        else
                        {
                            printf("%d: excepted )\n", line);
                            return 0;
                        }
                    }
                    else
                    {
                        printf("%d: excepted (\n", line);
                        return 0;
                    }

                    str_push_back(lexer_out_data, CALL);
                    copy_data_token(tmp);
                }
                else
                {
                    printf("%d: not found expression for '", line);
                    str_print(tmp);
                    printf("'\n");

                    return 0;
                }
            }
            else
            {
                skip();

                if(buf->end && buf->end->data=='[')
                {
                    if(tree_data_tmp->type!=ELEMENT)
                    {
                        printf("%d: index for variable '%s'\n", line, tree_data_tmp->name);
                        return 0;
                    }

                    str_pop(buf);
                    expr1=get_expression(cur_function, stack_functions);

                    if(buf->end && buf->end->data==']')
                    {
                        str_pop(buf);
                    }
                    else
                    {
                        printf("%d: expected ']'\n", line);
                        return 0;
                    }

                    skip();
                    if(buf->end && buf->end->data=='=')
                    {
                        str_pop(buf);
                        expr2=get_expression(cur_function, stack_functions);

                        new_assembler_assignment3(tmp, expr2->name, expr1->name);
                    }
                    else
                    {
                        printf("%d: expected '='\n", line);
                        return 0;
                    }
                }
                else if(buf->end && buf->end->data=='=')
                {
                    str_pop(buf);
                    expr1=get_expression(cur_function, stack_functions);
                    new_assembler_assignment2(tmp, expr1->name);
                }
                else
                {
                    printf("%d: expected '='\n", line);
                    return 0;
                }
            }
        }
    }

    printf("\nFunctions:\n");
    lexer_print_functions(fun);

    return lexer_out_data;
}
