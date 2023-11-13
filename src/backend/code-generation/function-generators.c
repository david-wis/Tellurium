#include "../support/logger.h"
#include "generator.h"
#include <stdlib.h>
#include <stdio.h>
extern FILE * outputFile;

void FunctionGenerate(FunctionNode * function) {
    LogDebug("\tFunctionGenerate %s\n", function->name);
    if (function->async) {
        fputs("async ", outputFile);
    }
    fprintf(outputFile, "function %s(", function->name);

    if (function->parameters != NULL) {
        ParameterDefinitionGenerate(function->parameters);
    }
    fputs(") ", outputFile);
    ScopeGenerate(function->scope);
}


void ParameterDefinitionGenerate(ParameterDefinitionNode * parameterDefinition) {
    LogDebug("\tParameterDefinitionGenerate %s\n", parameterDefinition->name);
    if (parameterDefinition->next != NULL) {
        ParameterDefinitionGenerate(parameterDefinition->next);
        fputs(", ", outputFile);
    }
    fprintf(outputFile, "%s", parameterDefinition->name);
}

void LambdaGenerate(LambdaNode * lambda) {
    LogDebug("\tLambdaGenerate\n");
    fputs("(", outputFile);
    if (lambda->parameters != NULL) {
        ParameterDefinitionGenerate(lambda->parameters);
    }
    fputs(") => ", outputFile);
    ScopeGenerate(lambda->scope);
}