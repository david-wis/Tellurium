#include "../support/logger.h"
#include "generator.h"
#include <stdlib.h>
#include <stdio.h>
extern FILE * outputFile;

void OperationGenerate(OperationNode * operation) {
    LogDebug("\tOperationGenerate\n");
    if (operation->left == NULL && operation->operator.noOp != NULL) {
        UnaryOperatorGenerate(operation->operator.unaryOp);
    } else if (operation->operator.noOp != NULL) {
        OperationGenerate(operation->left);
        BinaryOperatorGenerate(operation->operator.binaryOp);
    }
    OperandGenerate(operation->right);
}

void UnaryOperatorGenerate(UnaryOperatorNode * unaryOperator) {
    LogDebug("\tUnaryOperatorGenerate\n");
    switch (unaryOperator->operator) {
        case UNARY_AWAIT:
            fputs("await", outputFile);
            break;
        case UNARY_MINUS:
            fputs("-", outputFile);
            break;
        case UNARY_GENERIC:
            fputs(unaryOperator->op, outputFile);
            break;
        default:
            break;
    }

}

void BinaryOperatorGenerate(BinaryOperatorNode * binaryOperator) {
    //TODO analizar el caso de sequence
    LogDebug("\tBinaryOperatorGenerate\n");
    switch (binaryOperator->operator) {
        case BINARY_PLUS:
            fputs("+", outputFile);
            break;
        case BINARY_MINUS:
            fputs("-", outputFile);
            break;
        case BINARY_MULTIPLICATION:
            fputs("*", outputFile);
            break;
        case BINARY_GENERIC:
            fputs(binaryOperator->op, outputFile);
            break;
        default:
            break;
    }
}

void OperandGenerate(OperandNode * operand) {
    LogDebug("\tOperandGenerate\n");
    switch (operand->type) {
        case LITERAL:
            LiteralGenerate(operand->data.literal);
            break;
        case OBJECT:
            ObjectGenerate(operand->data.object);
            break;
        case ATTRIBUTE_LIST:
            fputs("{\n", outputFile);
            AttributeListGenerate(operand->data.attributes);
            fputs("\n}", outputFile);
            break;
        default:
            break;
    }

}