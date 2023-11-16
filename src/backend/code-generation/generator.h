#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"

void generate(Program * program, const char * inputFile);

// general-generation.c
void ProgramGenerate(Program * program);
void SuiteGenerate(SuiteNode * suite);
void ModuleListGenerate(ModuleListNode * moduleList);
void ModuleGenerate(ModuleNode * module);
void ScopeGenerate(ScopeNode * scope);
void StatementListGenerate(StatementListNode * statementList);
void StatementGenerate(StatementNode * statement);
void AssignmentGenerateAux(VariableNode * variable, char * op, ExpressionNode * expression);
void DeclarationGenerateAux(variable_scope_t type, char * name, char * op, ExpressionNode * expression);
//funnction-generation.c
void FunctionGenerate(FunctionNode * function);
void ParameterDefinitionGenerate(ParameterDefinitionNode * parameterDefinition);
void LambdaGenerate(LambdaNode * lambda);
//control-generation.c
void ControlGenerate(ControlNode * control);
void IfControlGenerate(IfControlNode * ifControl);
void ElseControlGenerate(ElseControlNode * elseControl);
void ForExpressionGenerate(ForExpressionNode * forExpression);
void TryControlGenerate(TryControlNode * tryControl);
void ExceptionSetGenerate(ExceptionSetNode * exceptionSet);
//operation-generation.c
void OperationGenerate(OperationNode * operation);
void UnaryOperatorGenerate(UnaryOperatorNode * unaryOperator);
void BinaryOperatorGenerate(BinaryOperatorNode * binaryOperator);
void OperandGenerate(OperandNode * operand);
//expression-generation.c
void ExpressionGenerate(ExpressionNode * expression);
void ObjectGenerate(ObjectNode * object);
void AttributeListGenerate(AttributeListNode * attributeList);
void AttributeGenerate(AttributeNode * attribute);
void VariableGenerate(VariableNode * variable);
void ParametersGenerate(ParametersNode * parameters);
void LiteralGenerate(LiteralNode * literal);
//sequence-generation.c
void SequenceGenerate(SequenceNode * sequence);
void ActionListGenerate(ActionListNode * actionList);
void ActionGenerate(ActionNode * action);

#endif
