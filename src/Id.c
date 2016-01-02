#include "Id.h"

Id* id_init(char min, char max)
{
    Id *id=malloc(sizeof(Id));

    id->prev_id=malloc(256);
    id->prev_id[0]=min;
    id->prev_id[1]='\0';
    id->min=min;
    id->max=max;

    return id;
}

char *generate_id(Id *id)
{
    int i=strlen(id->prev_id)-1;

    do
    {
        id->prev_id[i]++;
        if(id->prev_id[i]==id->max+1)
        {
            id->prev_id[i]=id->min;
            i--;
        }
        else
            break;

        if(i<0)
        {
            i=strlen(id->prev_id);
            id->prev_id[i]=id->min;
            id->prev_id[i+1]='\0';
            break;
        }
    }
    while(1);

    return id->prev_id;
}
