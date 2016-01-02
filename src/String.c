#include "String.h"
#include <stdlib.h>

void str_push(String *s, char data)
{
    struct StringNode *alloc=malloc(sizeof(struct StringNode));

    alloc->data=data;
    alloc->next=0;
    alloc->previous=s->end;

    if(s->begin==0)
    {
        s->begin=s->end=alloc;
        s->length=1;
        return;
    }

    s->end->next=alloc;
    s->end=alloc;
    s->length++;
}

void str_push_back(String *s, char data)
{
    struct StringNode *alloc=malloc(sizeof(struct StringNode));

    alloc->data=data;
    alloc->next=s->begin;
    alloc->previous=0;

    if(s->begin==0)
    {
        s->begin=s->end=alloc;
        s->length=1;
        return;
    }

    s->begin->previous=alloc;
    s->begin=alloc;
    s->length++;
}

char str_pop(String *s)
{
    char ret=s->end->data;
    struct StringNode *del=s->end;

    if(s->length==1)
    {
        s->begin=s->end=0;
        s->length=0;
        free(del);
        return ret;
    }

    s->end=s->end->previous;
    s->end->next=0;
    s->length--;
    free(del);

    return ret;
}

char str_pop_back(String *s)
{
    char ret=s->begin->data;
    struct StringNode *del=s->begin;

    if(s->length==1)
    {
        s->begin=s->end=0;
        s->length=0;
        free(del);
        return ret;
    }

    s->begin=s->begin->next;
    s->begin->previous=0;
    s->length--;
    free(del);

    return ret;
}

String *str_init(const char *c)
{
    int i=1;
    String *s=malloc(sizeof(String));
    struct StringNode *count;

    s->begin=0;
    s->end=0;
    s->length=0;

    if(c[0])
    {
        count=malloc(sizeof(struct StringNode));
        count->data=c[0];
        count->next=0;
        count->previous=0;
        s->begin=count;
        s->end=count;
        s->length=1;
    }
    else return s;

    while(c[i])
    {
        count->next=malloc(sizeof(struct StringNode));
        count->next->data=c[i];
        count->next->previous=count;
        count=count->next;

        i++;
    }
    s->length+=i-1;
    s->end=count;

    if(s->begin)
        count->next=0;

    return s;
}

void str_clear(String *s)
{
    struct StringNode *count=s->begin, *tmp;

    while(count)
    {
        tmp=count->next;
        free(count);
        count=tmp;
    }

    s->begin=0;
    s->end=0;
    s->length=0;
}

void str_copy(String *str1, String *str2)
{
    struct StringNode *count=str2->begin, *i=0;

    if(str1==str2)
        return;
    str_clear(str1);
    str1->length=str2->length;

    if(count)
    {
        i=malloc(sizeof(struct StringNode));
        i->data=count->data;
        i->next=0;
        i->previous=0;
        str1->begin=i;
        str1->end=i;
        count=count->next;
    }

    while(count)
    {
        i->next=malloc(sizeof(struct StringNode));
        i->next->previous=i;
        i->next->data=count->data;
        i=i->next;
        str1->end=count;

        count=count->next;
    }

    if(i)
        i->next=0;
}

void str_concat(String *s1, String *s2)
{
    struct StringNode *i=s1->begin, *count=s2->begin;
    int j, k;

    s1->length+=s2->length;

    if(s1!=s2)
    {
        if(i)
        {
            while(i->next)
                i=i->next;
        }
        else
            str_copy(s1, s2);

        while(count)
        {
            i->next=malloc(sizeof(struct StringNode));
            i->next->previous=i;
            i->next->data=count->data;
            i=i->next;
            s1->end=i;

            count=count->next;
        }

        if(i)
            i->next=0;
    }
    else if(i)
    {
        k=0;
        while(i->next)
        {
            i=i->next;
            k++;
        }

        for(j=0; j<=k; j++)
        {
            i->next=malloc(sizeof(struct StringNode));
            i->next->previous=i;
            i->next->data=count->data;
            i=i->next;
            s1->end=i;

            count=count->next;
        }

        i->next=0;
    }
}

void str_free(String *s)
{
    struct StringNode *count=s->begin, *tmp;

    while(count)
    {
        tmp=count->next;
        free(count);
        count=tmp;
    }

    free(s);
}

void str_print(String *s)
{
    struct StringNode *i=s->begin;

    while(i)
    {
        printf("%c", i->data);
        i=i->next;
    }
}

void str_print2(String *s)
{
    struct StringNode *i=s->end;

    while(i)
    {
        printf("%c", i->data);
        i=i->previous;
    }
}

int str_comparision(String *s1, String *s2)
{
    struct StringNode *i=s1->begin, *j=s2->begin;

    while(i && j)
    {
        if(i->data<j->data)
            return 1;
        else if(i->data>j->data)
            return -1;

        i=i->next;
        j=j->next;
    }

    if(i==0)
    {
        if(j!=0)
        {
            return 1;
        }
        else return 0;
    }
    else if(j==0)
    {
        if(i!=0)
        {
            return -1;
        }
        else return 0;
    }
}

int str_to_int(String *s)
{
    int ret=0;
    struct StringNode *i=s->end;
    int pow=1;

    while(i)
    {
        ret+=(i->data-'0')*pow;
        pow*=10;
        i=i->previous;
    }

    return ret;
}
