grammar Preprocessor;

options {
  output = AST;
  k = 4;
}

tokens {
	T_DEFINE;
	T_REPLACE;
	T_ID;
	T_IN_BRACKETS;
	T_PARAM;
	T_NAME;
	T_BACKSLASH;
	T_IMPORT;
	T_PATH;
	T_BLOCK;
	T_IF_DEFINE;
	T_IF_NOT_DEFINE;
	T_IF_SIMPLE;
	T_IF_MAIN;
	T_IF_ELSE;
	T_EXPRESSION;
	T_EX_AND;
	T_EX_OR;
	T_EX_SNGL;
	T_EX_MATH;
	T_EX_NOT;
	T_EX_COND;
	T_COND_OP;
	T_INCLUDE;
	T_EX_DEF;
	T_EX_OP;
}

@header {
package ru.andremoniy.objctojavacnv.antlr.output;
}
@lexer::header {
package ru.andremoniy.objctojavacnv.antlr.output;
}

code	:	code_fragment+
		EOF
	;

smth_else
	:	~(DEFINE | IMPORT | IF | ELSE | ENDIF | IFDEF | IFNDEF | ELIF)+;
code_fragment
	:	define_directive
	|	import_declaration
	|	if_simple_check_wrapper
	|	if_define_block_wrapper
	|	if_not_define_block_wrapper
	|	smth_else
	;
	
ret	:	RET+;

if_not_define_block_wrapper
	:	if_not_define_block -> ^(T_IF_NOT_DEFINE if_not_define_block);
	
if_not_define_block
	:	(ifndef1 | ifndef2) ret
		main_code_block
		(ELSE ret 
		else_code_block)? 
		ENDIF
	;

ifndef1	:	IFNDEF name;

ifndef2	:	IF  EXC  DEFINED L_BR  name R_BR;

if_simple_check_wrapper
	:	if_simple_check -> ^(T_IF_SIMPLE if_simple_check);
	
if_simple_check
	:	IF expression_wrapper ret
		main_code_block?
		(ELIF  expression_wrapper ret
		else_code_block)*
		(ELSE ret 
		else_code_block)? 
		ENDIF
	;
	
expression_wrapper
	:	expression -> ^(T_EXPRESSION expression);	
	
if_define_block_wrapper
	:	if_define_block -> ^(T_IF_DEFINE if_define_block);	
	
if_define_block
	:	(ifdef1 | ifdef2) ret
		main_code_block?
		(ELSE ret 
		else_code_block)? 
		ENDIF
	;

ifdef1	:	IF DEFINED L_BR name R_BR;

ifdef2	:	IFDEF  name;

main_code_block
	:	code_block -> ^(T_IF_MAIN code_block);

else_code_block
	:	code_block -> ^(T_IF_ELSE code_block);

code_block
	:	code_fragment+ -> ^(T_BLOCK code_fragment+);
	
inside_block
	:	~(ELSE | ENDIF | ELIF)*;

name	:	ID -> ^(T_NAME ID);

import_declaration
	:	(IMPORT | INCLUDE) import_end;
	
import_end
	:	import_internal 
	| 	import_external;
	
import_internal
	:	import_internal_string -> ^(T_IMPORT import_internal_string);
	
import_internal_string
	:	L_UBR  filename ('/' filename)*  R_UBR;
	
filename:	ID (MINUS ID)* ('.' ID)?;

filename2
	:	ID ((MINUS | PLUS) ID)* ('.' ID)?;

import_external
	:	import_external2 -> ^(T_INCLUDE import_external2);
	
import_external2
	:	QUOTE filename2 QUOTE;	
	
define_directive
	:	DEFINE  id_wrapper  mln_end? replace_wrapper? -> ^(T_DEFINE DEFINE  id_wrapper replace_wrapper?)
	;
	
mln_end	:	BACKSLASH ret;
	
id_wrapper
	:	name_wrapper in_brackets_wrapper? -> ^(T_ID name_wrapper in_brackets_wrapper?);	
	
name_wrapper
	:	ID -> ^(T_NAME ID);	
	
in_brackets_wrapper
	:	in_brackets -> ^(T_IN_BRACKETS in_brackets);	
	
in_brackets
	:	L_BR param_wrapper (','  param_wrapper)* R_BR
	;
	
param_wrapper
	:	param_common
	|	POINTS3
	;	

param_common
	:	ID -> ^(T_PARAM ID);
		
replace_wrapper
	:	replace -> ^(T_REPLACE replace);	
	
replace	:	replace_internal (backslash ret replace_internal)*;
	
backslash
	:	BACKSLASH -> ^(T_BACKSLASH BACKSLASH);
	
replace_internal
	:	('\'#\'' | ~(BACKSLASH | RET)+);

expression
	:	ex_or -> ^(T_EX_OR ex_or);

ex_or	:	ex_and_wrp ('||' expression_wrapper)*;
	
ex_and_wrp
	:	ex_and -> ^(T_EX_AND ex_and);	
	
ex_and	:	ex_not_wrp ('&&' expression_wrapper)*;

ex_not_wrp
	:	ex_not -> ^(T_EX_NOT ex_not);

ex_not	:	EXC? ex_cond;
	
ex_cond_wrp
	:	ex_cond -> ^(T_EX_COND ex_cond);	
	
ex_cond	:	ex_sngl_wrp (cond_op_wrp ex_sngl_wrp)?;	

cond_op_wrp
	:	cond_op -> ^(T_COND_OP cond_op);
	
cond_op	:	'==' | '<=' | '>=' | L_UBR | R_UBR;
	
ex_sngl_wrp
	:	ex_sngl -> ^(T_EX_SNGL ex_sngl);	

ex_sngl	:	math_expr_wrp 
	|	defined_wrp;
	
defined_wrp
	:	defined_expr -> ^(T_EX_DEF defined_expr);	
	
defined_expr
	:	DEFINED? L_BR expression_wrapper  mln_end? R_BR  mln_end?;	
	
math_expr_wrp
	:	math_expr -> ^(T_EX_MATH math_expr);	
		
math_expr
	:	single_expr (op_wrp single_expr)*;
	
op_wrp	:	op -> ^(T_EX_OP op);	
	
op	:	ASTERISK | PLUS | MINUS;

single_expr
	:	(PLUS | MINUS)? single_expr2;	

single_expr2
	:	name
	|	const_expr
	;

const_expr
	:	NUMBER;

COMMENT	: 	'/*' ( options {greedy=false;} : . )* '*/' {  $channel = HIDDEN;  };
    
SINGLE_COMMENT
	:	'//' ~('\n'|'\r')* {  $channel = HIDDEN;  };
	
IFNDEF	:	'#ifndef';	
IF	:	'#if';
ELIF	:	'#elif';
IFDEF	:	'#ifdef';
ELSE	:	'#else';
ENDIF	:	'#endif';
DEFINE	:	'#define';
INCLUDE	:	'#include';
IMPORT	:	'#import';
DEFINED	:	'defined';

BACKSLASH
	:	'\\';
POINTS3	:	'...';

EXC	:	'!';
PLUS	:	'+';
MINUS	:	'-';
L_BR	:	'(';
R_BR	:	')';
L_UBR	:	'<';
R_UBR	:	'>';
ASTERISK:	'*';
QUOTE	:	'"';

RET	:	'\r' | '\n';

ID	:	LETTER (DIGIT|LETTER)*;

NUMBER  : 	DIGIT (DIGIT | '.')* 'L'?;

//STRING_LITERAL2
//	:	QUOTE ( options {greedy=false;} : . )* QUOTE;
//STRING_LITERAL
//	: 	'@' STRING_LITERAL2;

SPECIAL_CHARS
	:	'%'|'?'|'|'|'&'|';'|'['|']'|'{'|'}'|'@'|':'|'/'|'#'|'=';

WS  	:	('\u000C' | ' ' | '\t')+ { $channel = HIDDEN; };

fragment DIGIT  : '0'..'9' ;
fragment LETTER	: ('a'..'z' | 'A'..'Z' | '_'); 


