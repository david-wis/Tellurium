#include "bison-actions.h"

ControlNode * ControlGrammarAction(ControlUnion control, ControlType type){
    ControlNode * node = gcCalloc(sizeof(*node));
    node->control = control;
    node->type = type;
    return node;
}

ControlNode * ForControlGrammarAction(ForExpressionNode * forExpressionStart, ForExpressionNode * forExpressionCondition, ForExpressionNode * forExpressionNext, ScopeNode * scope){
    ForControl * forControl = gcCalloc(sizeof(*forControl));
    forControl->forExpressionStart = forExpressionStart;
    forControl->forExpressionCondition = forExpressionCondition;
    forControl->forExpressionNext = forExpressionNext;
    forControl->scope = scope;
    return ControlGrammarAction((ControlUnion) { .forControl = forControl }, FOR_CONTROL);
}

ControlNode * WhileControlGrammarAction(ExpressionNode * condition, ScopeNode * scope){
    WhileControl * whileControl = gcCalloc(sizeof(*whileControl));
    whileControl->condition = condition;
    whileControl->scope = scope;
    return ControlGrammarAction((ControlUnion) { .whileControl = whileControl }, WHILE_CONTROL);
}

IfControlNode * IfControlGrammarAction(ExpressionNode * condition, ScopeNode * scope, ElseControlNode * elseControl){
    IfControlNode * node = gcCalloc(sizeof(*node));
    node->condition = condition;
    node->scope = scope;
    node->elseControl = elseControl;
    return node;
}

ElseControlNode * ElseControlGrammarAction(ElseControlUnion data, bool isScope){
    ElseControlNode * node = gcCalloc(sizeof(*node));
    node->data = data;
    node->isScope = isScope;
    return node;
}

ForExpressionNode * ForExpressionGrammarAction(ForExpressionUnion expression, ForExpressionType type){
    ForExpressionNode * node = gcCalloc(sizeof(*node));
    node->expression = expression;
    node->type = type;
    return node;
}

ForExpressionNode * DeclarationForExpressionGrammarAction(variable_scope_t type, char * name, char * op, ExpressionNode * expression){
    Declaration * declaration = gcCalloc(sizeof(*declaration));
    declaration->type = type;
    declaration->name = name;
    declaration->op = op;
    declaration->expression = expression;
    return ForExpressionGrammarAction((ForExpressionUnion) { .declaration = declaration }, FOR_DECLARATION);
}

ForExpressionNode * AssignmentForExpressionGrammarAction(VariableNode * variable, char * op, ExpressionNode * expression){
    Assignment * assignment = gcCalloc(sizeof(*assignment));
    assignment->variable = variable;
    assignment->op = op;
    assignment->expression = expression;
    return ForExpressionGrammarAction((ForExpressionUnion) { .assignment = assignment }, FOR_ASSIGNMENT);
}

TryControlNode * TryControlGrammarAction(ScopeNode * tryScope, RetryControlNode * retryControl, char * exceptionName, ScopeNode * catchScope, ScopeNode * finallyScope){
    TryControlNode * node = gcCalloc(sizeof(*node));
    node->tryScope = tryScope;
    node->retryControl = retryControl;
    node->exceptionName = exceptionName;
    node->catchScope = catchScope;
    node->finallyScope = finallyScope;
    return node;
}

RetryControlNode * RetryControlGrammarAction(ScopeNode * retryScope, unsigned retryCount, ExceptionSetNode * exceptionSet){
    RetryControlNode * node = gcCalloc(sizeof(*node));
    node->retryScope = retryScope;
    node->retryCount = retryCount;
    node->exceptionSet = exceptionSet;
    return node;
}

ExceptionSetNode * ExceptionSetGrammarAction(VariableNode * variable, ExceptionSetNode * next){
    ExceptionSetNode * node = gcCalloc(sizeof(*node));
    node->exception = variable;
    node->next = next;
    return node;
}
