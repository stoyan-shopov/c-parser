#ifndef SFEXT_H
#define SFEXT_H

void do_id_quote(void);
void do_to_long(void);
void do_to_int(void);
void do_to_struct(void);
void do_define_variables(void);
void do_aggregate_begin(void);
void do_aggregate_end(void);
void do_struct_declarator_list_begin(void);
void do_struct_declarator_list_end(void);
void do_declaration_end(void);
void do_to_pointer(void);
void do_array_end(void);
void do_to_anonymous_aggregate(void);
void do_to_function_parameter_type_list_begin(void);
void do_to_function_parameter_type_list_end(void);
void do_to_empty_array(void);
void do_parameter_list_boundary(void);
void do_to_abstract_declarator_array(void);
void do_to_abstract_declarator_function_id_list_begin(void);
void do_abstract_declarator_function_id_list_end(void);
void do_to_function_id_list_begin(void);
void do_function_id_list_end(void);

void empty(void);

#endif // SFEXT_H
