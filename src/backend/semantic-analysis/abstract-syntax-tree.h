#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/types.h"
#include <stdbool.h>

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
typedef struct OperationNode OperationNode; 
typedef struct OperandNode OperandNode;
typedef struct ObjectNode ObjectNode;
typedef struct ActionListNode ActionListNode;
typedef struct ParametersNode ParametersNode;
typedef struct ExpressionNode ExpressionNode;
typedef struct Assignment Assignment;
typedef struct SequenceNode SequenceNode;
typedef struct VariableNode VariableNode;
typedef struct AttributeListNode AttributeListNode;


// TODO: Ver si conviene mover tipos de dato globales a otro archivo 
typedef enum LiteralType {
	L_INTEGER,
	L_STRING,
	L_BOOLEAN,
	L_SEQUENCE,
	L_NIL,
	L_UNDEFINED,
	L_NAN
} LiteralType;


typedef union LiteralUnion {
	int integer;
	char * string; // Also used for number 
	bool boolean;
	null_literal_t nullLiteral;
	SequenceNode * sequence;
} LiteralUnion; 

typedef struct LiteralNode {
	LiteralUnion literalUnion;
	LiteralType type;
} LiteralNode;

struct ParametersNode {
	ExpressionNode * expression;
	ParametersNode * next;
};

typedef enum ActionKeyState {
	KEY_STATE_DOWN = 0,	
	KEY_STATE_UP,	
	KEY_STATE_PRESS
} ActionKeyState; 


typedef struct Key {
	action_key_t key_value;
	ActionKeyState state;
} Key;

typedef union ActionUnion {
	Key * key;
	char * stream;
} ActionUnion;

typedef enum ActionType {
	ACTION_KEY,
	ACTION_STREAM
} ActionType;

typedef struct ActionNode {
	ActionUnion action;
	ActionType type;
} ActionNode;

struct ActionListNode {
	ActionNode * action;
	ActionListNode * next;
};

struct SequenceNode {
	ActionListNode * actionList;
}; 

typedef struct Array {
	ParametersNode * parameters; // Nullable
} Array;

typedef struct FunctionCall {
	ObjectNode * callable;
	ParametersNode * parameters; // Nullable
} FunctionCall;

typedef struct XPath {
	cardinality_t cardinality;
	ExpressionNode * expression;
} XPath;


typedef union ObjectUnion {
	VariableNode * variable;
	ExpressionNode * expression;
	Assignment * assignment;
	Array * array;
	FunctionCall * functionCall;
	XPath * xpath;
} ObjectUnion;

typedef enum ObjectType {
	OBJ_VARIABLE,
	OBJ_XPATH,
	OBJ_EXPRESSION,
	OBJ_ASSIGNMENT,
	OBJ_ARRAY,
	OBJ_FUNCTION_CALL
} ObjectType;

struct ObjectNode {
	ObjectUnion object;
	ObjectType type;
} ;

struct VariableNode {
	char * name;
	ObjectNode * object; // Nulleable
	ExpressionNode * arraySubexpression; // Nulleable 
}; 

typedef struct LambdaNode {
	ParameterDefinitionNode * parameters;
	ScopeNode * scope;
	bool isArrow;
	bool async;
} LambdaNode;


typedef struct AttributeNode {
	char * name;
	ExpressionNode * expression;
} AttributeNode;


struct AttributeListNode {
	AttributeNode * attribute;
	AttributeListNode * next;
}; 

typedef union OperandUnion {
	OperandNode * operand;
	LiteralNode * literal;
	ObjectNode * object;
	AttributeListNode * attributes;
} OperandUnion;

typedef enum UnaryOperator {
	UNARY_MINUS,
	UNARY_AWAIT,
	UNARY_GENERIC
} UnaryOperator;

typedef struct UnaryOperatorNode {
	UnaryOperator operator;
	char * op; //Nullable
} UnaryOperatorNode;

typedef enum OperandType {
	LITERAL,
	OBJECT,
	ATTRIBUTE_LIST,
	OPERAND
} OperandType;

struct OperandNode {
	UnaryOperatorNode * unaryOperator; //Nullable
	OperandUnion data;
	OperandType type;
};

typedef enum BinaryOperator {
	BINARY_PLUS,
	BINARY_MINUS,
	BINARY_MULTIPLICATION,
	BINARY_GENERIC
} BinaryOperator;

typedef struct BinaryOperatorNode {
	BinaryOperator operator;
	char * op; //Nullable
} BinaryOperatorNode;

struct OperationNode {
	OperationNode * left; 
	OperandNode * right; 
	BinaryOperatorNode * operator; // Nullable
};

typedef union ExpressionUnion {
	OperationNode * operation;
	LambdaNode * lambda;
} ExpressionUnion;

struct ExpressionNode {
	ExpressionUnion expression;
	bool isOperation;
};

typedef struct Declaration {
	variable_scope_t type;
	char * name;
	char * op; //Nullable TODO: Revisar si es mejor usar enums 
	ExpressionNode * expression; //Nullable
} Declaration;

struct Assignment {
	VariableNode * variable;
	char * op; // TODO: Revisar si es mejor usar enums 
	ExpressionNode * expression;
};

struct ParameterDefinitionNode {
	ParameterDefinitionNode * next;
	char * name;
};

typedef struct FunctionNode {
	bool async;
	char * name;
	ParameterDefinitionNode * parameters;
	ScopeNode * scope;
} FunctionNode;


typedef union ElseControlUnion {
	IfControlNode * ifControl;
	ScopeNode * scope;
} ElseControlUnion;


typedef struct ElseControlNode {
	ElseControlUnion data;
	bool isScope;
} ElseControlNode;


struct IfControlNode{
	ExpressionNode * condition;
	ScopeNode * scope;
	ElseControlNode * elseControl; //Nullable
} ;

struct ExceptionSetNode {
	ExceptionSetNode * next;
	char * exception;
};

typedef struct RetryControlNode {
	ScopeNode * retryScope;
	unsigned retryCount; //default 1
	ExceptionSetNode * exceptionSet;
} RetryControlNode;

typedef struct TryControlNode {
	ScopeNode * tryScope;	
	RetryControlNode * retryControl; //Nullable
	char * exceptionName;
	ScopeNode * catchScope;
	ScopeNode * finallyScope; //Nullable
} TryControlNode;

typedef union ForExpressionUnion {
	ExpressionNode * expression;
	Declaration * declaration;
	Assignment * assignment;
} ForExpressionUnion;

typedef enum ForExpressionType {
	FOR_EXPRESSION,
	FOR_DECLARATION,
	FOR_ASSIGNMENT,
	FOR_EMPTY
} ForExpressionType;

typedef struct ForExpressionNode{
	ForExpressionUnion expression;
	ForExpressionType type;
} ForExpressionNode;

typedef struct ForControl{
	ForExpressionNode * forExpressionStart; //Nullable
	ForExpressionNode * forExpressionCondition; //Nullable
	ForExpressionNode * forExpressionNext; //Nullable
	ScopeNode * scope;
} ForControl;

typedef struct WhileControl{
	ExpressionNode * condition;
	ScopeNode * scope;
} WhileControl;

typedef union ControlUnion {
	IfControlNode * ifControl;
	TryControlNode * tryControl;
	ForControl * forControl;
	WhileControl * whileControl;
} ControlUnion;

typedef enum ControlType {
	IF_CONTROL,
	TRY_CONTROL,
	FOR_CONTROL,
	WHILE_CONTROL
} ControlType;


typedef struct ControlNode {
	ControlUnion control;
	ControlType type;
} ControlNode;

typedef struct Assertion {
	assertion_t type;
	ExpressionNode * expression;
	ExpressionNode * expected;
} Assertion;

typedef enum StatementType {
	STATEMENT_EXPRESSION,
	STATEMENT_DECLARATION,
	STATEMENT_ASSIGNMENT,
	STATEMENT_FUNCTION,
	STATEMENT_CONTROL,
	STATEMENT_ASSERTION,
	STATEMENT_RETURN
} StatementType;

typedef union StatementUnion {
	ExpressionNode * expression;
	Declaration * declaration;
	Assignment * assignment;
	FunctionNode * function;
	ControlNode * control;
	Assertion * assertion;
} StatementUnion;


typedef struct StatementNode {
	StatementUnion statementUnion;
	StatementType type;
} StatementNode; 

struct StatementListNode {
	StatementListNode * statementList; //Nullable
	StatementNode * statement; //Nullable
};

struct ScopeNode {
	StatementListNode * statementList;
};

typedef enum ModuleType {
	MODULE_BEFORE_ALL,
	MODULE_AFTER_ALL,
	MODULE_CUSTOM
} ModuleType; 

typedef struct ModuleNode {
	char * name;
	ModuleType type;	
	ScopeNode * scope;
} ModuleNode; 

struct ModuleListNode {
	ModuleListNode * moduleList;
	ModuleNode * module;
};

typedef struct SuiteNode {
	char * name;
	ModuleListNode * moduleList;
} SuiteNode;

// Program
typedef union ProgramUnion {
	SuiteNode * suite;
	StatementListNode * statementList;
} ProgramUnion;

typedef enum ProgramType {
	PROG_SUITE,
	PROG_STATEMENT_LIST
} ProgramType;

typedef struct Program {
	ProgramUnion program;
	ProgramType type;
} Program;

#endif
