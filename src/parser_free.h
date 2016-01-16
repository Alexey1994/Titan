#ifndef PARSER_FREE_H_INCLUDED
#define PARSER_FREE_H_INCLUDED

#include "tree.h"
#include "parser.h"

void parser_free_functions(Tree *f);
void parser_free_function(Function *f);
void parser_free_type(Type *t);

#endif // PARSER_FREE_H_INCLUDED
