#include "bison-actions.h"

ControlNode * ControlGrammarAction(ControlUnion control, ControlType type){
    return NULL;
}

ControlNode * ForControlGrammarAction(ForExpressionNode * forExpressionStart, ForExpressionNode * forExpressionCondition, ForExpressionNode * forExpressionNext, ScopeNode * scope){
    return NULL;
}

ControlNode * WhileControlGrammarAction(ExpressionNode * condition, ScopeNode * scope){
    return NULL;
}

IfControlNode * IfControlGrammarAction(ExpressionNode * condition, ScopeNode * scope, ElseControlNode * elseControl){
    return NULL;
}

ElseControlNode * ElseControlGrammarAction(ElseControlUnion data, bool_t isScope){
    return NULL;
}

ForExpressionNode * ForExpressionGrammarAction(ForExpressionUnion expression, ForExpressionType type){
    return NULL;
}

ForExpressionNode * DeclarationForExpressionGrammarAction(variable_scope_t type, char * name, char * op, ExpressionNode * expression){
    return NULL;
}

ForExpressionNode * AssignmentForExpressionGrammarAction(VariableNode * variable, char * op, ExpressionNode * expression){
    return NULL;
}

TryControlNode * TryControlGrammarAction(ScopeNode * tryScope, RetryControlNode * retryControl, char * exceptionName, ScopeNode * catchScope, ScopeNode * finallyScope){
    return NULL;
}

RetryControlNode * RetryControlGrammarAction(ScopeNode * retryScope, unsigned retryCount, ExceptionSetNode * exceptionSet){
    return NULL;
}

ExceptionSetNode * ExceptionSetGrammarAction(char * exception, ExceptionSetNode * next){
    return NULL;
}
