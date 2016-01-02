#ifndef STRINSSS_H_INCLUDED
#define STRINSSS_H_INCLUDED

struct StringNode
{
    char data;
    struct StringNode *next, *previous;
};

typedef struct
{
    struct StringNode *begin, *end;
    int length;
}String;

void str_concat(String *s1, String *s2);
void str_push(String *s, char data);
void str_push_back(String *s, char data);
char str_pop(String *s);
char str_pop_back(String *s);
void str_copy(String *str1, String *str2);
void str_clear(String *s);
String *str_init(const char *c);
void str_free(String *s);

void str_print(String *s);
void str_print2(String *s);
int str_comparision(String *s1, String *s2);

int str_to_int(String *s);

#endif // STRINSSS_H_INCLUDED
