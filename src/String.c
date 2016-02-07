#include "String.h"
#include <stdlib.h>

static void *str_realloc(String *s)
{
    char *buf_tmp=malloc(s->reserve_count_begin + s->reserve_count_end), *del;

    char *i=buf_tmp+s->reserve_count_begin,
         *j=s->begin;

    s->begin=i;
    del=j-s->reserve_count_begin+s->length_begin;

    int k;
    for(k=0; k<s->length-1; k++)
    {
        *i=*j;
        i++;
        j++;
    }
    s->end=i;
    *s->end='\0';
    free(del);
}

String *str_init(const char *c)
{
    String *s=malloc(sizeof(String));
    char *i;

    s->begin=malloc(STRING_BUF_RESERVE+STRING_BUF_RESERVE);
    s->begin+=STRING_BUF_RESERVE;

    s->reserve_count_end=STRING_BUF_RESERVE;
    s->reserve_count_begin=STRING_BUF_RESERVE;
    s->length=0;
    s->length_begin=0;

    i=s->begin;
    while(*c)
    {
        s->length++;
        if(s->length==s->reserve_count_end)
        {
            *i='\0';
            s->reserve_count_end+=STRING_BUF_RESERVE;
            str_realloc(s);
            i=s->end;
        }

        *i=*c;
        i++;
        c++;
    }
    *i='\0';
    s->end=i;

    return s;
}

void str_push(String *s, char data)
{
    s->length++;
    if(s->reserve_count_end-s->length_begin==s->length)
    {
        s->reserve_count_end+=STRING_BUF_RESERVE;
        str_realloc(s);
    }

    *s->end=data;
    s->end++;
    *s->end='\0';
}

void str_push_back(String *s, char data)
{
    s->length_begin++;
    s->length++;
    if(s->reserve_count_end-s->length_begin==s->length)
    {
        s->reserve_count_begin+=STRING_BUF_RESERVE;
        str_realloc(s);
    }

    s->begin--;
    *s->begin=data;
}

char str_pop(String *s)
{
    char ret;

    s->end--;
    ret=*s->end;
    *s->end='\0';
    s->length--;

    return ret;
}

char str_pop_back(String *s)
{
    char ret=*s->begin;

    s->begin++;
    s->length_begin--;
    s->length--;

    return ret;
}

void str_clear(String *s)
{
    free(s->begin-s->reserve_count_begin+s->length_begin);

    s->begin=malloc(STRING_BUF_RESERVE+STRING_BUF_RESERVE);
    s->begin+=STRING_BUF_RESERVE;

    s->reserve_count_end=STRING_BUF_RESERVE;
    s->reserve_count_begin=STRING_BUF_RESERVE;
    s->length=0;
    s->length_begin=0;

    s->end=s->begin;
    *s->begin='\0';
}

void str_copy(String *str1, String *str2)
{

}

void str_concat(String *s1, String *s2)
{
}

void str_free(String *s)
{
    free(s->begin - s->reserve_count_begin + s->length_begin);
    free(s);
}

void str_print(String *s)
{
    int i;
    for(i=0; i<s->length; i++)
        printf("%c", s->begin[i]);
}

int str_to_int(String *s)
{
    return atoi(s->begin);
}

int str_comparision(String *s1, String *s2)
{
    return strcmp(s1->begin, s2->begin);
}
