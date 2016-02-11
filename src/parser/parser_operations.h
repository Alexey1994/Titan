#ifndef PARSER_OPERATIONS_H_INCLUDED
#define PARSER_OPERATIONS_H_INCLUDED

typedef char try;

try parser_error();

try parser_print();

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

try parser_alloc();

#endif // PARSER_OPERATIONS_H_INCLUDED
