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
	ExceptionSetNode * exceptionSet;
	
	ExpressionNode * expression;
	LambdaNode * lambda;
	ObjectNode * object;

	OperationNode * operation;
	UnaryOperatorNode * unaryOperator;
	BinaryOperatorNode * binaryOperator;
	OperandNode * operand;
	
	AttributeListNode * attributeList;
	AttributeNode * attribute;
	VariableNode * variable;

	SequenceNode * sequence;
	ActionListNode * actionList;
	ActionNode * action;

	ParametersNode * parameters;	
	LiteralNode * literal;


	// Terminales.
	token token;
	int integer;
	char * number;
	char * string;
	char * operator;
	char * name;
	bool boolean;
	action_key_t key;
	assertion_t assertionType;
	cardinality_t cardinality;
	variable_scope_t variableScope;
	null_literal_t nullLiteral;
}


// Un token que jamás debe ser usado en la gramática.
%token <token> ERROR

// IDs y tipos de los tokens terminales generados desde Flex.

%token <token> BEGIN_SEQUENCE
%token <token> END_SEQUENCE
%token <key> KEY

%token <token> PIPE
%token <token> PLUS
%token <token> MINUS
%token <token> MULTIPLICATION
%token <operator> BINARY_OPERATOR
%token <operator> UNARY_OPERATOR
%token <operator> ASSIGNMENT_OPERATOR
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
%token <cardinality> XPATH_OPERATOR

%token <token> MODULE
%token <token> SUITE
%token <token> BEFORE_ALL
%token <token> AFTER_ALL

%token <assertionType> ASSERT
%token <assertionType> ASSERT_COMPARE

%token <token> RETRY
%token <token> TRY
%token <token> CATCH
%token <token> FINALLY

%token <token> IF
%token <token> ELSE
%token <token> FOR
%token <token> WHILE

%token <token> ARROW
%token <variableScope> VARIABLE
%token <token> FUNCTION
%token <token> ASYNC
%token <token> AWAIT
%token <token> RETURN

%token <integer> INTEGER
%token <number> NUMBER
%token <string> STRING
%token <boolean> BOOLEAN
%token <nullLiteral> NULL_LITERAL
%token <nullLiteral> UNDEFINED
%token <nullLiteral> NAN

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
%type <exceptionSet> exceptionSet
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

// De menor a mayor precedencia. 
%right ASSIGNMENT_OPERATOR
%left PLUS MINUS
%left MULTIPLICATION
%left BINARY_OPERATOR 

// El símbolo inicial de la gramatica.
%start program

%%

program: statementList											{ $$ = ProgramGrammarAction((ProgramUnion) {.statementList = $1}, PROG_STATEMENT_LIST); }
	|	 suite													{ $$ = ProgramGrammarAction((ProgramUnion) {.suite = $1}, PROG_SUITE); }
	;

suite:  SUITE OPEN_BRACE moduleList CLOSE_BRACE	   				{ $$ = SuiteGrammarAction(NULL, $3); } // suite -> Suite { moduleList }
	|	SUITE NAME OPEN_BRACE moduleList CLOSE_BRACE			{ $$ = SuiteGrammarAction($2, $4); 	} // suite -> Suite NAME { moduleList }
	;

moduleList: %empty 												{ $$ = ModuleListGrammarAction(NULL, NULL);  } 
	| moduleList module											{ $$ = ModuleListGrammarAction($1, $2); } // moduleList -> moduleList module
	;

module: MODULE scope 											{ $$ = ModuleGrammarAction(NULL, MODULE_CUSTOM, $2);       } // module -> Module scope
	| MODULE NAME scope 										{ $$ = ModuleGrammarAction($2, 	 MODULE_CUSTOM, $3);   	   } // module -> Module NAME scope
	| BEFORE_ALL scope 											{ $$ = ModuleGrammarAction(NULL, MODULE_BEFORE_ALL, $2);   } // module -> BeforeAll scope
	| AFTER_ALL scope 											{ $$ = ModuleGrammarAction(NULL, MODULE_AFTER_ALL, $2);   } // module -> AfterAll scope
	;

scope: OPEN_BRACE statementList CLOSE_BRACE  					{ $$ = ScopeGrammarAction($2); } // scope -> { statementList }
	;

statementList: %empty 											{ $$ = StatementListGrammarAction(NULL, NULL);  } 
	| statementList statement									{ $$ = StatementListGrammarAction($1, $2); } // statementList -> statementList statement
	; 

statement: expression SEMICOLON									{ $$ = StatementGrammarAction((StatementUnion) { .expression = $1 }, STATEMENT_EXPRESSION); } // statement -> expression ;
	| VARIABLE NAME ASSIGNMENT_OPERATOR expression SEMICOLON	{ $$ = DeclarationStatementGrammarAction($1, $2, $3, $4); } // statement -> var NAME = expression ;
	| VARIABLE NAME SEMICOLON									{ $$ = DeclarationStatementGrammarAction($1, $2, NULL, NULL); } // statement -> var NAME ;
	| variable ASSIGNMENT_OPERATOR expression SEMICOLON			{ $$ = AssignmentStatementGrammarAction($1, $2, $3); } // statement -> NAME = expression ;
	| RETURN expression SEMICOLON								{ $$ = StatementGrammarAction((StatementUnion) { .expression = $2 }, STATEMENT_RETURN); }
	| RETURN SEMICOLON											{ $$ = StatementGrammarAction((StatementUnion) { .expression = NULL }, STATEMENT_RETURN);  } // statement -> return ;
	| ASSERT_COMPARE expression COMMA expression SEMICOLON		{ $$ = AssertionStatementGrammarAction($1, $2, $4);  	} // statement -> assert expression , expression ; 
	| ASSERT expression SEMICOLON								{ $$ = AssertionStatementGrammarAction($1, $2, NULL);   } // statement -> assert expression ; 
	| function 													{ $$ = StatementGrammarAction((StatementUnion) { .function = $1 }, STATEMENT_FUNCTION); } // statement -> function
	| control													{ $$ = StatementGrammarAction((StatementUnion) { .control = $1 }, STATEMENT_CONTROL);  } // statement -> control
	; 

function: FUNCTION NAME OPEN_PARENTHESIS parameterDefinitions CLOSE_PARENTHESIS scope 		{ $$ = FunctionGrammarAction(false, $2, $4, $6); } // function -> "function" NAME ( parameters ) scope
	| ASYNC FUNCTION NAME OPEN_PARENTHESIS parameterDefinitions CLOSE_PARENTHESIS scope 	{ $$ = FunctionGrammarAction(true, $3, $5, $7); } // async function -> "function" NAME ( parameters ) scope
	| FUNCTION NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS scope 								{ $$ = FunctionGrammarAction(false, $2, NULL, $5); }  // function -> "function" NAME ( ) scope
	| ASYNC FUNCTION NAME OPEN_PARENTHESIS CLOSE_PARENTHESIS scope 							{ $$ = FunctionGrammarAction(true, $3, NULL, $6); }  // async function -> "function" NAME ( ) scope
	;

parameterDefinitions: parameterDefinitions COMMA NAME								{ $$ = ParameterDefinitionGrammarAction($1, $3); } // parameterDefinitions -> parameterDefinitions , NAME
	| NAME																			{ $$ = ParameterDefinitionGrammarAction(NULL, $1); } // parameterDefinitions -> NAME
	;
	
control: if 																									    { $$ = ControlGrammarAction((ControlUnion) { .ifControl = $1 }, IF_CONTROL); }
	| FOR OPEN_PARENTHESIS forExpression SEMICOLON forExpression SEMICOLON forExpression CLOSE_PARENTHESIS scope    { $$ = ForControlGrammarAction($3, $5, $7, $9); }
	| WHILE OPEN_PARENTHESIS expression CLOSE_PARENTHESIS scope														{ $$ = WhileControlGrammarAction($3, $5); }
	| try																											{ $$ = ControlGrammarAction((ControlUnion) { .tryControl = $1 }, TRY_CONTROL ); }
	; 

if: IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS scope 				{ $$ = IfControlGrammarAction($3, $5, NULL); } // if -> if ( expression ) scope
	| IF OPEN_PARENTHESIS expression CLOSE_PARENTHESIS scope else		{ $$ = IfControlGrammarAction($3, $5, $6); } // if -> if ( expression ) scope else
	; 

else: ELSE if 															{ $$ =  ElseControlGrammarAction((ElseControlUnion) { .ifControl = $2 }, false); } // else -> else if
	| ELSE scope													  	{ $$ =  ElseControlGrammarAction((ElseControlUnion) { .scope = $2}, true); } // else -> else scope
	; 

forExpression: %empty 													{ $$ = ForExpressionGrammarAction((ForExpressionUnion) {.expression = NULL}, FOR_EMPTY); } // forExpression -> %empty
	| expression														{ $$ = ForExpressionGrammarAction((ForExpressionUnion) { .expression = $1 }, FOR_EXPRESSION); } // forExpression -> expression
	| variable ASSIGNMENT_OPERATOR expression							{ $$ = AssignmentForExpressionGrammarAction($1, $2, $3); } // forExpression -> variable = expression
	| VARIABLE NAME ASSIGNMENT_OPERATOR expression						{ $$ = DeclarationForExpressionGrammarAction($1, $2, $3, $4); } // forExpression -> var NAME = expression
	; 

try: TRY scope retry CATCH OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS scope						{ $$ = TryControlGrammarAction($2, $3, $6, $8, NULL); } // try -> try scope retry catch ( NAME ) scope
	| TRY scope retry CATCH OPEN_PARENTHESIS NAME CLOSE_PARENTHESIS scope FINALLY scope			{ $$ = TryControlGrammarAction($2, $3, $6, $8, $10); } // try -> try scope retry catch ( NAME ) scope finally scope
	; 

retry: %empty																					{ $$ = NULL; } // retry -> %empty
	| RETRY OPEN_PARENTHESIS exceptionSet COMMA INTEGER CLOSE_PARENTHESIS scope					{ $$ = RetryControlGrammarAction($7, $5, $3); } // retry -> retry ( exceptionSet , INTEGER ) scope
	| RETRY OPEN_PARENTHESIS exceptionSet CLOSE_PARENTHESIS scope								{ $$ = RetryControlGrammarAction($5, 1, $3); } // retry -> retry ( exceptionSet ) scope
	; 

exceptionSet: variable																{ $$ = ExceptionSetGrammarAction($1, NULL); } // exceptionSet -> NAME
	| exceptionSet PIPE variable													{ $$ = ExceptionSetGrammarAction($3, $1); } // exceptionSet -> exceptionSet | NAME
	;
	
expression: operation																{ $$ = ExpressionGrammarAction((ExpressionUnion) {.operation = $1}, true); } // expression -> operation
	| lambda																		{ $$ = ExpressionGrammarAction((ExpressionUnion) {.lambda = $1}, false); } // expression -> lambda
	;

operation: operand 																	{ $$ = OperationGrammarAction(NULL, NULL, $1); } //operation -> operand
	| operation binaryOperator operand												{ $$ = OperationGrammarAction($2, $1, $3); } // operation -> operation binaryOperator operand
	;

unaryOperator: MINUS																{ $$ = UnaryOperatorGrammarAction(UNARY_MINUS, NULL); } // unaryOperator -> -
	| AWAIT 																		{ $$ = UnaryOperatorGrammarAction(UNARY_AWAIT, NULL); } // unaryOperator -> await
	| UNARY_OPERATOR																{ $$ = UnaryOperatorGrammarAction(UNARY_GENERIC, $1); } // unaryOperator -> UNARY_OPERATOR
	;

binaryOperator: PLUS																{ $$ = BinaryOperatorGrammarAction(BINARY_PLUS, NULL); } // binaryOperator -> +
	| MINUS																			{ $$ = BinaryOperatorGrammarAction(BINARY_MINUS, NULL); } // binaryOperator -> -
	| MULTIPLICATION																{ $$ = BinaryOperatorGrammarAction(BINARY_MULTIPLICATION, NULL); } // binaryOperator -> *
	| BINARY_OPERATOR																{ $$ = BinaryOperatorGrammarAction(BINARY_GENERIC, $1); } // binaryOperator -> BINARY_OPERATOR
	;

operand: unaryOperator operand														{ $$ = OperandGrammarAction(OPERAND, (OperandUnion) { .operand = $2 }, $1); } // operand -> unaryOperator operand
	| literal 											         					{ $$ = OperandGrammarAction(LITERAL, (OperandUnion) { .literal = $1 }, NULL); } // operand -> literal
	| object																		{ $$ = OperandGrammarAction(OBJECT, (OperandUnion) { .object = $1 }, NULL); } // operand -> object
	| OPEN_BRACE attributeList CLOSE_BRACE											{ $$ = OperandGrammarAction(ATTRIBUTE_LIST, (OperandUnion) { .attributes = $2}, NULL ); } // operand -> { attributeList }
	;

attributeList: %empty																{ $$ = AttributeListGrammarAction(NULL, NULL); } // attributeList -> %empty
	| attributeList COMMA attribute													{ $$ = AttributeListGrammarAction($3, $1); } // attributeList -> attributeList , attribute
	| attribute																		{ $$ = AttributeListGrammarAction($1, NULL); } // attributeList -> attribute
	;

attribute: NAME COLON expression													{ $$ = AttributeGrammarAction($1, $3); } // attribute -> NAME : expression
	;											

object: variable																	{ $$ = ObjectGrammarAction((ObjectUnion) { .variable = $1}, OBJ_VARIABLE); } // object -> $(expression) o #(expression)
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS									{ $$ = ObjectGrammarAction((ObjectUnion) { .expression = $2}, OBJ_EXPRESSION); } // object -> ( expression )
	| OPEN_PARENTHESIS variable ASSIGNMENT_OPERATOR expression CLOSE_PARENTHESIS	{ $$ = AssignmentObjectGrammarAction($2, $3, $4); } // object -> ( variable = expression )
	| OPEN_BRACKET CLOSE_BRACKET													{ $$ = ArrayObjectGrammarAction(NULL); } // object -> [ ] (empty array)
	| OPEN_BRACKET parameters CLOSE_BRACKET											{ $$ = ArrayObjectGrammarAction($2); } // object -> [ parameters ] (array with paraneters)		 */
	| object OPEN_PARENTHESIS CLOSE_PARENTHESIS										{ $$ = FunctionCallObjectGrammarAction($1, NULL); } // object -> NAME ()	
	| object OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS							{ $$ = FunctionCallObjectGrammarAction($1, $3); } // object -> NAME ( parameters )	
	| XPATH_OPERATOR expression CLOSE_PARENTHESIS									{ $$ = XPathObjectGrammarAction($1, $2); } // object -> (variable = expression)
	;

variable: NAME 																		{ $$ = VariableGrammarAction($1, NULL, NULL); } // variable -> NAME
	| object OPEN_BRACKET expression CLOSE_BRACKET									{ $$ = VariableGrammarAction(NULL, $1, $3); } // variable -> NAME [ expression ]
	| object DOT NAME																{ $$ = VariableGrammarAction($3, $1, NULL); } // variable -> object . NAME
	;

lambda: OPEN_PARENTHESIS parameterDefinitions ARROW scope										{ $$ = LambdaGrammarAction($2, $4, true, false); } // lambda -> ( parameterDefinitions ) -> scope
	| OPEN_PARENTHESIS ARROW scope																{ $$ = LambdaGrammarAction(NULL, $3, true, false); } // lambda -> () -> scope
	| FUNCTION OPEN_PARENTHESIS parameterDefinitions CLOSE_PARENTHESIS scope 					{ $$ = LambdaGrammarAction($3, $5, false, false); }  // lambda -> "function" ( parameterDefinitions ) scope
	| FUNCTION OPEN_PARENTHESIS CLOSE_PARENTHESIS scope 										{ $$ = LambdaGrammarAction(NULL, $4, false, false); }  // lambda -> "function" ( ) scope
	| ASYNC OPEN_PARENTHESIS parameterDefinitions ARROW scope									{ $$ = LambdaGrammarAction($3, $5, true, true); } // lambda -> ( parameterDefinitions ) -> scope
	| ASYNC OPEN_PARENTHESIS ARROW scope														{ $$ = LambdaGrammarAction(NULL, $4, true, true); } // lambda -> () -> scope
	| ASYNC FUNCTION OPEN_PARENTHESIS parameterDefinitions CLOSE_PARENTHESIS scope 				{ $$ = LambdaGrammarAction($4, $6, false, true); }  // lambda -> "function" ( parameterDefinitions ) scope
	| ASYNC FUNCTION OPEN_PARENTHESIS CLOSE_PARENTHESIS scope 									{ $$ = LambdaGrammarAction(NULL, $5, false, true); }  // lambda -> "function" ( ) scope
	;

sequence: BEGIN_SEQUENCE actionList END_SEQUENCE									{ $$ = SequenceGrammarAction($2); } // sequence -> actionList
	;

actionList: actionList action														{ $$ = ActionListGrammarAction($1, $2); } // actionList -> actionList action
	| %empty																		{ $$ = ActionListGrammarAction(NULL, NULL); } 
	;

action: KEY 																		{ $$ = KeyActionGrammarAction($1, KEY_STATE_PRESS); } // action -> KEY
	| PLUS KEY	 																	{ $$ = KeyActionGrammarAction($2, KEY_STATE_DOWN); } // action -> + KEY
	| MINUS KEY																		{ $$ = KeyActionGrammarAction($2, KEY_STATE_UP); } // action -> - KEY
	| STRING	 																	{ $$ = StreamActionGrammarAction($1); } // action -> STRING
	;

parameters: parameters COMMA expression												{ $$ = ParametersGrammarAction($1, $3); } // parameters -> parameters , expression
	| expression																	{ $$ = ParametersGrammarAction(NULL, $1); } // parameters -> NAME
	;

literal: INTEGER 																	{ $$ = LiteralGrammarAction((LiteralUnion) { .integer = $1 } , L_INTEGER); } // literal -> INTEGER
	| NUMBER 																		{ $$ = LiteralGrammarAction((LiteralUnion) { .string = $1 } , L_STRING); } // literal -> NUMBER
	| STRING 																		{ $$ = LiteralGrammarAction((LiteralUnion) { .string = $1 } , L_STRING); } // literal -> STRING
	| BOOLEAN							            								{ $$ = LiteralGrammarAction((LiteralUnion) { .boolean = $1 } , L_BOOLEAN); } // literal -> BOOLEAN
	| NULL_LITERAL																	{ $$ = LiteralGrammarAction((LiteralUnion) { .nullLiteral = $1 } , L_NIL); } // literal -> NULL_LITERAL
	| UNDEFINED																		{ $$ = LiteralGrammarAction((LiteralUnion) { .nullLiteral = $1 } , L_UNDEFINED); } // literal -> UNDEFINED
	| NAN																			{ $$ = LiteralGrammarAction((LiteralUnion) { .nullLiteral = $1 } , L_NAN); } // literal -> NAN
	| sequence 																		{ $$ = LiteralGrammarAction((LiteralUnion) { .sequence = $1 } , L_SEQUENCE); } // literal -> sequence
	;
%%
