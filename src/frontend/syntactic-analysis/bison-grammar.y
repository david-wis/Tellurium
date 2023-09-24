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
	int expression;
	int constant;

	// Terminales.
	token token;
	int integer;
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

%token <token> OPERATOR
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
/* %type <expression> expression */
%type <constant> constant

// El símbolo inicial de la gramatica.
%start program

%%

program: constant													{ $$ = ProgramGrammarAction(0); }
	|	 MODULE constant											{ $$ = ProgramGrammarAction(0);}
	;


constant: INTEGER													{}
	;

%%
