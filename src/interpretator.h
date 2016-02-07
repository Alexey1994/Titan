#ifndef INTERPRETATOR_H_INCLUDED
#define INTERPRETATOR_H_INCLUDED

#include "List.h"
#include "tree.h"

#define STACK_LENGTH 1000

void pure_run(Tree *fun);
void run(Tree *fun);
void interpretator_table_init();

#endif // INTERPRETATOR_H_INCLUDED
