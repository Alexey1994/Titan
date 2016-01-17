#ifndef PARSER_OPERATIONS_H_INCLUDED
#define PARSER_OPERATIONS_H_INCLUDED

char parser_error();

char parser_putc();

char parser_element_init();
char parser_ptrs_init();
char parser_array_init();
char parser_var_init();
char parser_const_init();
char parser_const_string_init();

char parser_assignment();
char parser_loop();
char parser_if();

char parser_break();
char parser_continue();
char parser_function();

char parser_end();
char parser_call();

char parser_inc();
char parser_dec();

char parser_add();
char parser_sub();
char parser_mul();
char parser_div();

char parser_shr();
char parser_shl();

char parser_xor();
char parser_and();
char parser_or();
char parser_not();

char parser_eq();
char parser_neq();
char parser_gt();
char parser_lt();
char parser_ge();
char parser_le();

char parser_alloc();

#endif // PARSER_OPERATIONS_H_INCLUDED
