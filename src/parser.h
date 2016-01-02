#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "stack.h"
#include "List.h"
#include "tree.h"
#include "Id.h"
#include "String.h"

#define BITS 4

/*комманды ассемблера*/
#define PTRS_INIT 0x01
#define ARRAY_INIT 0x02
#define ELEMENT_INIT 0x03
#define VAR_INIT 0x04
#define CONST_INIT 0x05
#define CONST_STRING_INIT 0x51
#define FUNCTION 0x06
#define STREAM 0x07

#define ELEMENT_VAR 0x0c
#define ELEMENT_ARRAY 0x0d
#define ELEMENT_CONST 0x0e
#define ELEMENT_PTR     0x0f
#define ELEMENT_ELEMENT 0x10
#define ELEMENT_CONST_STRING 0x52

#define VAR_VAR 0x11
#define VAR_ARRAY 0x12
#define VAR_CONST 0x13
#define VAR_PTR     0x14
#define VAR_ELEMENT 0x15
#define VAR_CONST_STRING 0x53

#define ARRAY_VAR 0x16
#define ARRAY_ARRAY 0x17
#define ARRAY_PTR   0x18
#define ARRAY_CONST 0x19
#define ARRAY_ELEMENT 0x1a
#define ARRAY_CONST_STRING 0x54

#define PTR_VAR 0x20
#define PTR_ARRAY 0x21
#define PTR_PTR   0x22
#define PTR_CONST 0x23
#define PTR_ELEMENT 0x24
#define PTR_CONST_STRING 0x55

#define LOOP       0x26
#define IF         0x27

#define ASSIGNMENT 0x28

#define INC 0x2a
#define DEC 0x2b

#define MUL 0x2c
#define DIV 0x2d
#define MOD 0x2e
#define ADD 0x2e
#define SUB 0x2f

#define SHR 0x30
#define SHL 0x31

#define AND 0x32
#define OR 0x33
#define NOT 0x34
#define XOR 0x35

#define CALL 0x36
#define CALL_PTR_FUNCTION 0x37

#define END 0x38

#define BREAK 0x39
#define CONTINUE 0x3a

#define UNDEFINED 0x3b
#define VARIABLE  0x3c

#define ALLOC 0x3d

#define ARRAY_ALLOC 0x3e
#define POINTERS_ALLOC 0x3f
#define ELEMENT_ALLOC 0x46

#define EQ  0x40 // ==
#define NEQ 0x41 // !=
#define GT  0x42 // >
#define LT  0x43 // <
#define GE  0x44 // >=
#define LE  0x45 // <=

#define PUTC 0x7f

//типы, хранимы в дереве
#define PTRS  0x01
#define ARRAY 0x02
#define ELEMENT 0x03
#define VAR 0x04
#define CONST 0x05
#define CONST_STRING 0x06

typedef enum
{
    INTEGER=0x01,
    FLOAT=0x02
}VariableType;

typedef struct
{
    VariableType type;
    char uninitialized;
    int data;
    String *name;
    int count;
}Variable;

typedef struct
{
    char type;
    char *data;
}PointerData;

typedef struct
{
    char uninitialized;
    String *name;
    PointerData *data;
    int ilength;
    int count;
}Pointers;

typedef struct
{
    char uninitialized;
    String *name;
    char *data;
    int isz, ilength;
    int count;
}Array;

typedef struct
{
    char uninitialized;
    String *name;
    char *data;
    int isz;
    int count;
}Element;

typedef struct
{
    char uninitialized;
    String *name;
    int data;
}Const;

typedef struct
{
    char uninitialized;
    String *name;
    char *data;
    int isz;
}ConstString;

typedef struct
{
    String *name;
    Tree *types, *functions;
    List *body;
    Stack *pos;
}Function;

typedef struct
{
    Element *el;
    Variable *in, *index;
}ElementVar;

typedef struct
{
    Element *el;
    Const *in;
    Variable *index;
}ElementConst;

typedef struct
{
    Element *el;
    ConstString *in;
}ElementConstString;

typedef struct
{
    Element *el;
    Variable *index;
    Array *arr;
}ElementArray;

typedef struct
{
    Element *el;
    Variable *index;
    Pointers *ptrs;
}ElementPtr;

typedef struct
{
    Element *el, *in;
}ElementElement;

typedef struct
{
    Variable *var, *index;
    Element *in;
}VarElement;

typedef struct
{
    Variable *var, *in;
}VarVar;

typedef struct
{
    Variable *var;
    Const *in;
}VarConst;

typedef struct
{
    Variable *var;
    ConstString *in;
}VarConstString;

typedef struct
{
    Variable *var, *index;
    Array *arr;
}VarArray;

typedef struct
{
    Variable *var, *index;
    Pointers *ptrs;
}VarPtr;


typedef struct
{
    Element *in;
    Variable *index;
    Array *arr;
}ArrayElement;

typedef struct
{
    Variable *var, *index;
    Array *arr;
}ArrayVar;

typedef struct
{
    Array *arr;
    Variable *index;
    Const *in;
}ArrayConst;

typedef struct
{
    Array *arr;
    Variable *index;
    ConstString *in;
}ArrayConstString;

typedef struct
{
    Variable *index_in, *index;
    Array *arr_in, *arr;
}ArrayArray;

typedef struct
{
    Array *arr;
    Variable *index;
    Pointers *ptrs;
}ArrayPtr;


typedef struct
{
    Element *in;
    Variable *index;
    Pointers *ptrs;
}PointerElement;

typedef struct
{
    Variable *var, *index;
    Pointers *ptrs;
}PointerVar;

typedef struct
{
    Pointers *ptrs;
    Variable *index;
    Const *in;
}PointerConst;

typedef struct
{
    Pointers *ptrs;
    Variable *index;
    ConstString *in;
}PointerConstString;

typedef struct
{
    Variable *index;
    Array *arr;
    Pointers *ptrs;
}PointerArray;

typedef struct
{
    Pointers *ptrs_in, *ptrs;
    Variable *index_in, *index;
}PointerPtr;


typedef struct
{
    String *name;
    char *data;
    int type;
}Type;

typedef struct
{
    char type;
    char *data;
}RunData;

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
    Variable *cond;
    List *eval;
}If;

typedef struct
{
    Variable *var;
}Increment;

typedef struct
{
    Variable *var;
}Decrement;

typedef struct
{
    Function *fun;
}Call;

typedef struct
{
    Variable *var1, *var2, *var_rez;
}Add;

typedef struct
{
    Variable *var1, *var2, *var_rez;
}Sub;

typedef struct
{
    Variable *var1, *var2, *var_rez;
}Mul;

typedef struct
{
    Variable *var1, *var2, *var_rez;
}Div;

typedef struct
{
    Variable *var1, *var2, *var_rez;
}And;

typedef struct
{
    Variable *var1, *var2, *var_rez;
}Or;

typedef struct
{
    Variable *var, *var_rez;
}Not;

typedef struct
{
    Variable *var1, *var2, *var_rez;
}Xor;

typedef struct
{
    Variable *var_rez, *var, *shift;
}Shr;

typedef struct
{
    Variable *var_rez, *var, *shift;
}Shl;


typedef struct
{
    Variable *left, *right, *var;
}Equal;

typedef struct
{
    Variable *left, *right, *var;
}NotEqual;

typedef struct
{
    Variable *left, *right, *var;
}GreatherThan;

typedef struct
{
    Variable *left, *right, *var;
}LesserThan;

typedef struct
{
    Variable *left, *right, *var;
}GreatherThanOrEqual;

typedef struct
{
    Variable *left, *right, *var;
}LesserThanOrEqual;

typedef struct
{
    Element *el;
    Variable *sz;
}ElementAlloc;

typedef struct
{
    Array *arr;
    Variable *sz, *length;
}ArrayAlloc;

typedef struct
{
    Pointers *ptrs;
    Variable *length;
}PointersAlloc;

typedef struct
{
    Type *data;
}Putc;

extern Id *id;

char tree_contains(Function *cur_function, String *name);
Type *find_type(Tree *types, String *name);
Function *find_global_function(Tree *tree_cur_function, Stack *functions, String *s);
Function* find_function(Tree *tree_cur_function, String *s);
Type *find_global_type(Function *cur_function, Stack *functions, String *s);

Tree *parse(String *s);
String *next_token(String *s);

#endif // PARSER_H_INCLUDED
