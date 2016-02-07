#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

typedef struct
{
    char *data;
    struct TreeNode *left, *right;
    unsigned char height;
}TreeNode;

typedef struct
{
    TreeNode *root;
}Tree;

void tree_add(Tree *tree, char *data, char(*comparision)(char *, char *));
void tree_print(Tree *tree, void (*print_data)(char *data));
Tree* tree_init();
void tree_free(Tree *tree, void(*free_data)(char *data));

#endif // TREE_H_INCLUDED
