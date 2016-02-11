#ifndef INTERPRETATOR_COMPILER_H_INCLUDED
#define INTERPRETATOR_COMPILER_H_INCLUDED

#include "../parser/parser.h"
#include "../List.h"

#define JMP 0x78

#define JZ 0x40
#define JNZ 0x41
#define JE 0x42
#define JNE 0x43
#define JLT 0x44
#define JGT 0x45
#define JLE 0x46
#define JGE 0x47

typedef struct
{
    struct ListNode *place;
}Jmp;

typedef struct
{
    Number *var;
    struct ListNode *place;
}Jz;

typedef struct
{
    Number *var;
    struct ListNode *place;
}Jnz;

typedef struct
{
    Number *var_l, *var_r;
    struct ListNode *place;
}Je;

typedef struct
{
    Number *var_l, *var_r;
    struct ListNode *place;
}Jne;

typedef struct
{
    Number *var_l, *var_r;
    struct ListNode *place;
}Jlt;

typedef struct
{
    Number *var_l, *var_r;
    struct ListNode *place;
}Jgt;

typedef struct
{
    Number *var_l, *var_r;
    struct ListNode *place;
}Jle;

typedef struct
{
    Number *var_l, *var_r;
    struct ListNode *place;
}Jge;

List *get_fast_run(Function *main);

#endif // INTERPRETATOR_COMPILER_H_INCLUDED
