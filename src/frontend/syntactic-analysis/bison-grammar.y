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
	int suite;
	int moduleList;
	int module;
	int scope;
	int statementList;
	int statement;
	int function;
	int control;
	int ifControl;
	int elseControl;
	int forExpression;
	int tryException;
	int retryException;
	int assertion;
	int expression;
	int attributeList;
	int attribute;
	int variable;
	int lambda;
	int sequence;
	int actionList;
	int action;
	int parameters;	
	int literal;

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

%token <token> IF
%token <token> ELSE
%token <token> FOR
%token <token> WHILE

%token <token> ARROW
%token <token> VAR
%token <token> FUNCTION
%token <token> RETURN

%token <integer> INTEGER
%token <number> NUMBER
%token <string> STRING
%token <boolean> BOOLEAN

%token <name> NAME

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <suite> suite
%type <moduleList> moduleList
%type <module> module
%type <scope> scope
%type <statementList> statementList
%type <statement> statement
%type <function> function
%type <control> control
%type <ifControl> if
%type <elseControl> else
%type <forExpression> forExpression
%type <tryException> try
%type <retryException> retry
%type <assertion> assertion
%type <expression> expression
%type <attributeList> attributeList
%type <attribute> attribute
%type <variable> variable
%type <lambda> lambda
%type <sequence> sequence
%type <actionList> actionList
%type <action> action
%type <parameters> parameters
%type <literal> literal

// El símbolo inicial de la gramatica.
%start program

%%


program: statementList													{ $$ = ProgramGrammarAction(0); }
	|	 suite													{ $$ = ProgramGrammarAction(0);}
	;

suite:  SUITE OPEN_BRACE moduleList CLOSE_BRACE	   				{ } // suite -> Suite { moduleList }
	|	SUITE NAME OPEN_BRACE moduleList CLOSE_BRACE			{ } // suite -> Suite NAME { moduleList }
	;

moduleList: %empty 											{ } // moduleList -> %empty
	| moduleList module										{ } // moduleList -> moduleList module
	;

module: MODULE scope 								{ } // module -> Module scope
	| MODULE NAME scope 							{ } // module -> Module NAME scope
	| BEFORE_ALL scope 								{ } // module -> BeforeAll scope
	| AFTER_ALL NAME scope 							{ } // module -> AfterAll scope
	;


scope: OPEN_BRACE statementList CLOSE_BRACE  					{ } // scope -> { statementList }

statementList: %empty
	| statementList statement									{ } // statementList -> statementList statement
	; 

statement: expression SEMICOLON									{ } // statement -> expression ;
	| VAR NAME ASSIGNMENT_OPERATOR expression SEMICOLON			{ } // statement -> var NAME = expression ;
	| VAR NAME SEMICOLON										{ } // statement -> var NAME ;
	| NAME ASSIGNMENT_OPERATOR expression SEMICOLON				{ } // statement -> NAME = expression ;
	| function 													{ } // statement -> function
	| RETURN expression SEMICOLON								{ } // statement -> return expression ;
	| RETURN SEMICOLON											{ } // statement -> return ;
	| control													{ } 
	| assertion expression SEMICOLON							{ } // statement -> assertion expression ;
	; 

function: FUNCTION NAME OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS scope { } // function -> "function" NAME ( parameters ) scope
	| FUNCTION NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS scope { }  // function -> "function" NAME ( ) scope
	| FUNCTION OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS scope { }  // function -> "function" NAME ( ) scope
	| FUNCTION OPEN_PARENTHESIS CLOSE_PARENTHESIS scope { }  // function -> "function" NAME ( ) scope

control: if 																									    { }
	| FOR OPEN_PARENTHESIS forExpression SEMICOLON forExpression SEMICOLON forExpression CLOSE_PARENTHESIS scope    { }
	| WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS scope														{ }
	;

if: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS scope 				{ } // if -> "if" ( expression ) scope
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS scope else		{ } // if -> "if" ( expression ) scope else
	;

else: ELSE if 															{ } // else -> "else" if
	| ELSE scope													  	{ } // else -> "else" scope
	;

forExpression: %empty 													{ }
	| expression														{ } // forExpression -> expression
	| variable ASSIGNMENT_OPERATOR expression							{ } // forExpression -> variable = expression
	;

try: TRY scope retry CATCH OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS scope												{ } // try -> "try" scope retry "catch" ( NAME ) scope
	| TRY scope retry CATCH OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS scope FINALLY scope									{ } // try -> "try" scope retry "catch" ( NAME ) scope "finally" scope
	;

retry: %empty																			{ } 
	| RETRY OPEN_PARENTHESIS NAME COMMA INTEGER CLOSE_PARENTHESIS scope					{ } // retry -> "retry" ( NAME , INTEGER ) scope
	| RETRY OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS scope								{ } // retry -> "retry" ( NAME ) scope

assertion: ASSERT_TRUE											{ } // assertion -> assert_true	
	| ASSERT_FALSE												{ } // assertion -> assert_false
	| ASSERT_EQUALS												{ } // assertion -> assert_equals
	| ASSERT_NOT_EQUALS											{ } // assertion -> assert_not_equals
	;	

expression: literal 											         			{ } // expression -> literal
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS									{ } // variable -> ( variable )
	| variable																		{ } // expression -> NAME
	| OPEN_PARENTHESIS NAME ASSIGNMENT_OPERATOR expression CLOSE_PARENTHESIS		{ } // expression -> ( NAME = expression )
	| expression BINARY_OPERATOR expression											{ } // expression -> expression BINARY_OPERATOR expression
	| UNARY_OPERATOR expression														{ } // expression -> UNARY_OPERATOR expression
	| XPATH_OPERATOR STRING CLOSE_PARENTHESIS										{ } // expression -> XPATH_OPERATOR  
	| OPEN_BRACKET CLOSE_BRACKET													{ } // expression -> [ ]
	| OPEN_BRACKET parameters CLOSE_BRACKET											{ } // expression -> [ parameters ] 
	| OPEN_BRACE attributeList CLOSE_BRACE											{ } // expression -> { attributeList } 
	| lambda
	;

attributeList: %empty	
	| attributeList COMMA attribute													{ } // attributeList -> attributeList , attribute
	| attribute																		{ } // attributeList -> attribute
	;

attribute: NAME COLON expression													{ } // attribute -> NAME : expression

variable: NAME 																		{ } // variable -> NAME
	| variable OPEN_BRACKET expression CLOSE_BRACKET								{ } // variable -> NAME [ expression ]
	| variable DOT variable															{ } // variable -> variable . variable	
	| variable OPEN_PARENTHESIS CLOSE_PARENTHESIS									{ } // variable -> NAME ()	
	| variable OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS						{ } // variable -> NAME ( parameters )	
	;

lambda: OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS ARROW expression				{ } // lambda -> ( parameters ) -> expression
	| OPEN_PARENTHESIS CLOSE_PARENTHESIS ARROW expression							{ } // lambda -> () -> expression
	| OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS ARROW scope						{ } // lambda -> ( parameters ) -> scope
	| OPEN_PARENTHESIS CLOSE_PARENTHESIS ARROW scope								{ } // lambda -> () -> scope
	;

sequence: actionList																{ } // sequence -> actionList
	;

actionList: actionList action														{ } // actionList -> actionList action
	| %empty																		{ } // actionList -> %empty
	;

action: KEY 																		{ } // action -> KEY
	| KEY_DOWN KEY	 																{ } // action -> + KEY
	| KEY_UP KEY																	{ } // action -> - KEY
	| STRING	 																	{ } // action -> STRING
	;

parameters: parameters COMMA expression													{ } // parameters -> parameters , expression
	| expression																	{ } // parameters -> NAME
	;

literal: INTEGER | NUMBER | STRING | BOOLEAN							            { } // literal -> INTEGER | STRING | BOOLEAN
	;
%%
