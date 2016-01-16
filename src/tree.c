#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

const char ERROR=1,
           OK=0;

Tree* tree_init()
{
    Tree *tree=malloc(sizeof(Tree));
    tree->root=0;
    tree->length=0;

    return tree;
}

char tree_add(Tree *tree, char *data, int(*comparision)(char *, char *))
{
    int cmp;
    TreeNode *i=tree->root, *alloc;

    while(i)
    {
        cmp=comparision(data, i->data);
        if(cmp==0)
        {
            return ERROR;
        }
        else if(cmp<0)
        {
            if(i->left)
                i=(TreeNode*)i->left;
            else
            {
                alloc=(TreeNode*)malloc(sizeof(TreeNode));
                alloc->data=data;
                alloc->left=alloc->right=0;
                i->left=(TreeNode*)alloc;
                tree->length++;
                return OK;
            }
        }
        else
        {
            if(i->right)
                i=(TreeNode*)i->right;
            else
            {
                alloc=(TreeNode*)malloc(sizeof(TreeNode));
                alloc->data=data;
                alloc->left=alloc->right=0;
                i->right=(TreeNode*)alloc;
                tree->length++;
                return OK;
            }
        }
    }

    alloc=(TreeNode*)malloc(sizeof(TreeNode));
    alloc->data=data;
    alloc->left=alloc->right=0;
    tree->root=(TreeNode*)alloc;
    tree->length++;

    return OK;
}

static void(*tree_node_print_data)(char *data);

static void tree_print_nodes(TreeNode *root)
{
    if(root)
    {
        tree_print_nodes((TreeNode*)root->left);
        tree_node_print_data(root->data);
        tree_print_nodes((TreeNode*)root->right);
    }
}

void tree_print(Tree *tree, void(*tree_print_data)(char *data))
{
    tree_node_print_data=tree_print_data;
    tree_print_nodes(tree->root);
}

static void(*tree_node_free_data)(char *data);

static void tree_free_nodes(TreeNode *root)
{
    if(root)
    {
        tree_node_free_data(root->data);
        tree_free_nodes((TreeNode*)root->left);
        tree_free_nodes((TreeNode*)root->right);
        free(root);
    }
}

void tree_free(Tree *tree, void(*free_data)(char *data))
{
    tree_node_free_data=free_data;
    tree_free_nodes(tree->root);
    free(tree);
}
