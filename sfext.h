#ifndef __SFEXT_H__
#define __SFEXT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "engine.h"
#include "sf-word-wizard.h"

void do_id_quote(void);
void do_to_long(void);
void do_to_int(void);
void do_to_void(void);
void do_to_struct(void);
void do_define_variables(void);
void do_aggregate_begin(void);
void do_aggregate_end(void);
void do_to_struct_declaration(void);
void do_declaration_end(void);
void do_to_pointer(void);
void do_array_end(void);
void do_to_anonymous_aggregate(void);
void do_to_function_parameter_type_list_begin(void);
void do_function_parameter_type_list_end(void);
void do_to_empty_array(void);
void do_to_abstract_declarator_array(void);
void do_to_function_id_list_begin(void);
void do_function_id_list_end(void);
void do_to_abstract_parameter_declaration(void);
void do_to_parameter_declaration(void);
void do_to_parameter_list(void);
void do_to_parameter_declaration_specifiers(void);
void do_to_id_list(void);
void do_declaration_without_declarator_list_begin(void);
void do_declaration_with_declarator_list_begin(void);
void do_compound_statement_begin(void);

void empty(void);

void sf_base_dictionary_init(void);
void sf_expression_dictionary_init(void);

#ifdef __cplusplus
}
#endif

#endif // __SFEXT_H__
