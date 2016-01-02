#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

struct ListNode
{
  char *data;
  struct ListNode *next, *previouse;
};

typedef struct
{
    struct ListNode *begin, *end;
    int length;
}List;

List* list_init();
void list_clear(List *list);
void list_free(List *list);

void list_delete(List *list, int position);
void list_insert(List *list, char *data, int position);

void list_push(List *list, char *data);
char *list_pop(List *list);

char* list_get(List *list, int position);

#endif
