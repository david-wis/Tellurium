#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"


#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "../../backend/support/shared.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// general-actions.c
Program * ProgramGrammarAction(ProgramUnion p, ProgramType type);
SuiteNode * SuiteGrammarAction(char * name, ModuleListNode * moduleList);
ModuleListNode * ModuleListGrammarAction(ModuleListNode * moduleList, ModuleNode * module);
ModuleNode * ModuleGrammarAction(char * name, ModuleType type, ScopeNode * scope);
ScopeNode * ScopeGrammarAction(StatementListNode * statementList);
StatementListNode * StatementListGrammarAction(StatementListNode * statementList, StatementNode * statement);
StatementNode * StatementGrammarAction(StatementUnion statementUnion, StatementType type);
StatementNode * DeclarationStatementGrammarAction(variable_scope_t type, char * name, char * op, ExpressionNode * expression);
StatementNode * AssignmentStatementGrammarAction(VariableNode * variable, char * op, ExpressionNode * expression);
StatementNode * AssertionStatementGrammarAction(assertion_t type, ExpressionNode * expression, ExpressionNode * expected);

// function-actions.c
FunctionNode * FunctionGrammarAction(bool async, char * name, ParameterDefinitionNode * parameters, ScopeNode * scope);
ParameterDefinitionNode * ParameterDefinitionGrammarAction(ParameterDefinitionNode * list, char * name);
LambdaNode * LambdaGrammarAction(ParameterDefinitionNode * parameters, ScopeNode * scope, bool isArrow, bool isAsync);

// control-actions.c
ControlNode * ControlGrammarAction(ControlUnion control, ControlType type);
ControlNode * ForControlGrammarAction(ForExpressionNode * forExpressionStart, ForExpressionNode * forExpressionCondition, ForExpressionNode * forExpressionNext, ScopeNode * scope);
ControlNode * WhileControlGrammarAction(ExpressionNode * condition, ScopeNode * scope);
IfControlNode * IfControlGrammarAction(ExpressionNode * condition, ScopeNode * scope, ElseControlNode * elseControl);
ElseControlNode * ElseControlGrammarAction(ElseControlUnion data, bool isScope);
ForExpressionNode * ForExpressionGrammarAction(ForExpressionUnion expression, ForExpressionType type);
ForExpressionNode * DeclarationForExpressionGrammarAction(variable_scope_t type, char * name, char * op, ExpressionNode * expression);
ForExpressionNode * AssignmentForExpressionGrammarAction(VariableNode * variable, char * op, ExpressionNode * expression);
TryControlNode * TryControlGrammarAction(ScopeNode * tryScope, RetryControlNode * retryControl, char * exceptionName, ScopeNode * catchScope, ScopeNode * finallyScope);
RetryControlNode * RetryControlGrammarAction(ScopeNode * retryScope, unsigned retryCount, ExceptionSetNode * exceptionSet);
ExceptionSetNode * ExceptionSetGrammarAction(VariableNode * variable, ExceptionSetNode * next);

// operation-actions.c
OperationNode * OperationGrammarAction(BinaryOperatorNode * operator, OperationNode * left, OperandNode * right);
UnaryOperatorNode * UnaryOperatorGrammarAction(UnaryOperator operator, char * op);
BinaryOperatorNode * BinaryOperatorGrammarAction(BinaryOperator operator, char * op); 
OperandNode * OperandGrammarAction(OperandType type, OperandUnion data, UnaryOperatorNode * unaryOperator);

// expression-actions.c
ExpressionNode * ExpressionGrammarAction(ExpressionUnion expression, bool isOperation);
ObjectNode * ObjectGrammarAction(ObjectUnion object, ObjectType type);
ObjectNode * AssignmentObjectGrammarAction(	VariableNode * variable, char * op, ExpressionNode * expression);
ObjectNode * ArrayObjectGrammarAction(ParametersNode * parameters);
ObjectNode * FunctionCallObjectGrammarAction(ObjectNode * callable, ParametersNode * parameters);
ObjectNode * XPathObjectGrammarAction(cardinality_t cardinality, ExpressionNode * expression);
AttributeListNode * AttributeListGrammarAction(AttributeNode * attribute, AttributeListNode * next);
AttributeNode * AttributeGrammarAction(char * name, ExpressionNode * expression);
VariableNode * VariableGrammarAction(char * name, ObjectNode * object, ExpressionNode * arraySubexpression);
ParametersNode * ParametersGrammarAction(ParametersNode * next, ExpressionNode * expression);
LiteralNode * LiteralGrammarAction(LiteralUnion literalUnion, LiteralType type);

// sequence-actions.c
SequenceNode * SequenceGrammarAction(ActionListNode * actionList);
ActionListNode * ActionListGrammarAction(ActionListNode * next, ActionNode * action);
ActionNode * KeyActionGrammarAction(action_key_t key, ActionKeyState state);
ActionNode * StreamActionGrammarAction(char* stream);

#endif
