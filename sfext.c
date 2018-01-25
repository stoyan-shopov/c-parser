#include "engine.h"
#include "sf-word-wizard.h"

#include "sfext.h"

static void do_squared(void) { cell x = sf_pop(); sf_push(x * x); }
static void do_gcd(void)
/* implements the FORTH word:
: gcd ( n1 n2 | n) begin over mod swap over 0= until nip ;
*/
{ do { do_over(); do_mod(); do_swap(); do_over(); do_zero_not_equals(); } while (sf_pop()); do_nip(); }

static struct word dict_base_dummy_word[1] = { MKWORD(0, 0, "", 0), };
static const struct word custom_dict[] = {
	MKWORD(dict_base_dummy_word,	0,		"squared",	do_squared),
	MKWORD(custom_dict,		__COUNTER__,	"gcd",	do_gcd),
	MKWORD(custom_dict,		__COUNTER__,	"id\"",	do_id_quote),
	MKWORD(custom_dict,		__COUNTER__,	">>long",	do_to_long),
	MKWORD(custom_dict,		__COUNTER__,	">>int",	do_to_int),
	MKWORD(custom_dict,		__COUNTER__,	">>void",	do_to_void),
	MKWORD(custom_dict,		__COUNTER__,	">struct",	do_to_struct),
	MKWORD(custom_dict,		__COUNTER__,	"define-variables",	do_define_variables),
	MKWORD(custom_dict,		__COUNTER__,	"aggregate{",	do_aggregate_begin),
	MKWORD(custom_dict,		__COUNTER__,	"}aggregate-end",	do_aggregate_end),
	MKWORD(custom_dict,		__COUNTER__,	">struct-declaration",	do_to_struct_declaration),
	MKWORD(custom_dict,		__COUNTER__,	"declaration-end",	do_declaration_end),
	MKWORD(custom_dict,		__COUNTER__,	">pointer",	do_to_pointer),
	MKWORD(custom_dict,		__COUNTER__,	">array{",	empty),
	MKWORD(custom_dict,		__COUNTER__,	"}array-end",	do_array_end),
	MKWORD(custom_dict,		__COUNTER__,	">anonymous-aggregate",	do_to_anonymous_aggregate),
	MKWORD(custom_dict,		__COUNTER__,	">function-param-type-list{",	do_to_function_parameter_type_list_begin),
	MKWORD(custom_dict,		__COUNTER__,	"}function-param-type-list-end",	do_function_parameter_type_list_end),
	MKWORD(custom_dict,		__COUNTER__,	">array[]",	do_to_empty_array),
	MKWORD(custom_dict,		__COUNTER__,	">abstract-declarator-array[]",	do_to_abstract_declarator_array),
	MKWORD(custom_dict,		__COUNTER__,	">function-id-list{",	do_to_function_id_list_begin),
	MKWORD(custom_dict,		__COUNTER__,	"}function-id-list-end",	do_function_id_list_end),
	MKWORD(custom_dict,		__COUNTER__,	">abstract-parameter-declaration",	do_to_abstract_parameter_declaration),
	MKWORD(custom_dict,		__COUNTER__,	">parameter-declaration",	do_to_parameter_declaration),
	MKWORD(custom_dict,		__COUNTER__,	">parameter-list",	do_to_parameter_list),
	MKWORD(custom_dict,		__COUNTER__,	">parameter-declaration-specifiers",	do_to_parameter_declaration_specifiers),
	MKWORD(custom_dict,		__COUNTER__,	">id-list",	do_to_id_list),

}, * custom_dict_start = custom_dict + __COUNTER__;

static void sf_opt_sample_init(void) __attribute__((constructor));
static void sf_opt_sample_init(void)
{
	sf_merge_custom_dictionary(dict_base_dummy_word, custom_dict_start);
}
