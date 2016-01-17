#ifndef INTERPRETATOR_H_INCLUDED
#define INTERPRETATOR_H_INCLUDED

#include "tree.h"

#define STACK_LENGTH 1000

void run(Tree *fun);
void interpretator_table_init();

#endif // INTERPRETATOR_H_INCLUDED
