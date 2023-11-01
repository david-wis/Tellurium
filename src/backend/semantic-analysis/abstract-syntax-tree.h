#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
// typedef struct Expression Expression;

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/
// typedef struct {
// 	int value;
// } Constant;

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identitifque cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/
// typedef enum {
// 	EXPRESSION,
// 	CONSTANT
// } FactorType;

// typedef struct {
// 	FactorType type;
// 	Expression * expression;
// } Factor;

// typedef enum {
// 	ADDITION,
// 	SUBTRACTION,
// 	// MULTIPLICATION,
// 	DIVISION,
// 	FACTOR
// } ExpressionType;

// struct Expression {
// 	ExpressionType type;
// 	Expression * leftExpression;
// 	Expression * rightExpression;
// };

typedef struct ModuleListNode ModuleListNode;
typedef struct StatementListNode StatementListNode;
typedef struct ExceptionSetNode ExceptionSetNode;
typedef struct IfControlNode IfControlNode;
typedef struct ParameterDefinitionNode ParameterDefinitionNode;
typedef struct ScopeNode ScopeNode;

typedef struct {

} VariableNode;



typedef union {
	OperationNode * operation;
	LambdaNode * lambda;
} ExpressionNode;

typedef enum {
	CONST,
	LET,
	VAR
} declaration_type_t;

typedef struct {
	declaration_type_t type;
	char * name;
	ExpressionNode * expression;
} Declaration;

typedef struct {
	VariableNode * variable;
	ExpressionNode * expression;
} Assignment;

struct ParameterDefinitionNode {
	ParameterDefinitionNode * parameterDefinitionList;
	char * name;
};

typedef struct {
	char * name;
	ParameterDefinitionNode * parameters;
	ScopeNode * scope;
} FunctionNode;

typedef union {
	IfControlNode * ifControl;
	ScopeNode * scope;
} ElseControlNode;

struct IfControlNode{
	ExpressionNode * condition;
	ScopeNode * scope;
	ElseControlNode * elseControl; //Nullable
} ;

struct ExceptionSetNode {
	ExceptionSetNode * exceptionSet;
	char * exception;
};

typedef struct {
	ScopeNode * retryScope;
	unsigned retryCount; //default 1
	ExceptionSetNode * exceptionSet;
} RetryControlNode;

typedef struct {
	ScopeNode * tryScope;	
	RetryControlNode * retryControl; //Nullable
	char * exceptionName;
	ScopeNode * catchScope;
	ScopeNode * finallyScope; //Nullable
} TryControlNode;

typedef union {
	ExpressionNode * expression;
	Declaration * declaration;
	Assignment * assignment
} ForExpressionNode;

typedef struct {
	ForExpressionNode * forExpressionStart; //Nullable
	ForExpressionNode * forExpressionCondition; //Nullable
	ForExpressionNode * forExpressionNext; //Nullable
	ScopeNode * scope;
} ForControl;

typedef struct {
	ExpressionNode * condition;
	ScopeNode * scope;
} WhileControl;

typedef struct {
	IfControlNode * ifControl;
	TryControlNode * tryControl;
	ForControl * forControl;
	WhileControl * whileControl;
} ControlNode;

typedef enum {
	ASSERT_TRUE,
	ASSERT_FALSE,
	ASSERT_EQUALS,
	ASSERT_NOT_EQUALS
} assertion_type_t;

typedef struct {
	assertion_type_t type;
	ExpressionNode * expression;
	ExpressionNode * expected;
} Assertion;

typedef struct {
	ExpressionNode * expression;
} Return;

typedef union {
	ExpressionNode * expression;
	Declaration * declaration;
	Assignment * assignment;
	FunctionNode * function;
	ControlNode * control;
	Assertion * assertion;
	Return * ret;
} StatementNode; 

struct StatementListNode {
	StatementListNode * statementList;
	StatementNode * statement;
};

struct ScopeNode{
	StatementListNode * statementList;
};

typedef enum {
	BEFORE_ALL,
	AFTER_ALL,
	CUSTOM
} module_type_t; 

typedef struct {
	char * name;
	module_type_t type;	
	ScopeNode * scope;
} ModuleNode; 

struct ModuleListNode{
	ModuleListNode * moduleList;
	ModuleNode * module;
};

typedef struct {
	char * name;
	ModuleListNode * moduleList;
} SuiteNode;

typedef union {
	SuiteNode * suite;
	StatementListNode * statementList;
} Program;

#endif
