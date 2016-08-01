grammar Objch;


options {
  output = AST;
  k = 4;
}

tokens {
	TYPEDEF;
	TYPEDEF_NAME;
	TYPEDEF_ELEMENT;
	INTERFACE_NAME;
	SUPERCLASS_NAME;
	FIELDS;
	FIELD;
	TYPE;
	FIELD_NAME;
	INTERFACE;
	METHOD;
	METHOD_TYPE;
	METHOD_NAME;
	METHOD_PARAMS;
	METHOD_PARAM;
	PARAM_NAME;
	PARAM_PREFIX;
	STATIC_METHOD;
	EXTERN;
	MODIFIER;
	GROUP_MODIFIER;
	PROTOCOL;
	NAME;
	CATEGORY;
	OLD_NAME;
	ENUM;
	STRUCT;
	VALUE;
	INIT_DECLARATION;
	ARCHIVE_DECLARATION;
	UNION;
	T_STATIC;
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
		code_internal*
	;
	
code_internal
	:	class_declaration 
	| 	typedef_declaration_wrapper 
	| 	enum_wrapper
	|	protocol_declaration_wrapper 
	|	interface_declaration_wrapper 
	|	extern_declarations
	|	typedef_struct
	|	declarations
	;	
		
enum_wrapper
	:	enum_declaration ';' -> ^(ENUM enum_declaration);	
		
extern_declarations
	:	extern_declatation+ -> ^(EXTERN extern_declatation)+;	
	
declarations
	:	optional_prefix? method_declaration_wrapper
	|	property_prefix? optional_prefix? field_declaration_wrapper
	|	ns_inline ~'{'+ ~'}'+ '}' // do not parse such inline methods yet
	;
	
static_prefix
	:	'static' -> ^(T_STATIC 'static');	
	
method_declaration_wrapper
	:	method_declaration -> ^(METHOD method_declaration);
	
field_declaration_wrapper
	:	field_declaration -> ^(FIELD field_declaration);
	
property_prefix
	:	'@property' '(' property_prefix_param (',' property_prefix_param)* ')';	
	
optional_prefix
	:	'@optional';
	
ns_inline
	:	'NS_INLINE';	
	
property_prefix_param
	:	'retain'
	|	'readwrite'
	|	'readonly'
	|	'nonatomic'
	|	'assign'
	|	'copy'
	;	

interface_declaration_wrapper
	:	interface_declaration -> ^(INTERFACE interface_declaration);
	
protocol_declaration_wrapper
	:	protocol_declaration -> ^(PROTOCOL protocol_declaration);

typedef_declaration_wrapper
	:	typedef_declaration -> ^(TYPEDEF typedef_declaration);

typedef_declaration
	: 'typedef' typedef_internal (typedef_name | func_pointer)? ';'?;
	
func_pointer
	:	'(' '*'? ID ')' '(' (ID ('*' ID)? (',' ((ID ('*' ID)?) | '...') )*)? ')'
	;
	
typedef_internal
	:	typedef_enum_wrapper2 
	| 	typedef_rename 
	| 	typedef_struct_wrapper
	|	union_declaration_wrapper
	;
	
union_declaration_wrapper
	:	union_declaration -> ^(UNION union_declaration);	
	
union_declaration
	:	'union' 
		'{'
			typedef_struct+
		'}' union_name?
	;	
	
union_internal
	:	field_declaration_wrapper
	|	typedef_struct_wrapper
	;	
typedef_rename
	:	ID '*'? -> ^(OLD_NAME ID)
	;		
	
typedef_enum_wrapper2
	:	enum_declaration -> ^(ENUM enum_declaration);		
	
enum_declaration
	:	'enum' struct_name? '{' typedef_declaration_element_wrapper (',' typedef_declaration_element_wrapper)* ','? '}';	
	
typedef_struct_wrapper
	:	typedef_struct -> ^(STRUCT typedef_struct);
	
typedef_struct
	:	'struct' typedef_name?
		'{'
			struct_field_wrapper+
		'}' (struct_name (',' struct_name)*)? ';'?
	;
	
struct_name
	:	ID -> ^(NAME ID);
	
struct_field_wrapper
	:	struct_field -> ^(FIELD struct_field);	

struct_field
	:	struct_field1
	|	struct_field2
	;
	
struct_field1
	:	type_declaration field_name (',' field_name)* ';';
	
struct_field2
	:	typedef_struct;	
	
typedef_name
	: ID -> ^(TYPEDEF_NAME ID);

typedef_declaration_element_wrapper
	:	typedef_declaration_element -> ^(TYPEDEF_ELEMENT typedef_declaration_element)
	;
	
typedef_declaration_element
	: ID ('=' element_value)?
	| element_value ('=' element_value)?
	;
	
element_value
	:	simple_expression -> ^(VALUE simple_expression)
	;	
	
simple_expression
	:	simple_expression2 ( op2 simple_expression2)*
	;
	
simple_expression2
	:	ID
	|	NUMBER
	|	'(' simple_expression ')';	
	
op2	:	'|' | '&~' | '&' | '<<' | '>>';

class_declaration
	: '@class' ID (',' ID)* ';';
	
protocol_declaration
	:	'@protocol' protocol_name generic?
		 protocol_end
	;	

protocol_end
	:	protocol_end1
	|	protocol_end2;

protocol_end1
	:	declarations*
		'@end'
	;
	
protocol_end2
	:	';'
	;
	
	
generic	:	'<' generic_internal '>';

generic_internal
	:	type_declaration (',' type_declaration)*;
	
protocol_name
	:	ID -> ^(NAME ID);		
	
interface_declaration
	: '@interface' interface_name (':' superclass_name)?
	interface_body?
	declarations*
	'@end'
	;
	
interface_body
	:
	'{'
		interface_body_item*
	'}'
	;
	
interface_body_item
	:	group_modifier_wrapper simple_fields_declaration?
	|	simple_fields_declaration
	|	union_declaration_wrapper ';'
	;

union_name
	:	ID -> ^(NAME ID);	
	
group_modifier_wrapper
	:	group_modifier -> ^(GROUP_MODIFIER group_modifier);	
	
group_modifier
	:	'@private' | '@protected' | '@public';	
		
interface_name
	:	ID category? -> ^(INTERFACE_NAME ID category?);

category:	'(' ID ')' -> ^(CATEGORY ID);

superclass_name
	:	ID generic? -> ^(SUPERCLASS_NAME ID generic?);
	
simple_fields_declaration
	:	field_declaration_wrapper+ -> ^(FIELDS field_declaration_wrapper+);
	
field_declaration
	:	'__weak'? static_prefix? 'inline'? type_declaration ((field_name (classical_method_params | (',' field_name)*)) | func_pointer) ';';
	
classical_method_params
	:	'(' (classical_param (',' classical_param)*)? ')' attribute?
	;	
	
attribute
	:	'__attribute__' '(' attribute_internal ')';	
	
attribute_internal
	:	attribute_internal1;
	
attribute_internal1
	:	'(' 'format' '(' (_format_item (',' _format_item)*)? ')' ')';		

_format_item
	:	ID
	|	NUMBER;	
classical_param
	:	type_declaration ID ('[' NUMBER? ']')*
	|	'...';		
	
type_declaration
	:	'const'? 'enum'? 'typedef'? 'struct'? (('unsigned' type_dec?) | type_dec) 'const'? generic? '*'? -> ^(TYPE type_dec generic? '*'?);	

type_dec:	type_dec_internal ('[' ']')* 
	;
	
type_dec_internal
	:	knownTypes 
	| 	ID
	;

knownTypes
	:	'int'
	|	'long'+ 'int'?
	;
	
field_name
	:	ID array_size? (field_crap)? -> ^(FIELD_NAME ID array_size?);			

array_size
	:	'[' NUMBER? ']';

field_crap
	:	':' NUMBER;
	
method_declaration
	:	method_modifier_wrapper method_declaration_variants;
	
method_modifier_wrapper
	:	method_modifier -> ^(MODIFIER method_modifier);
	
method_modifier
	:	'+' | '-';
	
method_declaration_variants
	:
	//	init_declaration_wrapper 
//	|	archive_declaration_wrapper
	 	method_declaration_common;	

method_declaration_common
	:	('(' type_declaration ')')? method_name method_params? ';';	

/*
init_declaration_wrapper
	:	init_declaration -> ^(INIT_DECLARATION init_declaration);
	
archive_declaration_wrapper
	:	archive_declaration -> ^(ARCHIVE_DECLARATION archive_declaration);		
	
init_declaration
	:	init_name method_params? ';';
	
archive_declaration
	:	archive_name method_params? ';';	

init_name
	:	init_variants -> ^(METHOD_NAME init_variants);
	
archive_name
	:	archive_variants -> ^(METHOD_NAME archive_variants);

init_variants
	:	'init'
	|	'initWithType'
	|	'initWithContentsOfFile'
	|	'initWithIconFamilyHandle'
	|	'initWithIconOfFile'
	|	'initWithSystemIcon'
	|	'initWithThumbnailsOfImage'	
	|	'initWithScrap'
	;	

archive_variants
	:	'archiveToPropertyListForRootObject'
	|	'unarchiveFromPropertyListFormat'
	|	'archiveFromPropertyListFormat'
	;	
	*/
	
method_name
	:	ID -> ^(METHOD_NAME ID)
//	|	init_variants -> ^(METHOD_NAME init_variants)
	|	property_prefix_param -> ^(METHOD_NAME property_prefix_param); // uuuuur!
	
method_params
	:	method_param+ -> ^(METHOD_PARAMS method_param+);

method_param
	:	prefix? ':' ('(' type_declaration ')')? param_name -> ^(METHOD_PARAM prefix? ':' '(' type_declaration ')' param_name);	
	
param_name
	:	ID -> ^(PARAM_NAME ID)
	|	'format' -> ^(PARAM_NAME 'format');
	
prefix	:	ID -> ^(PARAM_PREFIX ID);

extern_declatation
	:	'extern' field_declaration;
/*------------------------------------------------------------------
 * LEXER RULES
 *------------------------------------------------------------------*/
 
SINGLE_COMMENT
	:	'//' ~('\r' | '\n')* ('\r'? '\n')+ { skip(); };

MULTI_COMMENT options { greedy = false; }
 	: '/*' .* '*/' ('\r'? '\n')? { skip(); };

PREPROCESSOR_DECLARATION
	: '#' ~('\r' | '\n')* ('\r' | '\n')+ { skip(); }; 	

NUMBER  : '-'? DIGIT+ 
	| '0x' ('A'|'B'|'C'|'D'|'E'|'F'|DIGIT)+;

WHITESPACE : ( '\t' | ' ' | '\r' | '\n'| '\u000C' )+  { $channel = HIDDEN; } ;

ID	: LETTER (DIGIT|LETTER)*;

FILE_NAME 	: (LETTER|NUMBER)+ ('.'(LETTER|NUMBER)+)?;

fragment DIGIT  : '0'..'9' ;
fragment LETTER	: ('a'..'z' | 'A'..'Z' | '_');  
  
