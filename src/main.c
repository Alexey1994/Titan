#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "interpretator.h"
#include "lexer.h"
#include "expression.h"
#include "String.h"
#include "parser_free.h"
#include "time.h"
#include "C.h"

#include "make_PE.h"

char debug=0, disasm=0;

void free_test()
{
    Tree *tree=0;
    char data[]={
                 INT_INIT, 'n',0,
                 INT_INIT, 'm',0,
                 INT_INIT, 't',0,
                 ARRAY_INIT, 'g',0,
                 ELEMENT_INIT, 'e',0,
                 PTRS_INIT, 'p',0,
                 CONST_STRING_INIT, 'z',0, 3,0,0,0, 't','l','l',
                 CONST_INIT, 'c',0, 1,0,0,0,
                 CONST_INIT, '0',0, 0,0,0,1,

                 ASSIGNMENT, 'n',0, 'c',0,
                 ASSIGNMENT, 'm',0, '0',0,
                 ASSIGNMENT, 'e',0, 'e',0,

                 PRINT, 'n',0,

                 LOOP,
                    //EQ, 't',0, 'm',0, 'n',0,
                    IF, 't',0,
                        BREAK,
                    END,
                    INC, 'n',0,
                 END,

                 INC, 'n',0,

                 IF, 'n',0,
                    ASSIGNMENT, 'm',0, '0',0,
                 END,

                 FUNCTION, 'a',0,
                    FUNCTION, 'o',0,
                        INT_INIT, 'l',0,
                    END,
                 END,

                 //EQ, 't',0, 'm',0, 'n',0,

                 CALL, 'a',0
                };

    size_data=sizeof(data);

    String *s=str_init("");

    while(1)
    {
        int g;
        for(g=size_data-1; g>=0; g--)
            str_push(s, data[g]);
        tree=parse(s);

        parser_free_functions(tree);
        str_clear(s);
    }
}

void power_test()
{
    Tree *tree=0;
    int start;

    char data[]={
                 INT_INIT, 'i',0,
                 INT_INIT, 't',0,
                 INT_INIT, 'v',0,
                 CONST_INIT, 's',0, 0,0,0,1,
                 CONST_INIT, '0',0, 0,0,0,0,

                 ASSIGNMENT, 'i',0, '0',0,
                 ASSIGNMENT, 't',0, 's',0,

                 ASSIGNMENT, 'v',0, 's',0,

                 LOOP,
                    //EQ, 'v',0, 't',0, 'i',0,
                    INC, 'i',0,
                    IF, 'v',0,
                        BREAK,
                        PRINT, 'v',0,
                    END,
                 END,

                 PRINT, 'i',0
                };
    size_data=sizeof(data);

    String *s=str_init("");

    printf("\nparser:\n\n");

    int g;
    for(g=size_data-1; g>=0; g--)
        str_push(s, data[g]);
    tree=parse(s);

    printf("\ninterpretator:\n\n");

    start=time(0);
    if(tree)
        run(tree);
    else
        printf("error");
    printf("\n%d seconds", (time(0)-start));
}

void if_test()
{
    Tree *tree=0;

    interpretator_table_init();

    char data[]={
                 CONST_INIT, 'c',0, 1,0,0,0,
                 INT_INIT, 's',0,

                 ASSIGNMENT, 's',0, 'c',0,

                 IF, 's',0,
                    PRINT, 'c',0,
                 END,

                 LOOP,
                    BREAK,
                    CONTINUE,
                    PRINT, 's',0,
                 END,

/*
                 FUNCTION, 'a',0,
                 END,

                 CALL, 'a',0,*/

                 INC, 's',0
                };
    size_data=sizeof(data);

    String *s=str_init("");

    printf("\nparser:\n\n");

    int g;
    for(g=size_data-1; g>=0; g--)
        str_push(s, data[g]);
    tree=parse(s);

    printf("\ninterpretator:\n\n");

    if(tree)
        run(tree);
    else
        printf("error");
}

void c_test()
{
    Tree *tree=0;

    interpretator_table_init();

    char data[]={
                 CONST_INIT, 'c',0, 1,0,0,0,
                 CONST_INIT, 'd',0, 2,0,0,0,
                 INT_INIT, 's',0,

                 ASSIGNMENT, 's',0, 'c',0,

/*
                 FUNCTION, 'a',0,
                 END,

                 CALL, 'a',0,*/

                 INC, 's',0,
                 MUL, 's',0, 's',0, 's',0,
                 PRINT, 's',0
                };
    size_data=sizeof(data);

    String *s=str_init("");

    printf("\nparser:\n\n");

    int g;
    for(g=size_data-1; g>=0; g--)
        str_push(s, data[g]);
    tree=parse(s);

    printf("\ninterpretator:\n\n");

    if(tree)
        translate_C(tree);
    else
        printf("error");
}

void functions_test()
{
    Tree *tree=0;

    interpretator_table_init();

    char data[]={

                 FUNCTION, 'a','d',0,
                    INT_INIT, '0','b',0,
                    CONST_INIT, '1',0, 1,0,0,0,
                    //CONST_INIT, '0',0, 0,0,0,0,
                    ASSIGNMENT, '0','b',0, '1',0,

                    PRINT, '0','b',0,
                 END
                };
    size_data=sizeof(data);

    String *s=str_init("");

    printf("\nparser:\n\n");

    int g;
    for(g=size_data-1; g>=0; g--)
        str_push(s, data[g]);
    tree=parse(s);

    printf("\ninterpretator:\n\n");

    if(tree)
        run(tree);
    else
        printf("error");
}

void arifmetic_test()
{
    Tree *tree=0;
    FILE *f=fopen("bytecode.app", "wb");
    interpretator_table_init();

    char data[]={
                INT_INIT, 'i',0,

                FUNCTION, 'f',0, 1,0,0,0,
                    INT_INIT, 'i',0,
                END,

                ASSIGNMENT, 'i',0, 'i',0, 'f',0, 'i',0, ADD, 0,

        /*
                 CONST_INIT, 'c',0, 1,0,0,0,
                 INT_INIT, 's',0,
                 CONST_STRING_INIT, 'q',0, 2,0,0,0, 'a','l',

                 LOOP,
                    LOOP,
                        ASSIGNMENT, 's',0, 'c',0,
                        LOOP,
                            ASSIGNMENT, 's',0, 'c',0,
                            BREAK,
                        END,

                        BREAK,
                    END,
                    ASSIGNMENT, 's',0, 's',0,
                    BREAK,
                 END,

                 INT_INIT, 'x',0,
                 INT_INIT, 'y',0,
                 INT_INIT, 'z',0,

                 IF, IZ, 'x',0, IZ, 'x',0, AND, 0, //if(!x)
                 END,
*/
/*
                 IF, INZ, 'x',0, 0, //if(x)
                 END,

                 IF, IGT, 'x',0, 'y',0, 0, //if(x>y)
                 END,*/

                 //ASSIGNMENT, 's',0, 's',0,
/*
                 ADD, 's',0, 's',0, 's',0,
                 MUL, 's',0, 's',0, 's',0,
                 DIV, 's',0, 's',0, 's',0,
                 SUB, 's',0, 's',0, 's',0,

                 AND, 's',0, 's',0, 's',0,
                 OR, 's',0, 's',0, 's',0,
                 NOT, 's',0, 's',0,
                 XOR, 's',0, 's',0, 's',0,

                 ASSIGNMENT, 's',0, 'c',0,

                 SHL, 's',0, 's',0, 's',0,
                 SHR, 's',0, 's',0, 's',0,

                 EQ, 's',0, 's',0, 's',0,
                 NEQ, 's',0, 's',0, 's',0,
                 LT, 's',0, 's',0, 's',0,
                 GT, 's',0, 's',0, 's',0,
                 LE, 's',0, 's',0, 's',0,
                 GE, 's',0, 's',0, 's',0,*/
/*
                 FUNCTION, 'f',0, 2,0,0,0,
                    INT_INIT, 'a',0,
                    INT_INIT, 'b',0,

                    PUTC, 'a',0,
                    PUTC, 'b',0,
                 END,

                 CALL, 'f',0, 's',0, 's',0,
*/
/*
                 REAL_INIT, 'r',0,
                 ASSIGNMENT, 'r',0, 'c',0,
                 PUTC, 'r',0,
                 ADD, 's',0, 'r',0, 'r',0,
                 PUTC, 's',0*/
                };
    size_data=sizeof(data);

    String *s=str_init("");

    printf("\nparser:\n\n");

    int g;
    for(g=size_data-1; g>=0; g--)
    {
        fputc(data[g], f);
        str_push(s, data[g]);
    }
    tree=parse(s);

    printf("\ninterpretator:\n\n");

    if(tree)
        pure_run(tree);
        //run(tree);
    else
        printf("error");
    fclose(f);
}

int main()
{
    debug=1;
    disasm=1;
    arifmetic_test(); return 0;
    //functions_test(); return 0;
    //c_test();
    //return 0;

    FILE *f=fopen("bytecode.app","wb");
    int i;

    debug=1;
    disasm=1;

    if(debug)
        printf("\nlexer:\n\n");
    String *data;
    Tree *tree=0;

    data=lexer("example.ti");

    struct ListNode *l=data->end;
    while(l)
    {
        fputc(l->data, f);
        l=l->previouse;
    }
    fclose(f);

    if(debug)
        printf("\nparser:\n\n");
    if(data)
        tree=parse(data);

    if(debug)
        printf("\ninterpretator:\n\n");

    if(tree)
        run(tree);
    else
        printf("error");

    return 0;
}
