#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include "Id.h"
#include "parser.h"

typedef struct
{
    unsigned int index;
    String *name_element;
    Type *element;
}PtrsMapping;

extern int size_data, line;
extern Id *id_const;

char is_number(char c);
char is_symbol(char c);

String *lexer(char *name);
void skip();

PtrsMapping *new_ptrs_map_element(String *name, Type *data, unsigned int index);
Type *new_tree_data(char *name, char *data, char type);

#endif // LEXER_H_INCLUDED
