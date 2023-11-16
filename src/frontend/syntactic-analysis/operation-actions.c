#include "bison-actions.h"

OperationNode * OperationGrammarAction(BinaryOperatorNode * operator, OperationNode * left, OperandNode * right) {
    OperationNode * operation = calloc(1, sizeof(*operation)); 
    operation->left = left;
    operation->right = right;
    operation->operator = operator;
    return operation;
}

UnaryOperatorNode * UnaryOperatorGrammarAction(UnaryOperator operator, char * op) {
    UnaryOperatorNode * unaryOperator = calloc(1, sizeof(*unaryOperator));
    unaryOperator->operator = operator;
    unaryOperator->op = op;
    return unaryOperator;   
}

BinaryOperatorNode * BinaryOperatorGrammarAction(BinaryOperator operator, char * op) {
    BinaryOperatorNode * binaryOperator = calloc(1, sizeof(*binaryOperator));
    binaryOperator->operator = operator;
    binaryOperator->op = op;
    return binaryOperator;   
}

OperandNode * OperandGrammarAction(OperandType type, OperandUnion data, UnaryOperatorNode * unaryOperator){
    OperandNode * operand = calloc(1, sizeof(*operand));
    operand->type = type;
    operand->data = data;
    operand->unaryOperator = unaryOperator;
    return operand;
}
