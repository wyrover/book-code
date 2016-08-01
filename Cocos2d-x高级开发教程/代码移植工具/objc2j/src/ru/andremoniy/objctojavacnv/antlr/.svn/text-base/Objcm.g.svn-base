grammar Objcm;

options {
  output = AST;
  k = 4;
}

tokens {
	MODIFIER;
	TYPE;
	NAME;
	VALUE;
	FIELD;
	INTERFACE;
	CATEGORY;
	METHOD;
	PARAM;
	IMPLEMENTATION;
	OBJECT;
	METHOD_NAME;
	MESSAGE;
	BLOCK;
	METHOD_CALL;
	TYPE_CONVERTION;
	PREFIX;
	INCOMPLETE_PREFIX;
	MSG_LIST;
	GENERIC;
	TYPEDEF_ELEMENT;
	TYPEDEF;
	TYPEDEF_STRUCT;
	STRUCT_FIELD;
	STATIC;
	STATIC_METHOD;
	OPERATOR;
	VA_ARGS;
	ENUM;
	SUPER_CLASS;
	EXTERN;
	M_IMPORT;
	M_TYPE_START;
	SWITCH;
	SWITCH_EXPRESSION;
	SWITCH_BODY;	
	CASE_STMT;
	CASE_EXPR;
	CASE_BODY;
	DEFAULT_STMT;
	DEFAULT_BODY;
	IF_STMT;
	IF_EXPR;
	IF_BLOCK;
	SELECTOR;
	PROTOCOL;
	ENCODE;
	SELECTOR_VALUE;
	OP;
	OPER;
	EXPR;
	CLASSICAL_EXPR;
	CLASSICAL_EXPR_2;
	WHILE_STMT;
	WHILE_EXPR;
	NOT;
	BREAK;
	RETURN_STMT;
	FIELD_ACCESS;
	FIELD_TYPE_STARTED;
	STATIC_START;
	FOR_STMT;
	FOR_STMT_EXPR;
	FOR_IN_STMT;
	METHOD_MSG;
	EXPR_FULL;
	EXPR_ASSIGN;
	EXPR_OR_OR;
	EXPR_AND_AND;
	EXPR_OR;
	EXPR_XOR;
	EXPR_AND;
	EXPR_EQ;
	EXPR_COND;
	EXPR_MOV;
	EXPR_ADD;
	EXPR_MULT;
	EXPR_TYPE;
	EXPR_NOT;
	SIMPLE_EXPR;
	EXPR_QUESTION;
	FUNCTION;
	THROW_STMT;
	THROW_EXCEPTION;
	ASSIGN;
	VARIABLE_INIT;
	STATIC_TYPE;
	INDEX;
	INDEX_NUMBER;
	ARRAY_INIT;
	CONST_EXPR;
	GOTO;
	STRING;
	TYPE_RENAME;
}

@header {
package ru.andremoniy.objctojavacnv.antlr.output;
}
@lexer::header {
package ru.andremoniy.objctojavacnv.antlr.output;
}


/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/
code	:	
		code_internal+
//		EOF
	;
	
code_internal
	:	operator_wrapper
	| 	implementation_wrapper
	;

operator_wrapper
	:	operator -> ^(OPERATOR operator);
 
operator
	:	extern_wrapper 
	|	static_section_wrapper
	|	type_start_wrapper 
	| 	interface_declaration_wrapper 
	|	typedef_struct_declaration 
	|	typedef_declaration_wrapper
	|	typedef
	;
	
/*------------------------------------------------------------------
 * IMPLEMENTATION RULES
 *------------------------------------------------------------------*/
 
 extern_wrapper
 	:	extern -> ^(EXTERN)
 	;
 
 extern	:	'extern'  '"C"'  L_FBR ~(R_FBR) R_FBR
 	;
implementation_wrapper
 	:	implementation -> ^(IMPLEMENTATION implementation);
 	
implementation
	:	'@implementation'  name  category? super_class?
		implementation_body*
		'@end'  SEMICOLON?
	; 	

category:	L_BR  name  R_BR -> ^(CATEGORY name);	

implementation_body
	:	directives
	| 	typedef
	| 	static_section_wrapper
	| 	implementation_method_wrapper
//	|	classic_method_declaration_wrapper
	|	field_type_started_wrapper
	| 	field_declaration_wrapper
	|	typedef_struct_declaration_wrapper
	;

directives
	:	synthesize
	;	
	
synthesize
	:	'@synthesize'  ID (L_EQ ID)? SEMICOLON
	;	
	
typedef	:	'typedef'  (typedef_declaration_wrapper | typedef_struct_declaration_wrapper | type_rename_wrapper) 
	;
	
type_rename_wrapper
	:	type_rename -> ^(TYPE_RENAME type_rename);	
	
type_rename
	:	ID ID SEMICOLON;	
		
static_section_wrapper
	:	static_section -> ^(STATIC static_section);		
	
static_section
	:	STATIC_PREFIX CONST_PREFIX? STRUCT_PREFIX? ENUM_PREFIX? static_section2;

static_section2
	:	inline_section 
	| 	implementation_method_wrapper2 
	| 	static_section3
	;
	
inline_section
	:	'inline'  field_type name  method_params2? (block_wrapper | SEMICOLON);
	
static_section3
	:	field_type name indexed*  static_end;
	
	
static_end
	:	implementation_method_wrapper3
		| field_end;
		
field_end
	:	field_end_internal -> ^(FIELD field_end_internal);
	
field_end_internal
	:	value_set? SEMICOLON;	
		
implementation_method_wrapper3
	:	implementation_method3 -> ^(METHOD implementation_method3);
	
implementation_method3
	:	method_header_body3  SEMICOLON?
		attribute?
		block_wrapper?
	;
	
attribute
	:	'__attribute__'  L_BR  L_BR  'constructor'  R_BR  R_BR  SEMICOLON;	
	
implementation_method2
	:	 SEMICOLON?
		block_wrapper
	;
	
field_type_started_wrapper
	:	field_type_started -> ^(FIELD_TYPE_STARTED field_type_started);	
	
field_type_started
	:	field_type name (classic_method_declaration_wrapper | field_declaration_end_wrapper);
	
field_declaration_end_wrapper
	:	field_declaration_end -> ^(FIELD field_declaration_end);
	
classic_method_declaration_wrapper
	:	classic_method_declaration -> ^(STATIC_METHOD classic_method_declaration);	
	
classic_method_declaration
//	:	field_type name method_params2 
	:	method_params2 
		block_wrapper 
	;	

implementation_method_wrapper
	:	implementation_method -> ^(METHOD implementation_method);

implementation_method_wrapper2
	:	implementation_method2 -> ^(METHOD implementation_method2);

implementation_method
	:	method_header_body SEMICOLON?
		block_wrapper 
	;
	
block_wrapper
	:	block -> ^(BLOCK block);	
	
block	:		
		L_FBR
			(block_internal_full | block)*		
		R_FBR
	;

block_case
	:		
		L_FBR
			(block_internal_full | block)*		
			default_stmt?
		R_FBR
	;
	
block_internal_full
	:	block_internal
	;

block_internal
	:	try_stmt
	|	throw_stmt_wrapper
	|	static_start_wrapper SEMICOLON
	|	do_stmt
	|	const_expression SEMICOLON
	|	variable_init_wrapper
	|	if_stmt_wrapper
	|	else_stmt
	|	switch_stmt_wrapper
	|	return_stmt
	|	for_stmt_wrapper
	|	while_stmt_wrapper
	|	single_operators
	|	struct_variable SEMICOLON
	|	enum_variable SEMICOLON
	|	typeof_started SEMICOLON
	|	known_type_started SEMICOLON
	|	SEMICOLON	
	|	goto_wrapper
	|	'extern'
	;
	
goto_wrapper
	:	goto_operator -> ^(GOTO goto_operator);

goto_operator
	:	'goto' ID SEMICOLON;		

variable_init_wrapper
	:	variable_init -> ^(VARIABLE_INIT variable_init);
	
variable_init
	:	full_expr_wrapper ((full_expr2? SEMICOLON) | COLON);	
	
known_type_started
	:	known_types ASTERISK* full_expr2;		
	
typeof_started
	:	typeof full_expr_wrapper
	;	
	
struct_variable
	:	STRUCT_PREFIX full_expr_wrapper;

enum_variable
	:	ENUM_PREFIX full_expr_wrapper;
		
const_expression
	:	CONST_PREFIX full_expr_wrapper;

throw_stmt_wrapper
	:	throw_stmt -> ^(THROW_STMT throw_stmt);

throw_stmt
	:	'@throw'  SEMICOLON	
	;
	
try_stmt:	'@try'  block  catch_stmt* ('@finally'  block)?
	;
	
catch_stmt
	:	'@catch'  L_BR  ID  ASTERISK* ID  R_BR  block 
	;	

switch_stmt_wrapper
	:	switch_stmt -> ^(SWITCH switch_stmt);

switch_stmt
	:	'switch'  L_BR  switch_expr R_BR  
		switch_body_wrapper
	;	
	
switch_body_wrapper
	:	L_FBR switch_body R_FBR -> ^(SWITCH_BODY switch_body);	
	
switch_body
	:	block_internal_full*
		switch_internal*
	;	

switch_expr
	:	classical_expr_wrp -> ^(SWITCH_EXPRESSION classical_expr_wrp);

switch_internal
	:	case_stmt_wrapper
	| 	default_stmt_wrapper;

case_stmt_wrapper
	:	case_stmt -> ^(CASE_STMT case_stmt);

case_stmt
	:	'case'  case_expr_wrapper  COLON 
		case_body_wrapper?
	;	
	
case_body_wrapper
	:	case_body -> ^(CASE_BODY case_body);	
	
case_body
	:	block_break 
	| 	block_internal_full+
	 ;
			
case_expr_wrapper
	:	case_expr -> ^(CASE_EXPR case_expr);
					
case_expr
	:	const_expr 
	| 	ID;	
	
block_break
	:	block_case single_operators?
	;

default_stmt_wrapper
	:	default_stmt -> ^(DEFAULT_STMT default_stmt);

default_stmt
	:	'default'  COLON 
		default_body_wrapper?
	;	

default_body_wrapper
	:	default_body -> ^(DEFAULT_BODY default_body)
	;
	
default_body
	:	block_break
	| 	block_internal_full+
	;		
	
single_operators
	:	SINGLE_OP 
	| 	break_wrapper
	;
	
break_wrapper
	:	break_stmt -> ^(BREAK break_stmt);
	
break_stmt
	:	BREAK_WORD  SEMICOLON;	
	
do_stmt	:	'do'  if_stmt_block  'while'  L_BR  expression R_BR;

while_stmt_wrapper
	:	while_stmt -> ^(WHILE_STMT while_stmt)
	;

while_stmt
	:	'while'  L_BR  while_expr R_BR  if_stmt_block_wrapper; 	
	
while_expr
	:	 classical_expr_wrp -> ^(WHILE_EXPR classical_expr_wrp);	
	
for_stmt_wrapper
	:	for_stmt -> ^(FOR_STMT for_stmt);	
	
for_stmt:	'for'  L_BR  for_stmt_expr_wrapper R_BR  if_stmt_block
	;
	
for_stmt_expr_wrapper
	:	for_stmt_expr -> ^(FOR_STMT_EXPR for_stmt_expr);	
	
for_stmt_expr
	:	full_expr_wrapper (for_stmt_int1 | (full_expr2 (for_stmt_int1 | for_stmt_int2)) | for_stmt_int2)
	|	for_stmt_int2
	;	
			
for_stmt_int1
	:	'in' object_wrapped2  -> ^(FOR_IN_STMT object_wrapped2);	
	
for_stmt_int2
	:	SEMICOLON classical_expr_wrp? SEMICOLON classical_expr_wrp?
	;

//return_stmt_wrapper
//	:	return_stmt -> ^(RETURN_STMT return_stmt)
//	;
	
return_stmt
	:	'return' classical_expr_wrp? SEMICOLON -> ^(RETURN_STMT classical_expr_wrp? SEMICOLON);	

if_stmt_wrapper
	:	if_stmt -> ^(IF_STMT if_stmt);

if_stmt	:	'if'  L_BR  if_expr R_BR  if_stmt_block_wrapper
	;	
	
if_expr	:	 classical_expr_wrp -> ^(IF_EXPR classical_expr_wrp);	
	
else_stmt
	:	'else'  if_stmt_block;

if_stmt_block_wrapper
	:	if_stmt_block -> ^(IF_BLOCK if_stmt_block);

if_stmt_block
	:	block_wrapper
	| 	block_internal
	;	

static_start_wrapper
	:	static_start -> ^(STATIC_START static_start);
	
static_start
	:	STATIC_PREFIX CONST_PREFIX? static_type_wrapper? full_expr2
	;
	
static_type_wrapper
	:	static_type -> ^(STATIC_TYPE static_type);	
	
static_type
	:	object_name ASTERISK*;	
	
id_part_end
	:	id_part_end_internal+
	;
	
id_part_end_internal	
	:	(DOT | '->') id_part -> ^(FIELD_ACCESS id_part)
	;	
	
known_types
	:	'unsigned'
	|	VOID
	;	

id_part	:	 name_internal function_brackets_wrapper? index_wrapper*
	;
	
object_name
	:	id_part id_part_end?
	;	
	
method_call_wrapper
	:	L_KBR method_call2 R_KBR -> ^(METHOD_CALL method_call2)
	;
				
method_call_wrapper2
	:	method_call2 -> ^(METHOD_CALL method_call2);	
	
method_call2
	:	object_wrapper method_name ( method_call_message_list_wrapper)?;

object_wrapper
	:	object_wrapper_internal -> ^(OBJECT object_wrapper_internal);
	
object_wrapper_internal
//	:	L_BR object_name generic? ASTERISK* R_BR (object_name | method_call_wrapper)
	:	type_convertion (object_name | method_call_wrapper)
	|	object_name
	|	method_call_wrapper
	|	STRING_LITERAL
	;	

method_message3
	:	classical_expr_wrp -> ^(MESSAGE classical_expr_wrp);
		
type_convertion
	:	type_convertion_internal -> ^(TYPE_CONVERTION type_convertion_internal);
		
type_convertion_internal
	:	L_BR 'const'? 'unsigned'? type_internal generic? ASTERISK* R_BR -> ^(type_internal generic?);

method_name
	:	ID -> ^(METHOD_NAME ID)
	|	property -> ^(METHOD_NAME property);

method_call_message_list_wrapper
	:	method_call_message_list -> ^(MSG_LIST method_call_message_list);

method_call_message_list
	:	method_call_message_first_wrapper method_call_message_wrapper*;	

method_call_message_first_wrapper
	:	method_call_message_first -> ^(METHOD_MSG method_call_message_first);

method_call_message_first
	:	COLON  message;
	
method_call_message_wrapper
	:	method_call_message -> ^(METHOD_MSG method_call_message);
	
method_call_message
	:	(prefix | incomplete_prefix)  message;
	
message	:	msg_expr (COMMA msg_expr)* -> ^(MESSAGE msg_expr (COMMA  msg_expr)*)
	;
		

msg_expr 
	: 	classical_expr_wrp
	;	
	
/*------------------------------------------------------------------
 * COMMON EXPRESSION RULES
 *------------------------------------------------------------------*/

expression
	:	classical_expr_wrp
	|	struct_init
	;

full_expr_wrapper
	:	full_expr -> ^(EXPR_FULL full_expr);

full_expr
	:	classical_expr_wrp (COMMA classical_expr_wrp)*;

full_expr2
	:	classical_expr_wrp2 (COMMA classical_expr_wrp2)*;

expr_assign_wrapper
	:	expr_assign -> ^(EXPR_ASSIGN expr_assign);
	
expr_assign	
	:	assign_wrapper (classical_expr_wrp | array_init_wrapper | (func_pointer2 method_call_wrapper?));

array_init_wrapper
	:	array_init -> ^(ARRAY_INIT array_init);	
	
array_init
	:	L_FBR (classical_expr_wrp3 (COMMA classical_expr_wrp3)*)? R_FBR
	;
	
classical_expr_wrp3
	:	array_init_wrapper 
	|	classical_expr_wrp
	;	
	
classical_expr_wrp
	:	classical_expr -> ^(CLASSICAL_EXPR classical_expr);

classical_expr
	:	simple_expr_wrapper (expr_question_wrapper | expr_assign_wrapper)?;
	
classical_expr_wrp2
	:	classical_expr2 -> ^(CLASSICAL_EXPR_2 classical_expr2);

classical_expr2
	:	(object_name | func_pointer1) expr_assign_wrapper?;

expr_question_wrapper
	:	expr_question -> ^(EXPR_QUESTION expr_question);

expr_question
	:	L_QUESTION classical_expr_wrp COLON classical_expr_wrp;

assign_wrapper
	:	assign -> ^(ASSIGN assign);
	
assign
	:	L_EQ | L_PLUS_EQ | L_MINUS_EQ | L_MULT_EQ | L_DIV_EQ
	|   L_PERC_EQ | L_LEFT_EQ | L_RIGHT_EQ | L_AND_EQ | L_XOR_EQ | L_OR_EQ
	;
	
simple_expr_wrapper
	:	simple_expr -> ^(SIMPLE_EXPR simple_expr);	

simple_expr
	:	expr_or_or -> ^(EXPR_OR_OR expr_or_or);

expr_or_or
	:	expr_and_and_wrapper (L_OR_OR simple_expr_wrapper)*;

expr_and_and_wrapper
	:	expr_and_and -> ^(EXPR_AND_AND expr_and_and);

expr_and_and
	:	expr_or_wrapper (L_AND_AND simple_expr_wrapper)*;

expr_or_wrapper
	:	expr_or -> ^(EXPR_OR expr_or);
	
expr_or
	:	expr_xor_wrapper (L_OR simple_expr_wrapper)*;

expr_xor_wrapper
	:	expr_xor -> ^(EXPR_XOR expr_xor);

expr_xor
	:	expr_and_wrapper (L_XOR simple_expr_wrapper)*;
	
expr_and_wrapper
	:	expr_and -> ^(EXPR_AND expr_and);	

expr_and
	:	expr_eq_wrapper (L_AND simple_expr_wrapper)*;
	
expr_eq_wrapper
	:	expr_eq -> ^(EXPR_EQ expr_eq);	

expr_eq
	:	expr_cond_wrapper ((L_EQ_EQ | L_NEQ) simple_expr_wrapper)*;

expr_cond_wrapper
	:	expr_cond -> ^(EXPR_COND expr_cond);

expr_cond
	:	expr_mov_wrapper ((L_LESS | L_MORE | L_LESS_EQ | L_MORE_EQ) simple_expr_wrapper)*;

expr_mov_wrapper
	:	expr_mov -> ^(EXPR_MOV expr_mov);	

expr_mov
	:	expr_add_wrapper ((L_LEFT | L_RIGHT) simple_expr_wrapper)*;	

expr_add_wrapper
	:	expr_add -> ^(EXPR_ADD expr_add);

expr_add
	:	expr_mult_wrapper ((L_PLUS | L_MINUS) simple_expr_wrapper)*;

expr_mult_wrapper
	:	expr_mult -> ^(EXPR_MULT expr_mult);

expr_mult
	:	expr_type_wrapper ((ASTERISK | L_DIV | L_PERC) simple_expr_wrapper)*;
	
expr_type_wrapper
	:	expr_type -> ^(EXPR_TYPE expr_type);

expr_type
	:	type_convertion? expr_unname;
	
expr_unname
	:	(ASTERISK type_convertion?)? expr_addr;
	
expr_addr
	:	L_AND? expr_sign;
	
expr_sign
	:	(L_MINUS | L_PLUS)? expr_not_wrapper;
	
expr_not_wrapper
	:	expr_not -> ^(EXPR_NOT expr_not);	
	
expr_not
	:	L_NOT? expr_not2;
	
expr_not2
	:	L_TILDA? expr_size_of;
	
expr_size_of
	:	SIZEOF? expr_xcrement;
	
expr_xcrement
	:	(L_PLUS_PLUS | L_MINUS_MINUS)? object_wrapped1;
	
object_wrapped1
	:	object_wrapped2 (L_PLUS_PLUS | L_MINUS_MINUS)?
	;

object_wrapped2
	: 	'unsigned'? object_name generic?
	|	method_call_wrapper id_part_end?
	|	L_BR classical_expr_wrp R_BR id_part_end?
	|	const_expr_wrapper
	|	a_started
	;
	
function_brackets_wrapper
	:	function_brackets -> ^(FUNCTION function_brackets);	
	
function_brackets
	:	L_BR (classical_expr_wrp (COMMA classical_expr_wrp)*)? R_BR;	

index_wrapper
	:	index -> ^(INDEX index);
	
index	:	L_KBR index_number_wrapper? R_KBR;

index_number_wrapper
	:	classical_expr_wrp -> ^(INDEX_NUMBER classical_expr_wrp);

struct_init
	:	L_BR  STRUCT_PREFIX? ID  R_BR  L_FBR 
			struct_init_line (COMMA  struct_init_line)*
		R_FBR 
	;	
	
struct_init2
	:	L_FBR 
			struct_init_line (COMMA  struct_init_line)*
		R_FBR 
	;	

struct_init_line
	:	struct_init_var
	|	struct_init_expression
	;	
	
struct_init_expression
	:	classical_expr_wrp;	
	
struct_init_method
	:	classical_expr_wrp;	
	
struct_init_var
	:	(DOT id_part)+  L_EQ  (in_q_brackets | expression) ;	

a_started
	:	a_selector_wrapper
	|	a_encode_wrapper
	|	a_protocol_wrapper
	;

typeof	:	'__typeof__'  L_BR  ID  R_BR ;
	
a_selector_wrapper
	:	a_selector -> ^(SELECTOR a_selector);	
	
a_selector:	'@selector' L_BR a_selector_value_wrapper R_BR;	

a_protocol_wrapper
	:	a_protocol -> ^(PROTOCOL a_protocol);
	
a_protocol
	:	'@protocol' L_BR a_selector_value_wrapper R_BR;	

a_encode_wrapper
	:	a_encode -> ^(ENCODE a_encode);

a_selector_value_wrapper
	:	a_selector_value -> ^(SELECTOR_VALUE a_selector_value);
	
a_selector_value
	:	 sel_id  (COLON  (sel_id  COLON)*)?;
	
sel_id	:	ID
	|	property
	;	

a_encode:	'@encode' L_BR ~(R_BR)+ R_BR;

const_expr_wrapper
	:	const_expr -> ^(CONST_EXPR const_expr);

const_expr	
	: 	NUMBER | STRING_LITERAL | STRING_LITERAL2 | STRING_LITERAL3 | null_stmt;
	
//string_wrapper
//	:	string_internal -> ^(STRING string_internal);	
	
//string_internal
//	:	STRING_LITERAL | STRING_LITERAL2 | STRING_LITERAL3;	
	
string_literal3
	:	STRING_LITERAL3;	

method_brackets
	:	L_BR  (expression  (COMMA  classical_expr_wrp)*)? R_BR;
	
func_pointer1
	:	type_internal  ASTERISK? L_BR ASTERISK? type_internal  R_BR  func_pointer_params?
	;
	
func_pointer2
	:	L_BR  type_internal L_BR  ASTERISK  R_BR  func_pointer_params  R_BR
	;	
	
func_pointer_params
	:	L_BR  ID  ASTERISK* (COMMA  ID  ASTERISK*)* R_BR;
		
method_start
	:	ID method_brackets?;
	
in_q_brackets
	:	L_FBR  q_brackets_source R_FBR
	;
	
q_brackets_source
	:	q_source
	|	q_br_source
	;	
	
q_br_source
	:	in_q_brackets  (COMMA  in_q_brackets)* COMMA?
	;
	
q_source:	(expression  (COMMA  expression)*)?
	;	
	
simple_method_call
	:	ID method_brackets?;	
					
index_brackets
	:	L_KBR  classical_expr_wrp R_KBR
	;

/*------------------------------------------------------------------
 * INTERFACE RULES
 *------------------------------------------------------------------*/

interface_declaration_wrapper
	:	interface_declaration -> ^(INTERFACE interface_declaration);
	
interface_declaration
	:	'@interface'  name  interface_category? (super_class)?
		(annotated_block | interface_methods | interface_fields)*
		
		'@end';
		
annotated_block
	:	L_FBR 
		annotated_block_internal*
		R_FBR
	;
		
annotated_block_internal
	:	block_annotation 
	| 	public_block_internal 
	;
		
block_annotation
	:	'@public'
	|	'@private'
	;			
		
public_block_internal
	:	simple_field_declaration
	|	union_declaration
	;
	
simple_field_declaration
	:	field_type name  SEMICOLON
	;	
	
union_declaration
	:	'union'  L_FBR 
			simple_field_declaration+
		R_FBR  ID  SEMICOLON
	;	
		
super_class
	:	COLON  ID  -> ^(SUPER_CLASS ID);		

interface_category
	:	L_BR  ID?  R_BR -> ^(CATEGORY L_BR  ID?  R_BR);				
	
interface_fields
	:	interface_field_wrapper+;
	
interface_field_wrapper
	:	interface_field -> ^(FIELD interface_field);	
	
interface_field
	:	property_prefix? interface_field_declaration;	
	
interface_field_declaration
	:	field_type name SEMICOLON;	
	
property_prefix
	:	'@property' L_BR property (COMMA property)* R_BR;	
	
property:	'readwrite'
	|	'assign'
	|	'copy'
	;	
	
interface_methods
	:	(enum_declaration | interface_method)+;		
	
interface_method
	:	method_header -> ^(METHOD method_header);	
	
method_header
	:	method_header_body SEMICOLON?;

method_header_body
	:	method_modifier_wrapper  method_type?  name  method_params? va_args_wrapper?;

va_args_wrapper
	:	va_args -> ^(VA_ARGS va_args);
	
va_args	:	COMMA  '...'  SEMICOLON?;	

method_header_body2
	:	method_type name  method_params2?;

method_header_body3
	:	method_params2;

method_modifier_wrapper
	:	method_modifier -> ^(MODIFIER method_modifier);

method_modifier
	:	'+' | '-';	
	
method_type
	:	L_BR  STRUCT_PREFIX? type_internal  generic? ASTERISK* R_BR -> ^(TYPE type_internal)
	| 	L_BR  CONST_PREFIX? known_types? ID* generic? ASTERISK* R_BR -> ^(TYPE ID+)
	;			
		
type_internal
	:	type_internal1 (L_KBR  R_KBR)?
	;		
		
type_internal1
	:	VOID
	|	ID
	|	'unsigned'
	;	
	
generic:	 L_LESS generic_internal L_MORE -> ^(GENERIC generic_internal);	

generic_internal
	:	 ID ASTERISK* (COMMA  ID  ASTERISK*)*;

method_params2
	:	L_BR  method_param_wrapper2  (COMMA  method_param_wrapper2)* R_BR
	;

method_param_wrapper2
	:	method_param2 -> ^(PARAM method_param2);
	
method_param2
	:	CONST_PREFIX? ENUM_PREFIX? STRUCT_PREFIX? 'unsigned'? field_type name? indexed?;
	
indexed	:	L_KBR NUMBER? R_KBR;
	
method_params
	:	method_param+ -> ^(PARAM method_param)+;	
	
method_param
	:	(prefix? | COLON) method_type? name
	;	
	
prefix	:	ID COLON -> ^(PREFIX ID COLON);

incomplete_prefix
	:	COLON -> ^(INCOMPLETE_PREFIX COLON);


/*------------------------------------------------------------------
 * TYPEDEF_STRUCT RULES
 *------------------------------------------------------------------*/
 
typedef_struct_declaration_wrapper
	:	typedef_struct_declaration -> ^(TYPEDEF_STRUCT typedef_struct_declaration)
	; 


typedef_struct_declaration
	: STRUCT_PREFIX name?
	L_FBR 
		typedef_struct_body+
	R_FBR  name? SEMICOLON?
	;
	
typedef_struct_body
	:	typedef_struct_declaration_element_wrapper
	;
	
typedef_struct_declaration_element_wrapper
	: struct_field -> ^(STRUCT_FIELD struct_field);
	
struct_field
	:	field_type name  (COMMA  name)* SEMICOLON	
	;

/*------------------------------------------------------------------
 * TYPEDEF RULES
 *------------------------------------------------------------------*/
 
typedef_declaration_wrapper
	:	typedef_declaration -> ^(TYPEDEF typedef_declaration)
	; 

enum_declaration
	:	typedef_declaration -> ^(ENUM typedef_declaration);	
	
typedef_declaration
	:  ENUM_PREFIX name? L_FBR  typedef_declaration_element_wrapper  (COMMA  typedef_declaration_element_wrapper)* R_FBR  name? SEMICOLON;
	
typedef_declaration_element_wrapper
	:	typedef_declaration_element -> ^(TYPEDEF_ELEMENT typedef_declaration_element)
	;

typedef_declaration_element
	: ID  (L_EQ  (ID| NUMBER))?
	| NUMBER L_EQ NUMBER
	;

/*------------------------------------------------------------------
 * FIELD RULES
 *------------------------------------------------------------------*/

field_declaration_wrapper
	:	field_declaration -> ^(FIELD field_declaration);

field_declaration
	:	field_modifier_wrapper  field_type name field_declaration_end;
	
field_declaration_end
	:	 value_set? SEMICOLON;
	
type_start_wrapper
	:	type_start -> ^(M_TYPE_START type_start);

type_start
	:	CONST_PREFIX? field_type name (L_KBR R_KBR)? type_end;
	
type_end:	field_declaration4 -> ^(FIELD field_declaration4)
	|	method_declaration4 -> ^(STATIC_METHOD method_declaration4)
	;	
	
field_declaration4
	:	 value_set? SEMICOLON
	;
	
method_declaration4
	:	 method_params2 
		block_wrapper
	;	

field_modifier_wrapper
	:	field_modifier -> ^(MODIFIER field_modifier);

field_modifier	
	:	'extern'
	;

field_type
	:	 field_type_internal -> ^(TYPE field_type_internal);	
	
field_type_internal
	:	type_internal generic? ASTERISK*;

/*------------------------------------------------------------------
 * COMMON RULES
 *------------------------------------------------------------------*/
	
name	:	name_internal -> ^(NAME name_internal)
	;	
	
name_internal
	:	ID 
	| 	'in'	
	| 	property
	;

value_set
	:	L_EQ  value -> ^(VALUE value);

value	:	ID | string_value | null_stmt | array | NUMBER | struct_init2;	

string_value
	:	STRING_LITERAL;
	
null_stmt	:	'nil' | 'Nil';

array	:	L_FBR  value  (COMMA  value)* R_FBR;

/*------------------------------------------------------------------
 * LEXER RULES
 *------------------------------------------------------------------*/

L_FBR	:	'{';
R_FBR	:	'}';
L_BR 	:	'(';
R_BR 	:	')';
L_KBR	:	'[';
R_KBR	:	']';
SEMICOLON
	:	';';
COLON	:	':';

L_LEFT_EQ	:	'<<=';
L_RIGHT_EQ	:	'>>=';

L_LESS_EQ
	:	'<=';
L_MORE_EQ
	:	'>=';

L_LEFT	:	'<<';
L_RIGHT	:	'>>';
L_LESS	:	'<';
L_MORE	:	'>';
DOT	:	'.';
COMMA	:	',';
L_NEQ	:	'!=';
L_NOT	:	'!';
L_EQ_EQ	:	'==';

L_PLUS_EQ	:	'+=';
L_MINUS_EQ	:	'-=';
L_MULT_EQ	:	'*=';
L_DIV_EQ	:	'/=';
L_PERC_EQ	:	'%=';
L_AND_EQ	:	'&=';
L_XOR_EQ	:	'^=';
L_OR_EQ		:	'|=';

L_EQ 	:	'=';
ASTERISK
	:	'*';
L_DIV	:	'/';
L_PERC	:	'%';
L_PLUS_PLUS
	:	'++';
L_MINUS_MINUS
	:	'--';
L_PLUS	:	'+';
L_MINUS	:	'-';	
	
L_OR_OR	:	'||';
L_AND_AND	
	:	'&&';
L_OR	:	'|';
L_AND	:	'&';
L_XOR	:	'^';
L_TILDA	:	'~';
SIZEOF	:	'sizeof';
L_QUESTION
	:	'?';

CONST_PREFIX
 	:	'const' ;
 	
ENUM_PREFIX
	:	'enum' ;

STRUCT_PREFIX
	:	'struct' ;

STATIC_PREFIX
	:	'static' ;

SINGLE_COMMENT
	:	'//' ~('\r' | '\n')* ('\r'? '\n')+ { skip(); };

MULTI_COMMENT options { greedy = false; }
 	: '/*' .* '*/' ('\r'? '\n')? { skip(); };

PREPROCESSOR_DECLARATION
	: 	'#' ~('\\' | '\r' | '\n')* ('\\' ('\r' | '\n')+ ~('\\' | '\r' | '\n')+)* ('\r' | '\n')+ { skip(); }; 	
	
NUMBER  : DIGIT+ ('u'|'U')?
	| DIGIT+ DOT DIGIT* ('f'|'F')?
	| DIGIT* DOT DIGIT+ ('f'|'F')?
	| ('0x'  (DIGIT|'A'..'F')*(DOT DIGIT+)?)?
	| DIGIT+ (DOT DIGIT+)? 'e' ('-'|'+')? DIGIT+;

WS : ( '\t' | ' ' | '\r' | '\n'| '\u000C')+  { $channel = HIDDEN; } ;

SINGLE_OP
	:	'continue'  SEMICOLON
	;
	
BREAK_WORD
	:	'break';	
		
VOID	:	'void';
		
ID	:	LETTER (DIGIT|LETTER)*;

//CHAR_LITERAL
//	:	'\'' LETTER '\'';

STRING_LITERAL2
	:	'"' (WS | ESC | ~('\r' | '\n' | '"'))* '"';

STRING_LITERAL3
	:	'\'' (WS | ESC | ~('\r' | '\n' | '\''))* '\'';

STRING_LITERAL : '`'? '@' STRING_LITERAL2;

fragment DIGIT  : '0'..'9' ;
fragment LETTER	: ('a'..'z' | 'A'..'Z' | '_'); 
fragment ESC
	:	'\\' '"' { setText("\\\""); }
	;