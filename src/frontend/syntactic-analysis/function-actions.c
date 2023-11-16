#include "bison-actions.h"
FunctionNode * FunctionGrammarAction(bool async, char * name, ParameterDefinitionNode * parameters, ScopeNode * scope) {
    FunctionNode * node = gcCalloc(sizeof(*node));
    node->async = async;
    node->name = name;
    node->parameters = parameters;
    node->scope = scope;
    return node;
}

ParameterDefinitionNode * ParameterDefinitionGrammarAction(ParameterDefinitionNode * list, char * name) {
    ParameterDefinitionNode * node = gcCalloc(sizeof(*node));
    node->next = list;
    node->name = name;
    return node;
}

LambdaNode * LambdaGrammarAction(ParameterDefinitionNode * parameters, ScopeNode * scope, bool isArrow, bool async) {
    LambdaNode * node = gcCalloc(sizeof(*node));
    node->parameters = parameters;
    node->scope = scope;
    node->isArrow = isArrow;
    node->async = async;
    return node;
}
