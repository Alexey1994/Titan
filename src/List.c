#include "List.h"
#include <stdlib.h>

List* list_init()
{
    List *list=malloc(sizeof(List));
    list->begin=0;
    list->end=0;
    list->length=0;

    return list;
}

char* list_get(List *list, int pos)
{
    struct ListNode *cur=list->begin;
    int i;

    for(i=0; i<pos; i++)
        cur=cur->next;

    return cur->data;
}

void list_insert(List *list, char *data, int pos)
{
    int i;
    struct ListNode *cur=list->begin, *ins;

    ins=malloc(sizeof(struct ListNode));
    ins->data=data;

    if(pos==0)
    {
        if(list->begin)
            list->begin->previouse=ins;
        else
            list->end=ins;


        ins->next=list->begin;
        ins->previouse=0;
        list->begin=ins;
        list->length++;

        return;
    }

    if(pos==list->length)
    {
        list->end->next=ins;
        ins->next=0;
        ins->previouse=list->end;
        list->end=ins;
        list->length++;

        return;
    }

    for(i=0; i<pos-1; i++)
        cur=cur->next;

    ins->next=cur->next;
    ins->previouse=cur;
    cur->next=ins;
    list->length++;

    return;
}

void list_push(List *list, char *data)
{
    struct ListNode *ins=malloc(sizeof(struct ListNode));

    ins->data=data;
    ins->next=0;
    ins->previouse=list->end;
    if(list->end)
        list->end->next=ins;
    else
        list->begin=ins;
    list->end=ins;

    list->length++;
}

char *list_pop(List *list)
{
    struct ListNode *del=list->end;
    char *data=list->end->data;

    if(list->end==list->begin)
        list->begin=0;

    list->end=list->end->previouse;
    list->end->next=0;

    free(del);

    return data;
}

void list_delete(List *list, int position)
{
    struct ListNode *del, *cur=list->begin;
    int i;

    if(position==0)
    {
        if(list->begin==list->end)
            list->end=0;
        list->begin=cur->next;
        list->begin->previouse=0;

        free(cur->data);
        free(cur);
        list->length--;

        return;
    }

    if(position==list->length-1)
    {
        del=list->end;
        list->end=del->previouse;
        list->end->next=0;

        free(del->data);
        free(del);
        list->length--;

        return;
    }

    for(i=0; i<position-1; i++)
        cur=cur->next;

    del=cur->next;
    cur->next=del->next;
    cur->previouse=del->previouse;
    free(del->data);
    free(del);

    list->length--;

    return;
}

void list_clear(List *list, void (*free_data)(char *data))
{
    struct ListNode *i=list->begin, *del;

    while(i)
    {
        del=i;
        i=i->next;
        free_data(del->data);
        free(del);
    }

    list->length=0;
    list->begin=0;
    list->end=0;
}

void list_free(List *list, void (*free_data)(char *data))
{
    struct ListNode *i=list->begin, *del;

    while(i)
    {
        del=i;
        i=i->next;
        free_data(del->data);
        free(del);
    }

    free(list);
}
