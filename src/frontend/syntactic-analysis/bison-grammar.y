%{

#include "bison-actions.h"

%}

%code requires {
	#include "../types.h"
}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int program;
	int literal;
	int suite;
	int module;
	int moduleList;
	int function;
	int scope;
	int statement;
	int expression;
	int sequence;


	// Terminales.
	token token;
	int integer;
	char * number;
	char * string;
	char * operator;
	char * name;
	bool_t boolean;
	actionkey_t key;
}


// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.


%token <token> KEY_DOWN
%token <token> KEY_UP
%token <token> KEY

%token <token> BINARY_OPERATOR
%token <token> UNARY_OPERATOR
%token <token> ASSIGNMENT_OPERATOR
%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> COMMA
%token <token> SEMICOLON
%token <token> COLON
%token <token> DOT
%token <token> XPATH_OPERATOR

%token <token> MODULE
%token <token> SUITE
%token <token> BEFORE_ALL
%token <token> AFTER_ALL

%token <token> ASSERT_TRUE
%token <token> ASSERT_FALSE
%token <token> ASSERT_EQUALS
%token <token> ASSERT_NOT_EQUALS

%token <token> RETRY
%token <token> TRY
%token <token> CATCH
%token <token> FINALLY

%token <token> VAR
%token <token> FUNCTION
%token <token> RETURN

%token <integer> INTEGER
%token <string> STRING
%token <boolean> BOOLEAN

%token <name> NAME

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program

%type <literal> literal
%type <suite> suite
%type <module> module
%type <moduleList> moduleList
%type <function> function
%type <scope> scope
%type <statement> statement
%type <expression> expression


// El símbolo inicial de la gramatica.
%start program

%%



program: scope													{ $$ = ProgramGrammarAction(0); }
	|	 suite													{ $$ = ProgramGrammarAction(0);}
	;


suite:  SUITE OPEN_BRACE moduleList CLOSE_BRACE	   				{ } // suite -> Suite { moduleList }
	|	 SUITE NAME OPEN_BRACE moduleList CLOSE_BRACE			{ } // suite -> Suite NAME { moduleList }
	;


moduleList: moduleList module									{ } // moduleList -> moduleList module
	|   module													{ } // moduleList -> module
	;


module: MODULE OPEN_BRACE scope CLOSE_BRACE						{ } // module -> Module { scope }
	| MODULE NAME OPEN_BRACE scope CLOSE_BRACE					{ } // module -> Module NAME { scope }
	| BEFORE_ALL OPEN_BRACE scope CLOSE_BRACE					{ } // module -> BeforeAll { scope }
	| AFTER_ALL NAME OPEN_BRACE scope CLOSE_BRACE				{ } // module -> AfterAll { scope }
	;

function: FUNCTION NAME OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS OPEN_BRACE scope CLOSE_BRACE	{ } // function -> "function" NAME ( parameters ) { scope }

scope: scope statement 											{ } // scope -> scope statement
	|  statement												{ } // scope -> statement
	;

statement: expression SEMICOLON									{ } // statement -> expression ;
	| VAR NAME ASSIGNMENT_OPERATOR expression SEMICOLON			{ } // statement -> var NAME = expression ;
	| VAR NAME SEMICOLON										{ } // statement -> var NAME ;
	| NAME ASSIGNMENT_OPERATOR expression SEMICOLON				{ } // statement -> NAME = expression ;
	| function 													{ } // statement -> function
	| RETURN expression SEMICOLON								{ } // statement -> return expression ;
	| RETURN SEMICOLON											{ } // statement -> return ;
	| assertion expression SEMICOLON							{ } // statement -> assertion expression ;
	; 

assertion: ASSERT_TRUE											{ } // assertion -> assert_true	
	| ASSERT_FALSE												{ } // assertion -> assert_false
	| ASSERT_EQUALS												{ } // assertion -> assert_equals
	| ASSERT_NOT_EQUALS											{ } // assertion -> assert_not_equals
	;	

expression: literal 											         			{ } // expression -> literal
	| NAME																			{ } // expression -> NAME
	| OPEN_PARENTHESIS NAME ASSIGNMENT_OPERATOR expression CLOSE_PARENTHESIS		{ } // expression -> ( NAME = expression )
	| expression BINARY_OPERATOR expression											{ } // expression -> expression BINARY_OPERATOR expression
	| UNARY_OPERATOR expression														{ } // expression -> UNARY_OPERATOR expression
	| NAME  OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS							{ } // expression -> NAME ( parameters )
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS									{ } // expression -> ( expression )
	;

parameters: %empty 																	{ }
 	| parameters COMMA expression													{ } // parameters -> parameters , expression
	| expression																	{ } // parameters -> NAME
	;


literal: INTEGER | STRING | BOOLEAN							                    	{ } // literal -> INTEGER | STRING | BOOLEAN
	;
%%
