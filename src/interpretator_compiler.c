#include "interpretator_compiler.h"

typedef struct
{
    Jmp *end,
        *begin;
}BlockLoop;

typedef struct
{
    Jmp *end;
}BlockIf;

static Stack           *next_element,
                       *block,
                       *if_places;
static struct ListNode *i;
static List            *ret;
static BlockLoop       *cur_loop;
static Stack           *loops_not_updated_end;

static void add_loop()
{
    BlockLoop *block_loop=malloc(sizeof(BlockLoop));
    block_loop->begin=malloc(sizeof(Jmp));
    block_loop->end=malloc(sizeof(Jmp));
    block_loop->begin->place=ret->end;
    cur_loop=block_loop;
    push(block, new_data(LOOP, cur_loop));
}

static void add_JZ(IfZero *iz)
{
    Jz *alloc=malloc(sizeof(Jz));
    alloc->var=iz->var->data;
    list_push(if_places, &alloc->place);
    list_push(ret, new_data(JZ, alloc));
}

static Data* add_interpretator_cond(Data *parser_cond)
{
    Data *alloc;
    Jz      *jz_data;

    alloc=malloc(sizeof(Data));

    switch(parser_cond->type)
    {
    case IZ:
        add_JZ(parser_cond->data);
        break;
    }
}

static void print_cond(Data *run_data)
{
    switch(run_data->type)
    {
    case IZ:
        printf("IZ ");
        break;

    case INZ:
        printf("INZ ");
        break;
    }
}

static void add_if_refs(If *if_data)
{
    struct ListNode *i;
    Stack           *s;
    Data            *run_data,
                    *op1,
                    *op2;

    Jmp             *jmp_data;
    Jz              *jz_data ;
    Jnz             *jnz_data;
    Je              *je_data ;
    Jne             *jne_data;
    Jlt             *jlt_data;
    Jgt             *jgt_data;
    Jle             *jle_data;
    Jge             *jge_data;

    i=if_data->cond->begin;
    s=stack_init();

    while(i)
    {
        run_data=i->data;

        switch(run_data->type)
        {
        case OR:
            op2=pop(s);
            op1=pop(s);
            add_interpretator_cond(op1);
            add_interpretator_cond(op2);
            break;

        case AND:
            op2=pop(s);
            op1=pop(s);
            add_interpretator_cond(op1);
            add_interpretator_cond(op2);
            break;

        case IZ:
            push(s, run_data);
            break;

        case INZ:
            push(s, run_data);
            break;

        default: printf(":(");
        }

        i=i->next;
    }

    free(s);
}

static void add_if()
{
    BlockIf *block_if=malloc(sizeof(BlockIf));
    block_if->end=malloc(sizeof(Jmp));
    block_if->end->place=ret->end;
    push(block, new_data(IF, block_if));
}

static void add_break()
{
    list_push(ret, new_data(JMP, cur_loop->end));
}

static void update_cur_loop()
{
    Data *data;
    NodeStack *i=block->begin;

    cur_loop=0;
    while(i)
    {
        data=i->data;
        if(data->type==LOOP)
        {
            cur_loop=data->data;
            break;
        }
        i=i->previouse;
    }
}

static void update_loop(BlockLoop *block_loop)
{
    list_push(ret, new_data(JMP, block_loop->begin));
    block_loop->end->place=ret->end;
    push(loops_not_updated_end, block_loop);
    update_cur_loop();
}

static void update_loop_refs()
{
    NodeStack *i=loops_not_updated_end->begin;
    BlockLoop *loop;

    while(i)
    {
        loop=i->data;
        loop->end->place=loop->end->place->next;
        if(loop->begin->place)
            loop->begin->place=loop->begin->place->next;
        else
            loop->begin->place=ret->begin;
        i=i->previouse;
    }
}

static void update_if(BlockIf *block_if)
{

}

static void add_run_element(Data *run_data)
{
    switch(run_data->type)
    {
    case ASSIGNMENT:
        i=i->next;
        break;

    case IF:
        add_if();
        push(next_element, i->next);
        add_if_refs(run_data->data);
        i=((If*)run_data->data)->eval->begin;
        break;

    case LOOP:
        add_loop();
        push(next_element, i->next);
        i=((Loop*)run_data->data)->eval->begin;
        break;

    case BREAK:
        add_break();
        i=i->next;
        break;

    default:
        list_push(ret, run_data);
        i=i->next;
    }
}

static void update_block(Data *run_data)
{
    switch(run_data->type)
    {
    case LOOP:
        update_loop(run_data->data);
        break;

    case IF:
        update_if(run_data->data);
        break;
    }
}

List *get_fast_run(Function *main)
{
    Data *run_data;

    if_places=list_init();
    ret=list_init();
    next_element=stack_init();
    block=stack_init();
    i=main->body->begin;
    loops_not_updated_end=stack_init();

    while(i)
    {
        add_run_element(i->data);

        while(next_element->begin && i==0)
        {
            update_block(pop(block));
            i=pop(next_element);
        }
    }

    update_loop_refs();

    return ret;
}
