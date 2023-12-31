#include "bison-actions.h"

ExpressionNode * ExpressionGrammarAction(ExpressionUnion expression, bool isOperation) {
    ExpressionNode * node = gcCalloc(sizeof(*node));
    node->expression = expression;
    node->isOperation = isOperation;
    return node;
}

ObjectNode * ObjectGrammarAction(ObjectUnion object, ObjectType type) {
    ObjectNode * node = gcCalloc(sizeof(*node));
    node->object = object;
    node->type = type;
    return node;
}

ObjectNode * AssignmentObjectGrammarAction(	VariableNode * variable, char * op, ExpressionNode * expression) {
    Assignment * assignment = gcCalloc(sizeof(*assignment));
    assignment->variable = variable;
    assignment->op = op;
    assignment->expression = expression;
    return ObjectGrammarAction((ObjectUnion) {.assignment = assignment}, OBJ_ASSIGNMENT);
}

ObjectNode * ArrayObjectGrammarAction(ParametersNode * parameters) {
    Array * array = gcCalloc(sizeof(*array));
    array->parameters = parameters;
    return ObjectGrammarAction((ObjectUnion) {.array = array}, OBJ_ARRAY);
}

ObjectNode * FunctionCallObjectGrammarAction(ObjectNode * callable, ParametersNode * parameters) {
    FunctionCall * functionCall = gcCalloc(sizeof(*functionCall));
    functionCall->callable = callable;
    functionCall->parameters = parameters;
    return ObjectGrammarAction((ObjectUnion) {.functionCall = functionCall}, OBJ_FUNCTION_CALL);
}

ObjectNode * XPathObjectGrammarAction(cardinality_t cardinality, ExpressionNode * expression) {
    XPath * node = gcCalloc(sizeof(*node));
    node->cardinality = cardinality;
    node->expression = expression;
    return ObjectGrammarAction((ObjectUnion) {.xpath = node}, OBJ_XPATH);
}

AttributeListNode * AttributeListGrammarAction(AttributeNode * attribute, AttributeListNode * next) {
    AttributeListNode * node = gcCalloc(sizeof(*node));
    node->attribute = attribute;
    node->next = next;
    return node;
}

AttributeNode * AttributeGrammarAction(char * name, ExpressionNode * expression) {
    AttributeNode * node = gcCalloc(sizeof(*node));
    node->name = name;
    node->expression = expression;
    return node;
}

VariableNode * VariableGrammarAction(char * name, ObjectNode * object, ExpressionNode * arraySubexpression) {
    VariableNode * node = gcCalloc(sizeof(*node));
    node->name = name;
    node->object = object;
    node->arraySubexpression = arraySubexpression;
    return node;
}

ParametersNode * ParametersGrammarAction(ParametersNode * next, ExpressionNode * expression) {
   ParametersNode * node = gcCalloc(sizeof(*node));
    node->next = next;
    node->expression = expression;
    return node;
}

LiteralNode * LiteralGrammarAction(LiteralUnion literalUnion, LiteralType type) {
    LiteralNode * node = gcCalloc(sizeof(*node));
    node->literalUnion = literalUnion;
    node->type = type;
    return node;
}

