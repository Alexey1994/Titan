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
void list_clear(List *list, void (*free_data)(char *data));
void list_free(List *list, void (*free_data)(char *data));

void list_print(List *list, void (*print_data)(char *data));

void list_delete(List *list, int position);
void list_insert(List *list, char *data, int position);

void list_push(List *list, char *data);
void list_push_front(List *list, char *data);
char *list_pop(List *list);
char *list_queue_pop(List *list);

char* list_get(List *list, int position);

#endif
