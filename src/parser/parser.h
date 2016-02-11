#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "../stack.h"
#include "../List.h"
#include "../tree.h"
#include "../Id.h"
#include "../String.h"

typedef char try;

#define BITS                  4

/*комманды ассемблера*/
#define PTRS_INIT             0x01
#define ELEMENT_INIT          0x03
#define INT_INIT              0x04
#define REAL_INIT             0x05
#define CONST_INIT            0x06
#define CONST_STRING_INIT     0x07
#define FUNCTION              0x08
#define STREAM                0x09

#define ELEMENT_INT           0x0a
#define ELEMENT_CONST         0x0c
#define ELEMENT_PTR           0x0d
#define ELEMENT_ELEMENT       0x0d
#define ELEMENT_CONST_STRING  0x0e

#define INT_INT               0x0f
#define INT_CONST             0x11
#define INT_PTR               0x12
#define INT_ELEMENT           0x13
#define INT_CONST_STRING      0x14

#define PTR_INT               0x1b
#define PTR_PTR               0x1d
#define PTR_CONST             0x1e
#define PTR_ELEMENT           0x1f
#define PTR_CONST_STRING      0x20

#define LOOP                  0x21
#define IF                    0x22

#define IZ                    0x01
#define INZ                   0x02
#define IGT                   0x03
#define ILT                   0x04
#define IGE                   0x05
#define ILE                   0x06
#define IE                    0x07
#define INE                   0x08

#define ASSIGNMENT            0x23

#define INC                   0x24
#define DEC                   0x25

#define MUL                   0x26
#define DIV                   0x27
#define MOD                   0x28
#define ADD                   0x29
#define SUB                   0x2a

#define FMUL                  0x2b
#define FDIV                  0x2c
#define FADD                  0x2d
#define FSUB                  0x2e

#define SHR                   0x2f
#define SHL                   0x30

#define AND                   0x31
#define OR                    0x32
#define NOT                   0x33
#define XOR                   0x34

#define CALL                  0x35
#define CALL_PTR_FUNCTION     0x36

#define END                   0x37

#define BREAK                 0x38
#define CONTINUE              0x39

#define UNDEFINED             0x3a
//#define INTEGER  0x3c

#define ALLOC                 0x3b

#define POINTERS_ALLOC        0x3d
#define ELEMENT_ALLOC         0x3e

#define PRINT                 0x3f

#define RETURN                0x40

//типы, хранимы в дереве
#define PTRS                  0x01
#define ELEMENT               0x03
#define INTEGER               0x04
#define CONST                 0x05
#define CONST_STRING          0x06

#define REAL                  0x07

#define ELEMENT_CONST_SIZE    0x08
#define PTRS_CONST_SIZE       0x09

//возвращаемые значения функций
#define FUNCTION_OTHER        0x01
#define FUNCTION_PTRS         0x02
#define FUNCTION_ELEMENT      0x03
#define FUNCTION_NUMBER       0x04
#define FUNCTION_CONST        0x05
#define FUNCTION_CONST_STRING 0x06

typedef struct
{
    char  type,
         *data;
}Data;

typedef struct
{
    String *name;
    char   *data;
    int     type;
}Type;

typedef struct
{
    char    type;
    char    uninitialized;
    char    is_closure;
    char   *data;
    String *name;
    int     count;
}Number;

typedef struct
{
    char    uninitialized;
    String *name;
    Data   *data;
    int     ilength;
    int     count;
}Pointers;

typedef struct
{
    char    uninitialized;
    String *name;
    char   *data;
    int     isz;
    int     count;
}Element;

typedef struct
{
    char    uninitialized;
    String *name;
    int     data;
}Const;

typedef struct
{
    char    uninitialized;
    String *name;
    char   *data;
    int     isz;
}ConstString;

typedef struct
{
    String *name;
    Tree   *types,
           *functions;
    int     length_args;
    List   *args;
    List   *body;
    Stack  *pos;
}Function;

typedef struct
{
    Element *el;
    Number  *in,
            *index;
}ElementVar;

typedef struct
{
    Element *el;
    Const   *in;
    Number  *index;
}ElementConst;

typedef struct
{
    Element     *el;
    ConstString *in;
}ElementConstString;

typedef struct
{
    Element  *el;
    Number   *index;
    Pointers *ptrs;
}ElementPtr;

typedef struct
{
    Element *el,
            *in;
}ElementElement;

typedef struct
{
    Number  *var,
            *index;
    Element *in;
}VarElement;

typedef struct
{
    Number *var,
           *in;
}VarVar;

typedef struct
{
    Number *var;
    Const  *in;
}VarConst;

typedef struct
{
    Number      *var;
    ConstString *in;
}VarConstString;

typedef struct
{
    Number   *var,
             *index;
    Pointers *ptrs;
}VarPtr;


typedef struct
{
    Element  *in;
    Number   *index;
    Pointers *ptrs ;
}PointerElement;

typedef struct
{
    Number   *var,
             *index;
    Pointers *ptrs;
}PointerVar;

typedef struct
{
    Pointers *ptrs;
    Number   *index;
    Const    *in;
}PointerConst;

typedef struct
{
    Pointers    *ptrs;
    Number      *index;
    ConstString *in;
}PointerConstString;

typedef struct
{
    Pointers *ptrs_in,
             *ptrs;
    Number   *index_in,
             *index;
}PointerPtr;


typedef struct
{
    List *eval;
}Loop;

typedef struct
{
    struct ListNode* loop;
}Break;

typedef struct
{
    struct ListNode* loop;
}Continue;

typedef struct
{
    List *cond;
    List *eval;
}If;

typedef struct
{
    Number *var;
}Increment;

typedef struct
{
    Number *var;
}Decrement;

typedef struct
{
    Function *fun;
    List     *args;
}Call;

typedef struct
{
    Number *var1,
           *var2,
           *var_rez;
}Add;

typedef struct
{
    Number *var1,
           *var2,
           *var_rez;
}Sub;

typedef struct
{
    Number *var1,
           *var2,
           *var_rez;
}Mul;

typedef struct
{
    Number *var1,
           *var2,
           *var_rez;
}Div;

typedef struct
{
    Number *var1,
           *var2,
           *var_rez;
}And;

typedef struct
{
    Number *var1,
           *var2,
           *var_rez;
}Or;

typedef struct
{
    Number *var,
           *var_rez;
}Not;

typedef struct
{
    Number *var1,
           *var2,
           *var_rez;
}Xor;

typedef struct
{
    Number *var_rez,
           *var,
           *shift;
}Shr;

typedef struct
{
    Number *var_rez,
           *var,
           *shift;
}Shl;

typedef struct
{
    Element *el;
    Number  *sz;
}ElementAlloc;

typedef struct
{
    Pointers *ptrs;
    Number   *length;
}PointersAlloc;

typedef struct
{
    Type *data;
}Print;

typedef struct
{
    Number *var;
}IfZero;

typedef struct
{
    Number *var;
}IfNotZero;

typedef struct
{
    Number *var_l,
           *var_r;
}IfEqual;

typedef struct
{
    Number *var_l,
           *var_r;
}IfNotEqual;

typedef struct
{
    Number *var_l,
           *var_r;
}IfLesserTnan;

typedef struct
{
    Number *var_l,
           *var_r;
}IfGreatherTnan;

typedef struct
{
    Number *var_l,
           *var_r;
}IfLesserTnanOrEqual;

typedef struct
{
    Number *var_l,
           *var_r;
}IfGreatherTnanOrEqual;

typedef struct
{
    Type *left_operand;
    List *expression;
}Assignment;

extern Id *id;

Type*     get_parser_op_all   (String *s, Function *cur_function);
char      tree_contains       (Function *cur_function, String *name);
Type*     find_type           (Tree *types, String *name);
Function* find_global_function(Function *cur_function, Tree *tree_cur_function, Stack *functions, String *s);
Function* find_function       (Tree *tree_cur_function, String *s);
Type*     find_global_type    (Function *cur_function, Stack *functions, String *s);

Tree*   parse     (String *s);
String* next_token(String *s);

void add_type(Tree *types, char *data, String *name_data, int type);
Data *new_data(char type, char *data);

void new_run_data();

#endif // PARSER_H_INCLUDED
