#include "bison-actions.h"
FunctionNode * FunctionGrammarAction(char * name, ParameterDefinitionNode * parameters, ScopeNode * scope) {
    FunctionNode * node = calloc(1, sizeof(*node));
    node->name = name;
    node->parameters = parameters;
    node->scope = scope;
    return node;
}

ParameterDefinitionNode * ParameterDefinitionGrammarAction(ParameterDefinitionNode * list, char * name) {
    ParameterDefinitionNode * node = calloc(1, sizeof(*node));
    node->parameterDefinitionList = list;
    node->name = name;
    return node;
}

LambdaNode * LambdaGrammarAction(ParameterDefinitionNode * parameters, ScopeNode * scope, bool_t isArrow) {
    LambdaNode * node = calloc(1, sizeof(*node));
    node->parameters = parameters;
    node->scope = scope;
    node->isArrow = isArrow;
    return node;
}
