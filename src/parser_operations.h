#ifndef PARSER_OPERATIONS_H_INCLUDED
#define PARSER_OPERATIONS_H_INCLUDED

typedef char try;

try parser_error();

try parser_putc();

try parser_element_init();
try parser_ptrs_init();
try parser_array_init();
try parser_integer_init();
try parser_real_init();
try parser_const_init();
try parser_const_string_init();

try parser_assignment();
try parser_loop();
try parser_if();

try parser_break();
try parser_continue();
try parser_function();

try parser_end();
try parser_call();

try parser_inc();
try parser_dec();

try parser_add();
try parser_sub();
try parser_mul();
try parser_div();

try parser_shr();
try parser_shl();

try parser_xor();
try parser_and();
try parser_or();
try parser_not();

try parser_eq();
try parser_neq();
try parser_gt();
try parser_lt();
try parser_ge();
try parser_le();

try parser_alloc();

#endif // PARSER_OPERATIONS_H_INCLUDED
