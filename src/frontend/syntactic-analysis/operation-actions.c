#include "bison-actions.h"

OperationNode * OperationGrammarAction(BinaryOperatorNode * operator, OperationNode * left, OperandNode * right) {
    OperationNode * operation = gcCalloc(sizeof(*operation)); 
    operation->left = left;
    operation->right = right;
    operation->operator = operator;
    return operation;
}

UnaryOperatorNode * UnaryOperatorGrammarAction(UnaryOperator operator, char * op) {
    UnaryOperatorNode * unaryOperator = gcCalloc(sizeof(*unaryOperator));
    unaryOperator->operator = operator;
    unaryOperator->op = op;
    return unaryOperator;   
}

BinaryOperatorNode * BinaryOperatorGrammarAction(BinaryOperator operator, char * op) {
    BinaryOperatorNode * binaryOperator = gcCalloc(sizeof(*binaryOperator));
    binaryOperator->operator = operator;
    binaryOperator->op = op;
    return binaryOperator;   
}

OperandNode * OperandGrammarAction(OperandType type, OperandUnion data, UnaryOperatorNode * unaryOperator){
    OperandNode * operand = gcCalloc(sizeof(*operand));
    operand->type = type;
    operand->data = data;
    operand->unaryOperator = unaryOperator;
    return operand;
}
