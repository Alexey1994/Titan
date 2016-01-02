#ifndef INTERPRETATOR_OPERATIONS_H_INCLUDED
#define INTERPRETATOR_OPERATIONS_H_INCLUDED

void interpretator_loop();
void interpretator_if();

void interpretator_break();
void interpretator_continue();

void interpretator_call();

void interpretator_putc();

void interpretator_var_var();
void interpretator_var_const();
void interpretator_var_el();
void interpretator_var_ptrs();
void interpretator_var_array();
void interpretator_var_const_string();

void interpretator_el_var();
void interpretator_el_const();
void interpretator_el_el();
void interpretator_el_ptrs();
void interpretator_el_array();
void interpretator_el_const_string();

void interpretator_ptrs_var();
void interpretator_ptrs_const();
void interpretator_ptrs_el();
void interpretator_ptrs_ptrs();
void interpretator_ptrs_array();
void interpretator_ptrs_const_string();

void interpretator_array_var();
void interpretator_array_const();
void interpretator_array_el();
void interpretator_array_ptrs();
void interpretator_array_array();
void interpretator_array_const_string();

void interpretator_inc();
void interpretator_dec();

void interpretator_add();
void interpretator_sub();
void interpretator_mul();
void interpretator_div();

void interpretator_shr();
void interpretator_shl();

void interpretator_xor();
void interpretator_and();
void interpretator_or();
void interpretator_not();

void interpretator_eq();
void interpretator_neq();
void interpretator_gt();
void interpretator_lt();
void interpretator_ge();
void interpretator_le();

void interpretator_element_alloc();
void interpretator_array_alloc();
void interpretator_pointers_alloc();

#endif // INTERPRETATOR_OPERATIONS_H_INCLUDED
