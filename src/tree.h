#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

typedef struct
{
    char *data;
    struct Tree *left, *right;
}Tree;

char tree_add(Tree *node, char *data, int(*comparision)(char *, char *));
void tree_print(Tree *node);
Tree* tree_init(char *root);
void tree_free(Tree *node);

#endif // TREE_H_INCLUDED
