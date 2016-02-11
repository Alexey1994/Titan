#include "parser_cmp.h"
#include "parser.h"

int tree_type_cmp(char *s1, char *s2)
{
    Type *t1=(Type*)s1, *t2=(Type*)s2;
    return str_comparision(t1->name, t2->name);
}

int function_cmp(char *s1, char *s2)
{
    Function *t1=(Function*)s1, *t2=(Function*)s2;
    return str_comparision(t1->name, t2->name);
}

int element_cmp(char *s1, char *s2)
{
    Element *t1=(Element*)s1, *t2=(Element*)s2;
    return str_comparision(t1->name, t2->name);
}
