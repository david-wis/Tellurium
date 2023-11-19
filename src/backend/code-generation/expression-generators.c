#include "../support/logger.h"
#include "generator.h"
#include <stdlib.h>
#include <stdio.h>
extern FILE * outputFile;


void ExpressionGenerate(ExpressionNode * expression) {
    LogDebug("\tExpressionGenerate\n");
    if(expression == NULL) return;
    if(expression->isOperation) OperationGenerate(expression->expression.operation);
    else LambdaGenerate(expression->expression.lambda);
}

void ObjectGenerate(ObjectNode * object) {
    LogDebug("\tObjectGenerate\n");
    switch (object->type) {
        case OBJ_VARIABLE: 
            VariableGenerate(object->object.variable); 
            break;
        case OBJ_XPATH: 
            fputs(object->object.xpath->cardinality == ONE ? 
                                "(await findByXPath(" : "(await findManyByXPath(", 
                                 outputFile);
            ExpressionGenerate(object->object.xpath->expression);
            fputs("))", outputFile);
            break;
        case OBJ_EXPRESSION:
            fputc('(', outputFile);
            ExpressionGenerate(object->object.expression);
            fputc(')', outputFile);
            break;
        case OBJ_ASSIGNMENT:
            fputc('(', outputFile);
            VariableGenerate(object->object.assignment->variable);
            fprintf(outputFile, " %s ", object->object.assignment->op);
            ExpressionGenerate(object->object.assignment->expression);
            fputc(')', outputFile);
            break;
        case OBJ_ARRAY:
            fputc('[', outputFile);
            if (object->object.array->parameters != NULL) {
                ParametersGenerate(object->object.array->parameters);
            }
            fputc(']', outputFile);
            break;
        case OBJ_FUNCTION_CALL:
            ObjectGenerate(object->object.functionCall->callable);
            fputc('(', outputFile);
            if (object->object.functionCall->parameters != NULL) {
                ParametersGenerate(object->object.functionCall->parameters);
            }
            fputc(')', outputFile);
            break;
        default:
            break;
    }
}

void AttributeListGenerate(AttributeListNode * attributeList) {
    LogDebug("\tAttributeListGenerate\n");
    if (attributeList->attribute != NULL) {
        if (attributeList->next != NULL) {
            AttributeListGenerate(attributeList->next);
            fputs(",\n", outputFile);
        }
        AttributeGenerate(attributeList->attribute);
    }
}

void AttributeGenerate(AttributeNode * attribute) {
    LogDebug("\tAttributeGenerate\n");
    fprintf(outputFile, "%s: ", attribute->name);
    ExpressionGenerate(attribute->expression);
}

void VariableGenerate(VariableNode * variable) {
    LogDebug("\tVariableGenerate\n");
    if (variable->arraySubexpression == NULL && variable->object == NULL) {
        fprintf(outputFile, "%s", variable->name);
    } else {
        ObjectGenerate(variable->object);
        if (variable->arraySubexpression != NULL) {
            fputc('[', outputFile);
            ExpressionGenerate(variable->arraySubexpression);
            fputc(']', outputFile);
        } else {
            fprintf(outputFile, ".%s", variable->name);
        }
    }
}

void ParametersGenerate(ParametersNode * parameters) {
    LogDebug("\tParametersGenerate\n");
    if (parameters->next != NULL) {
        ParametersGenerate(parameters->next);
        fputs(", ", outputFile);
    }
    ExpressionGenerate(parameters->expression);
}

void LiteralGenerate(LiteralNode * literal) {
    LogDebug("\tLiteralGenerate\n");
    switch (literal->type) {
        case L_INTEGER:
            fprintf(outputFile, "%d", literal->literalUnion.integer);
            break;
        case L_STRING:
            fprintf(outputFile, "%s", literal->literalUnion.string);
            break;
        case L_BOOLEAN:
            if (literal->literalUnion.boolean) {
                fputs("true", outputFile);
            } else {
                fputs("false", outputFile);
            }
            break;
        case L_NIL:
            fputs("null", outputFile);
            break;
        case L_UNDEFINED:
            fputs("undefined", outputFile);
            break;
        case L_NAN:
            fputs("NaN", outputFile);
            break;
        case L_SEQUENCE:
            SequenceGenerate(literal->literalUnion.sequence);
            break;
        default:
            break;
    }
}