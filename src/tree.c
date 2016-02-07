#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

const char ERROR=1,
           OK=0;

Tree* tree_init()
{
    Tree *tree=malloc(sizeof(Tree));
    tree->root=0;

    return tree;
}

static unsigned char height(TreeNode *p)
{
    if(p)
        return p->height;
    return 0;
}

static int bfactor(TreeNode *p)
{
    return height(p->right)-height(p->left);
}

static void fixheight(TreeNode *p)
{
    unsigned char hl=height(p->left);
    unsigned char hr=height(p->right);

    if(hl>hr)
        p->height=hl+1;
    else
        p->height=hr+1;
}

static TreeNode* rotateright(TreeNode *p)
{
    TreeNode *q=p->left;
    p->left=q->right;
    q->right=p;
    fixheight(p);
    fixheight(q);

    return q;
}

static TreeNode* rotateleft(TreeNode *q)
{
    TreeNode *p=q->right;
    q->right=p->left;
    p->left=q;
    fixheight(q);
    fixheight(p);

    return p;
}

static TreeNode* balance(TreeNode *p)
{
    fixheight(p);
    if(bfactor(p)==2)
    {
        if(bfactor(p->right)<0)
            p->right=rotateright((p->right));
        return rotateleft(p);
    }

    if(bfactor(p)==-2)
    {
        if(bfactor(p->left)>0)
            p->left=rotateleft(p->left);
        return rotateright(p);
    }

    return p;
}

static char *tree_add_data;
static char (*tree_comparision)(char *, char *);

static TreeNode *tree_add_node(TreeNode *node)
{
    if(node==0)
    {
        node=malloc(sizeof(TreeNode));
        node->data=tree_add_data;
        node->left=0;
        node->right=0;
        node->height=1;
    }
    else
    {
        int cmp=tree_comparision(tree_add_data, node->data);

        if(cmp<0)
            node->left=tree_add_node(node->left);
        else
            node->right=tree_add_node(node->right);
    }

    return balance(node);
}

void tree_add(Tree *tree, char *data, char(*comparision)(char *, char *))
{
    tree_add_data=data;
    tree_comparision=comparision;
    tree->root=tree_add_node(tree->root);
}

static void (*tree_print_func)(char *data);

static void tree_print_node(TreeNode *node)
{
    int i;

    if(node!=0)
    {
        tree_print_node(node->left);
        tree_print_func(node->data);
        tree_print_node(node->right);
    }
}

void tree_print(Tree *tree, void (*print_data)(char *data))
{
    tree_print_func=print_data;
    tree_print_node(tree->root);
}

static void (*tree_free_func)(char *data);

static void tree_free_node(TreeNode *node)
{
    if(node!=0)
    {
        tree_free_func(node->data);
        tree_free_node(node->left);
        tree_free_node(node->right);
        free(node);
    }
}

void tree_free(Tree *tree, void(*free_data)(char *data))
{
    if(tree->root==0)
    {

    }

    tree_free_func=free_data;
    tree_free_node(tree->root);
    free(tree);
}
