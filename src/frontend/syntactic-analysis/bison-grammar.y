%{

#include "bison-actions.h"

%}

%code requires {
	#include "../../shared/types.h"
}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	Program * program;

	SuiteNode * suite;
	ModuleListNode * moduleList;
	ModuleNode * module;
	ScopeNode * scope;
	StatementListNode * statementList;
	StatementNode * statement;

	FunctionNode * function;
	ParameterDefinitionNode * parameterDefinitions;

	ControlNode * control;
	IfControlNode * ifControl;
	ElseControlNode * elseControl;
	ForExpressionNode * forExpression;

	TryControlNode * tryControl;
	RetryControlNode * retryControl;
	
	ExpressionNode * expression;
	LambdaNode * lambda;
	ObjectNode * object;

	OperationNode * operation;
	UnaryOperator unaryOperator;
	BinaryOperator binaryOperator;
	OperandNode * operand;
	
	AttributeListNode * attributeList;
	AttributeNode * attribute;
	VariableNode * variable;

	SequenceNode * sequence;
	ActionListNode * actionList;
	ActionNode * action;

	ParametersNode * parameters;	
	LiteralNode * literal;
	ExceptionSetNode * exceptionSet;


	// Terminales.
	token token;
	int integer;
	char * number;
	char * string;
	char * operator;
	char * name;
	bool_t boolean;
	actionkey_t key;
	assertion_t assertionType;
	cmp_assertion_t cmpAssertionType;
	cardinality_t cardinality;
	variable_scope_t variableScope;
}


// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.

%token <token> BEGIN_SEQUENCE
%token <token> END_SEQUENCE
%token <token> KEY

%token <token> BINARY_OPERATOR
%token <token> PIPE
%token <token> PLUS
%token <token> MINUS
%token <token> MULTIPLICATION
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

%token <token> ASSERT
%token <token> ASSERT_COMPARE

%token <token> RETRY
%token <token> TRY
%token <token> CATCH
%token <token> FINALLY

%token <token> IF
%token <token> ELSE
%token <token> FOR
%token <token> WHILE

%token <token> ARROW
%token <token> VARIABLE
%token <token> FUNCTION
%token <token> RETURN

%token <integer> INTEGER
%token <number> NUMBER
%token <string> STRING
%token <boolean> BOOLEAN
%token <token> NULL_LITERAL
%token <token> UNDEFINED
%token <token> NAN

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
%type <parameterDefinitions> parameterDefinitions
%type <control> control
%type <ifControl> if
%type <elseControl> else
%type <forExpression> forExpression
%type <tryControl> try
%type <retryControl> retry
%type <expression> expression
%type <operation> operation
%type <binaryOperator> binaryOperator
%type <unaryOperator> unaryOperator
%type <operand> operand
%type <lambda> lambda
%type <object> object
%type <attributeList> attributeList
%type <attribute> attribute
%type <variable> variable
%type <sequence> sequence
%type <actionList> actionList
%type <action> action
%type <parameters> parameters
%type <literal> literal
%type <exceptionSet> exceptionSet

// De menor a mayor precedencia. 
%right ASSIGNMENT_OPERATOR
%left PLUS MINUS
%left MULTIPLICATION
%left BINARY_OPERATOR 

// El símbolo inicial de la gramatica.
%start program

%%

program: statementList											{ $$ = ProgramGrammarAction(0); }
	|	 suite													{ $$ = ProgramGrammarAction(0);}
	;

suite:  SUITE OPEN_BRACE moduleList CLOSE_BRACE	   				{ } // suite -> Suite { moduleList }
	|	SUITE NAME OPEN_BRACE moduleList CLOSE_BRACE			{ } // suite -> Suite NAME { moduleList }
	;

moduleList: %empty 												{ } 
	| moduleList module											{ } // moduleList -> moduleList module
	;

module: MODULE scope 											{ } // module -> Module scope
	| MODULE NAME scope 										{ } // module -> Module NAME scope
	| BEFORE_ALL scope 											{ } // module -> BeforeAll scope
	| AFTER_ALL scope 											{ } // module -> AfterAll scope
	;

scope: OPEN_BRACE statementList CLOSE_BRACE  					{ } // scope -> { statementList }
	;

statementList: %empty 											{ } 
	| statementList statement									{ } // statementList -> statementList statement
	; 

statement: expression SEMICOLON									{ } // statement -> expression ;
	| VARIABLE NAME ASSIGNMENT_OPERATOR expression SEMICOLON	{ } // statement -> var NAME = expression ;
	| variable ASSIGNMENT_OPERATOR expression SEMICOLON			{ } // statement -> NAME = expression ;
	| VARIABLE NAME SEMICOLON									{ } // statement -> var NAME ;
	| RETURN expression SEMICOLON								{ } // statement -> return expression ;
	| RETURN SEMICOLON											{ } // statement -> return ;
	| ASSERT_COMPARE expression COMMA expression SEMICOLON		{ } // statement -> assert expression , expression ; 
	| ASSERT expression SEMICOLON								{ } // statement -> assert expression ; 
	| function 													{ } // statement -> function
	| control													{ } // statement -> control
	; 

function: FUNCTION NAME OPEN_PARENTHESIS parameterDefinitions CLOSE_PARENTHESIS scope 	{ } // function -> "function" NAME ( parameters ) scope
	| FUNCTION NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS scope 							{ }  // function -> "function" NAME ( ) scope
	;

parameterDefinitions: parameterDefinitions COMMA NAME								{ } // parameterDefinitions -> parameterDefinitions , NAME
	| NAME																			{ } // parameterDefinitions -> NAME
	;
	
control: if 																									    { } // control -> if
	| FOR OPEN_PARENTHESIS forExpression SEMICOLON forExpression SEMICOLON forExpression CLOSE_PARENTHESIS scope    { } // control -> "for" ( forExpression ; forExpression ; forExpression ) scope
	| WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS scope														{ } // control -> "while" ( expression ) scope
	| try																											{ } // control -> try
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
	| VARIABLE NAME ASSIGNMENT_OPERATOR expression						{ } // forExpression -> var variable = expression
	;

try: TRY scope retry CATCH OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS scope						{ } // try -> "try" scope retry "catch" ( NAME ) scope
	| TRY scope retry CATCH OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS scope FINALLY scope			{ } // try -> "try" scope retry "catch" ( NAME ) scope "finally" scope
	;

retry: %empty																					{ } 
	| RETRY OPEN_PARENTHESIS exceptionSet COMMA INTEGER CLOSE_PARENTHESIS scope					{ } // retry -> "retry" ( NAME , INTEGER ) scope
	| RETRY OPEN_PARENTHESIS exceptionSet CLOSE_PARENTHESIS scope								{ } // retry -> "retry" ( NAME ) scope
	;

exceptionSet: NAME																	{ } // exceptionSet -> NAME 
	| exceptionSet PIPE NAME														{ } // exceptionSet -> exceptionSet exception
	;
	
expression: operation																{ } // expression -> operation 
	| lambda																		{ } // expression -> lambda
	;

operation: operand 																	{ } // operation -> operand
	| unaryOperator operand															{ } // operation -> UNARY_OPERATOR operand
	| operation binaryOperator operand												{ } // operation -> operation BINARY_OPERATOR operand
	;

unaryOperator: MINUS																{ } // unaryOperator -> -
	| UNARY_OPERATOR																{ } // unaryOperator -> UNARY_OPERATOR
	;

binaryOperator: PLUS																{ } // binaryOperator -> +
	| MINUS																			{ } // binaryOperator -> -
	| MULTIPLICATION																{ } // binaryOperator -> *
	| BINARY_OPERATOR																{ } // binaryOperator -> BINARY_OPERATOR
	;

operand: literal 											         				{ } // operand -> literal
	| object																		{ } // operand -> object
	| OPEN_BRACE attributeList CLOSE_BRACE											{ } // operand -> { attributeList } 
	;

attributeList: %empty																{ }
	| attributeList COMMA attribute													{ } // attributeList -> attributeList , attribute
	| attribute																		{ } // attributeList -> attribute
	;

attribute: NAME COLON expression													{ } // attribute -> NAME : expression
	;																				

object: variable																	{ } // object -> $(expression) o #(expression)
	| XPATH_OPERATOR expression CLOSE_PARENTHESIS									{ } // object -> (variable = expression)
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS									{ } // object -> ( expression )
	| OPEN_PARENTHESIS variable ASSIGNMENT_OPERATOR expression CLOSE_PARENTHESIS	{ } // object -> ( variable = expression )
	| OPEN_BRACKET CLOSE_BRACKET													{ } // object -> [ ] (empty array)
	| OPEN_BRACKET parameters CLOSE_BRACKET											{ } // object -> [ parameters ] (array with paraneters)		 */
	| object OPEN_PARENTHESIS CLOSE_PARENTHESIS										{ } // object -> NAME ()	
	| object OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS							{ } // object -> NAME ( parameters )	
	;

variable: NAME 																		{ } // variable -> NAME
	| object OPEN_BRACKET expression CLOSE_BRACKET									{ } // variable -> NAME [ expression ]
	| object DOT NAME																{ } // variable -> object . NAME
	;

lambda: OPEN_PARENTHESIS parameterDefinitions ARROW scope										{ } // lambda -> ( parameterDefinitions ) -> scope
	| OPEN_PARENTHESIS ARROW scope																{ } // lambda -> () -> scope
	| FUNCTION OPEN_PARENTHESIS parameterDefinitions CLOSE_PARENTHESIS scope 					{ }  // lambda -> "function" NAME ( parameterDefinitions ) scope
	| FUNCTION OPEN_PARENTHESIS CLOSE_PARENTHESIS scope 										{ }  // lambda -> "function" NAME ( ) scope
	;

sequence: BEGIN_SEQUENCE actionList END_SEQUENCE									{ } // sequence -> actionList
	;

actionList: actionList action														{ } // actionList -> actionList action
	| %empty																		{ } 
	;

action: KEY 																		{ } // action -> KEY
	| PLUS KEY	 																	{ } // action -> + KEY
	| MINUS KEY																		{ } // action -> - KEY
	| STRING	 																	{ } // action -> STRING
	;

parameters: parameters COMMA expression												{ } // parameters -> parameters , expression
	| expression																	{ } // parameters -> NAME
	;

literal: INTEGER 																	{ } // literal -> INTEGER
	| NUMBER 																		{ } // literal -> NUMBER
	| STRING 																		{ } // literal -> STRING
	| BOOLEAN							            								{ } // literal -> BOOLEAN
	| NULL_LITERAL																	{ } // literal -> NULL_LITERAL
	| UNDEFINED																		{ } // literal -> UNDEFINED
	| NAN																			{ } // literal -> NAN
	| sequence 																		{ } // literal -> NAME
	;
%%
