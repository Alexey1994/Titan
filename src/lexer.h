#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include "Id.h"
#include "parser.h"

typedef struct
{
    char *name;
    unsigned int index;
    Type *data;
}PtrsData;

extern int size_data, line;
extern Id *id_const;

char is_number(char c);
char is_symbol(char c);

String *lexer(char *name);
void skip();

PtrsData *new_tree_ptrs(char *name, Type *data,int index);
Type *new_tree_data(char *name, char *data, char type);

#endif // LEXER_H_INCLUDED
