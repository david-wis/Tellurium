#include "bison-actions.h"

ExpressionNode * ExpressionGrammarAction(ExpressionUnion expression, bool_t isOperation) {
    return NULL;
}

ObjectNode * ObjectGrammarAction(ObjectUnion object, ObjectType type) {
    return NULL;
}

ObjectNode * AssignmentObjectGrammarAction(	VariableNode * variable, char * op, ExpressionNode * expression) {
    return NULL;
}

ObjectNode * ArrayObjectGrammarAction(ParametersNode * parameters) {
    return NULL;
}

ObjectNode * FunctionCallGrammarAction(ObjectNode * callable, ParametersNode * parameters) {
    return NULL;
}

ObjectNode * XPathObjectGrammarAction(cardinality_t cardinality, ExpressionNode * expression) {
    return NULL;
}

AttributeListNode * AttributeListGrammarAction(AttributeNode * attribute, AttributeListNode * next) {
    return NULL;
}

AttributeNode * AttributeGrammarAction(char * name, ExpressionNode * expression) {
    return NULL;
}

VariableNode * VariableGrammarAction(char * name, ObjectNode * object, ExpressionNode * arraySubexpression) {
    return NULL;
}

ParametersNode * ParametersGrammarAction(ParametersNode * next, ExpressionNode * expression) {
    return NULL;
}

LiteralNode * LiteralGrammarAction(LiteralUnion literalUnion, LiteralType type) {
    return NULL;
}

