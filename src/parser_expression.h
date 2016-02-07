#ifndef PARSER_EXPRESSION_H_INCLUDED
#define PARSER_EXPRESSION_H_INCLUDED

#include "parser.h"
#include "List.h"
#include "string.h"

List *get_parser_expression(String *in, Function *cur_function);

#endif // PARSER_EXPRESSION_H_INCLUDED
