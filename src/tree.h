#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

typedef struct
{
    char *data;
    struct Tree *left, *right;
}TreeNode;

typedef struct
{
    TreeNode *root;
    int length;
}Tree;

char tree_add(Tree *tree, char *data, int(*comparision)(char *, char *));
void tree_print(Tree *tree, void(*tree_print_data)(char *data));
Tree* tree_init();
void tree_free(Tree *tree, void(*free_data)(char *data));

#endif // TREE_H_INCLUDED
