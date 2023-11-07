#include "bison-actions.h"

OperationNode * OperationGrammarAction(OperatorUnion operator, OperationNode * left, OperandNode * right) {
    return NULL;
}

UnaryOperatorNode * UnaryOperatorGrammarAction(UnaryOperator operator) {
    return NULL;   
}

BinaryOperatorNode * BinaryOperatorGrammarAction(BinaryOperator operator) {
    return NULL;
}

OperandNode * OperandGrammarAction(OperandType type, OperandUnion data){
    return NULL;
}
