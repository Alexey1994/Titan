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

void new_assembler_var(char *name, int *i, char *data)
{
    data[*i]=VAR_INIT; *i+=1;
    strcpy(data+*i, name); *i+=strlen(name)+1;
}

void new_assembler_assignment2(char *out, char *in, int *i, char *data)
{
    data[*i]=ASSIGNMENT; *i+=1;
    strcpy(data+*i, out); *i+=strlen(out)+1;
    strcpy(data+*i, in); *i+=strlen(in)+1;
}

void new_assembler_alloc2(char *out, char *sz, int *i, char *data)
{
    data[*i]=ALLOC; *i+=1;
    strcpy(data+*i, out); *i+=strlen(out)+1;
    strcpy(data+*i, sz); *i+=strlen(sz)+1;
}

void new_assembler_assignment3(char *out, char *ind, char *in, int *i, char *data)
{
    data[*i]=ASSIGNMENT; *i+=1;
    strcpy(data+*i, out); *i+=strlen(out)+1;
    strcpy(data+*i, ind); *i+=strlen(ind)+1;
    strcpy(data+*i, in); *i+=strlen(in)+1;
}

void new_assembler_inc(char *name, int *i, char *data)
{
    data[*i]=INC; *i+=1;
    strcpy(data+*i, name); *i+=strlen(name)+1;
}

void new_assembler_ptrs_init(char *name, int *i, char *data)
{
    data[*i]=PTRS_INIT; *i+=1;
    strcpy(data+*i, name); *i+=strlen(name)+1;
}

PtrsData *new_tree_ptrs(char *name, Type *data,int index)
{
    PtrsData *ret=malloc(sizeof(PtrsData));
    ret->index=index;
    ret->name=name;
    ret->data=data;
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

void get_pointers(char *ptrs, int *i, char *data)
{
    /*
    Tree *ptrs_ret=tree_init(new_tree_ptrs("Alexey", new_tree_data("0a", 0, VARIABLE), 0));
    char *s;

    skip();
    if(buf[j]!='{')
    {
        printf("line %d: excepted '{'", line);
        return 0;
    }
    j++;

    new_assembler_alloc2(ptrs, "sz", i, data);
    new_assembler_assignment2("0a", "0", i, data);

    while(buf[j]!='}')
    {
        skip();

        switch(buf[j])
        {
        case 'Z':
            {
                printf("%c", buf[j]);
                j++;
ptrs_get_z:
                skip();
                s=get_word();
                printf(s);
                skip();

                s=generate_id(id_const);

                new_assembler_var(s, i, data);
                new_assembler_assignment3(ptrs, "0a", s, i, data);
                new_assembler_inc("0a", i, data);

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
                break;
            }
        }
    }

    j++;
    printf("\n\n");*/
}

void init_constants_and_registers(String *data, Function *cur_function)
{
    add_type(cur_function, 0, str_init(""), VARIABLE);
    str_push_back(data, VAR_INIT);
    str_push_back(data, '\0');

    add_type(cur_function, 0, str_init("0"), CONST);
    str_push_back(data, CONST_INIT);
    str_push_back(data, '0');
    str_push_back(data, '\0');
    str_push_back(data, 0);
    str_push_back(data, 0);
    str_push_back(data, 0);
    str_push_back(data, 0);

    str_push_back(data, ASSIGNMENT);
    str_push_back(data, '\0');
    str_push_back(data, '0');
    str_push_back(data, '\0');

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

    add_type(cur_function, 0, str_init("0v"), VARIABLE);
    str_push_back(data, VAR_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'v');
    str_push_back(data, '\0');

    add_type(cur_function, 0, str_init("0s"), VARIABLE);
    str_push_back(data, VAR_INIT);
    str_push_back(data, '0');
    str_push_back(data, 's');
    str_push_back(data, '\0');

    add_type(cur_function, 0, str_init("0i"), VARIABLE);
    str_push_back(data, VAR_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'i');
    str_push_back(data, '\0');

    str_push_back(data, VAR_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'a');
    str_push_back(data, '\0');

    str_push_back(data, VAR_INIT);
    str_push_back(data, '0');
    str_push_back(data, 'd');
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

    Variable *t_var;

    FILE *f=fopen(name, "rb");

    lexer_out_data=str_init("");

    String *tmp, *tmp2;
    Function *cur_function=new_function(str_init("")), *tmp_function, *function_alloc;
    Tree *fun=tree_init((char*)cur_function);

    Stack *stack_functions=stack_init();

    Type *tree_data_tmp, *tree_data_tmp2, *tree_data_tmp3, *tree_data_tmp4;
    int i, k, l;

    int num, delta_line;
    char *id;

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
            tree_add(cur_function->functions, function_alloc, function_cmp);
            cur_function=function_alloc;

            str_push_back(lexer_out_data, FUNCTION);
            copy_data_token(tmp);

            str_clear(tmp);
        }
        else if(str_comparision(tmp, token_end)==0)
        {
            str_push_back(lexer_out_data, END);
        }
        else if(str_comparision(tmp, token_integer)==0)
        {
get_var:
            str_push_back(lexer_out_data, VAR_INIT);

            skip();
            tmp=get_word();

            if(tree_contains(cur_function, tmp))
            {
                printf("variable '%s' is defined\n", tmp);
                return 0;
            }

            add_type(cur_function, 0, tmp, VARIABLE);
            copy_data_token(tmp);

            skip();
            if(buf->end && buf->end->data=='=')
            {
                str_pop(buf);
                tree_data_tmp=find_type(cur_function->types, tmp);
                tree_data_tmp2=find_type(cur_function->types, token_empty);
                get_expression(cur_function, stack_functions, tree_data_tmp, tree_data_tmp2);
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
                        tree_data_tmp=find_global_type(cur_function, stack_functions, token_empty);
                        tree_data_tmp2=find_global_type(cur_function, stack_functions, token_empty);
                        get_expression(cur_function, stack_functions, tree_data_tmp, tree_data_tmp2);

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
                //get_pointers(tmp, &i, data);

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
            tree_data_tmp=find_global_type(cur_function, stack_functions, token_empty);
            tree_data_tmp2=find_global_type(cur_function, stack_functions, token_empty);
            get_expression(cur_function, stack_functions, tree_data_tmp, tree_data_tmp2);

            str_push_back(lexer_out_data, LOOP);

            str_push_back(lexer_out_data, ASSIGNMENT);
            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'v');
            str_push_back(lexer_out_data, '\0');

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, '\0');

            str_push_back(lexer_out_data, EQ);
            str_push_back(lexer_out_data, '\0');
            str_push_back(lexer_out_data, '\0');

            str_push_back(lexer_out_data, '0');
            str_push_back(lexer_out_data, 'v');
            str_push_back(lexer_out_data, '\0');

            str_push_back(lexer_out_data, IF);
            str_push_back(lexer_out_data, '\0');
            str_push_back(lexer_out_data, BREAK);
            str_push_back(lexer_out_data, END);
        }
        else if(str_comparision(tmp, token_break)==0)
        {
            str_push_back(lexer_out_data, BREAK);
        }
        else if(str_comparision(tmp, token_if)==0)
        {
            tree_data_tmp=find_global_type(cur_function, stack_functions, token_empty);
            tree_data_tmp2=find_global_type(cur_function, stack_functions, token_empty);
            get_expression(cur_function, stack_functions, tree_data_tmp, tree_data_tmp2);

            str_push_back(lexer_out_data, IF);
            str_push_back(lexer_out_data, '\0');
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
        }/*
        else
        {
            tree_data_tmp=find_tree_element(cur_function, tmp);
            if(tree_data_tmp==0 )//|| tree_data_tmp->name[0]=='\0')
            {
                /*
                tmp_function=find_function(fun, tmp);
                if(tmp_function && tmp_function->name[0]!='\0')
                {
                    data[i]=CALL;
                    i++;

                    strcpy(data+i, tmp);
                    i+=strlen(tmp)+1;
                }
                else
                {
                    if(tmp[0]) printf("%d: error: '%s'\n", line, tmp);
                    else if(buf[j]=='\'' || buf[j]=='"') printf("%d: error: %c\n", line, buf[j]);
                    else printf("%d: error: '%c'\n", line, buf[j]);

                    return 0;
                }*/
            /*
            }
            else
            {
                skip();

                if(buf[j]=='[')
                {
                    if(tree_data_tmp->type!=ELEMENT)
                    {
                        printf("%d: index for variable '%s'\n", line, tree_data_tmp->name);
                        return 0;
                    }

                    j++;
                    tree_data_tmp2=find_tree_element(cur_function, "0i");
                    get_expression(cur_function, global_types, tree_data_tmp2, tree_data_tmp2, data, &i);

                    if(buf[j]==']')
                    {
                        j++;
                    }
                    else
                    {
                        printf("%d: expected ']'\n", line);
                        return 0;
                    }

                    skip();
                    if(buf[j]=='=')
                    {
                        j++;
                        tree_data_tmp3=find_tree_element(cur_function, "");
                        tree_data_tmp2=find_tree_element(cur_function, "");
                        get_expression(cur_function, global_types, tree_data_tmp3, tree_data_tmp2, data, &i);

                        data[i]=ASSIGNMENT; i++;

                        strcpy(data+i, tmp); i+=strlen(tmp)+1;

                        data[i]='\0'; i++;

                        data[i]='0'; i++;
                        data[i]='i'; i++;
                        data[i]='\0'; i++;
                    }
                    else
                    {
                        printf("%d: expected '='\n", line);
                        return 0;
                    }
                }
                else if(buf[j]=='=')
                {
                    j++;
                    tree_data_tmp2=find_tree_element(cur_function, "");
                    get_expression(cur_function, global_types, tree_data_tmp, tree_data_tmp2, data, &i);
                }
                else
                {
                    printf("%d: expected '='\n", line);
                    return 0;
                }
            }
        }*/
        else
        {
            printf("error %c\n", buf->end->data);
            return 0;
        }
    }

    //printf("\nglobal:\n");
    //print_functions(fun);

    return lexer_out_data;
}
