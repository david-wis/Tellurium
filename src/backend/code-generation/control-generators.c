#include "../support/logger.h"
#include "generator.h"
#include <stdlib.h>
#include <stdio.h>

extern FILE * outputFile;

static void ForControlGenerateAux(ForControl * forControl);
static void WhileControlGenerateAux(WhileControl * whileControl);

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
    LogDebug("\tlol\n");
    ForExpressionGenerate(forControl->forExpressionStart);
    fputs("; ", outputFile);
    ForExpressionGenerate(forControl->forExpressionNext);
    fputs("; ", outputFile);
    ForExpressionGenerate(forControl->forExpressionCondition);
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
    fputs("try ", outputFile);
    ScopeGenerate(tryControl->tryScope);
    fprintf(outputFile, " catch (%s) ", tryControl->exceptionName);    
    ScopeGenerate(tryControl->catchScope);
    // RetryControlGenerate(tryControl->retryControl);
    if (tryControl->finallyScope != NULL) {
        fputs(" finally ", outputFile);
        ScopeGenerate(tryControl->finallyScope);
    }
}

void RetryControlGenerate(RetryControlNode * retryControl) {
    // fputs("retry ", outputFile);
    // ScopeGenerate(retryControl->retryScope);
    // //TODO
}

void ExceptionSetGenerate(ExceptionSetNode * exceptionSet) {
    //TODO
}

