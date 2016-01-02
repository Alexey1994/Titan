#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

const char ERROR=1,
           OK=0;

Tree* tree_init(char *root)
{
    Tree *t=malloc(sizeof(Tree));
    t->data=root;
    t->left=0;
    t->right=0;

    return t;
}

char tree_add(Tree *node, char *data, int(*comparision)(char *, char *))
{
    int cmp;
    Tree *i=node, *alloc;

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
                i=(Tree*)i->left;
            else
            {
                alloc=(Tree*)malloc(sizeof(Tree));
                alloc->data=data;
                alloc->left=alloc->right=0;
                i->left=(struct Tree*)alloc;
                return OK;
            }
        }
        else
        {
            if(i->right)
                i=(Tree*)i->right;
            else
            {
                alloc=(Tree*)malloc(sizeof(Tree));
                alloc->data=data;
                alloc->left=alloc->right=0;
                i->right=(struct Tree*)alloc;
                return OK;
            }
        }
    }
}

void tree_print(Tree *node)
{
    if(node!=0)
    {
        tree_print((Tree*)node->left);
        printf(node->data);printf("\n");
        tree_print((Tree*)node->right);
    }
}

void tree_free(Tree *node)
{
    if(node!=0)
    {
        tree_free((Tree*)node->left);
        tree_free((Tree*)node->right);
        free(node);
    }
}
