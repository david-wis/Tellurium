#include "../support/logger.h"
#include "generator.h"
#include <stdlib.h>
#include <stdio.h>

extern FILE * outputFile;

int retryVariableCount = 0;

static void ForControlGenerateAux(ForControl * forControl);
static void WhileControlGenerateAux(WhileControl * whileControl);
static void RetryControlGenerateAux(TryControlNode * tryControl);

void ControlGenerate(ControlNode * control) {
    LogDebug("\tControlGenerate\n");
    switch (control->type) {
        case IF_CONTROL:
            IfControlGenerate(control->control.ifControl);
            break;
        case WHILE_CONTROL:
            WhileControlGenerateAux(control->control.whileControl);
            break;
        case FOR_CONTROL:
            ForControlGenerateAux(control->control.forControl);
            break;
        case TRY_CONTROL:
            TryControlGenerate(control->control.tryControl);
            break;
        default:
            break;
    }
}

static void ForControlGenerateAux(ForControl * forControl) {
    LogDebug("\tForControlGenerateAux\n");
    fputs("for (", outputFile);
    ForExpressionGenerate(forControl->forExpressionStart);
    fputs("; ", outputFile);
    ForExpressionGenerate(forControl->forExpressionCondition);
    fputs("; ", outputFile);
    ForExpressionGenerate(forControl->forExpressionNext);
    fputc(')', outputFile);
    ScopeGenerate(forControl->scope);
}

static void WhileControlGenerateAux(WhileControl * whileControl) {
    LogDebug("\tWhileControlGenerateAux\n");
    fputs("while (", outputFile);
    ExpressionGenerate(whileControl->condition);
    fputc(')', outputFile);
    ScopeGenerate(whileControl->scope);
}

void IfControlGenerate(IfControlNode * ifControl) {
    LogDebug("\tIfControlGenerate\n");
    fputs("if (", outputFile);
    ExpressionGenerate(ifControl->condition);
    fputc(')', outputFile);
    ScopeGenerate(ifControl->scope);
    if (ifControl->elseControl != NULL) ElseControlGenerate(ifControl->elseControl);
}

void ElseControlGenerate(ElseControlNode * elseControl) {
    LogDebug("\tElseControlGenerate\n");
    fputs("else ", outputFile);
    if (!elseControl->isScope) IfControlGenerate(elseControl->data.ifControl);
    else ScopeGenerate(elseControl->data.scope);
}

void ForExpressionGenerate(ForExpressionNode * forExpression) {
    LogDebug("\tForExpressionGenerate\n");
    switch (forExpression->type) {
        case FOR_EXPRESSION:
            ExpressionGenerate(forExpression->expression.expression);
            break;
        case FOR_ASSIGNMENT:
            AssignmentGenerateAux(forExpression->expression.assignment->variable, 
                                  forExpression->expression.assignment->op, 
                                  forExpression->expression.assignment->expression);
            break;
        case FOR_DECLARATION:
            DeclarationGenerateAux(forExpression->expression.declaration->type, 
                                   forExpression->expression.declaration->name, 
                                   forExpression->expression.declaration->op, 
                                   forExpression->expression.declaration->expression);
        default: // FOR_EMPTY
            break;
    }
}

void TryControlGenerate(TryControlNode * tryControl) {
    LogDebug("\tTryControlGenerate\n");
    if (tryControl->retryControl != NULL) {
        RetryControlGenerateAux(tryControl);
    } else{
        fputs("try ", outputFile);
        ScopeGenerate(tryControl->tryScope);
    }
    fprintf(outputFile, " catch (%s) ", tryControl->exceptionName);    
    ScopeGenerate(tryControl->catchScope);
    if (tryControl->finallyScope != NULL) {
        fputs(" finally ", outputFile);
        ScopeGenerate(tryControl->finallyScope);
    }
}

static void RetryControlGenerateAux(TryControlNode * tryControl) {
    retryVariableCount++;
    RetryControlNode * retryControl = tryControl->retryControl;
    fputs("try {\n", outputFile);
    fprintf(outputFile, "let tellurium_finished%d = false;\n", retryVariableCount);
    fprintf(outputFile, "let tellurium_retry%d = %d;\n", retryVariableCount, retryControl->retryCount);
    fprintf(outputFile, "for (let tellurium_i%d = 0; !tellurium_finished%d && tellurium_i%d < tellurium_retry%d; tellurium_i%d++) {\n", 
            retryVariableCount, retryVariableCount, retryVariableCount, retryVariableCount, retryVariableCount);
    fputs("try {\n", outputFile); 
    ScopeGenerate(tryControl->tryScope);
    fprintf(outputFile, "tellurium_finished%d = true;\n", retryVariableCount);
    fprintf(outputFile, "} catch (tellurium_e%d) {\n", retryVariableCount);
    fputs("if (", outputFile);
    ExceptionSetGenerate(retryControl->exceptionSet);
    fputs(") continue;\n", outputFile);
    fprintf(outputFile, "throw tellurium_e%d;\n}\n", retryVariableCount);
    fputs("}\n", outputFile);
    fprintf(outputFile, "if (!tellurium_finished%d) {\n", retryVariableCount);
    ScopeGenerate(retryControl->retryScope);
    fputs("}\n", outputFile);
    fputs("}\n", outputFile);
    retryVariableCount--;
}

void ExceptionSetGenerate(ExceptionSetNode * exceptionSet) {
    if (exceptionSet->next != NULL) {
        ExceptionSetGenerate(exceptionSet->next);
        fputs(" || ", outputFile);
    }
    fprintf(outputFile, "tellurium_e%d instanceof ", retryVariableCount);
    VariableGenerate(exceptionSet->exception);
}

//e instanceof NoSuchElementError || error instanceof StaleElementReferenceError