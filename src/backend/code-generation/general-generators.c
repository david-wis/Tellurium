#include "../support/logger.h"
#include "generator.h"
#include <stdlib.h>
#include <stdio.h>

extern FILE * outputFile;

static void AssertionGenerateAux(Assertion * assertion);

void ProgramGenerate(Program * program) {
    LogDebug("\tProgramGenerate\n");
	switch (program->type) {
		case PROG_SUITE:
			SuiteGenerate(program->program.suite);
			break;
		case PROG_STATEMENT_LIST:
			StatementListGenerate(program->program.statementList);
			break;
		default:
            break;
	}
}

void SuiteGenerate(SuiteNode * suite) {
	LogDebug("\tSuiteGenerate\n");
	if (suite->name != NULL)
		fprintf(outputFile, "suiteName = \"%s\";\n", suite->name);
	ModuleListGenerate(suite->moduleList);
}

void ModuleListGenerate(ModuleListNode * moduleList) {
	LogDebug("\tModuleListGenerate\n");
	if (moduleList->module == NULL && moduleList->moduleList == NULL) {
		return;
	}
	ModuleListGenerate(moduleList->moduleList);
	ModuleGenerate(moduleList->module);
}

void ModuleGenerate(ModuleNode * module){
	LogDebug("\tModuleGenerate\n");
	if (module->type == MODULE_AFTER_ALL || module->type == MODULE_BEFORE_ALL) return;

	fputs("await (async function() {\n", outputFile);
	fputs("\t//starting BeforeAll\n", outputFile);
	if (state.beforeAll != NULL) {
		StatementListGenerate(state.beforeAll->scope->statementList);
	}
	fputs("\t//ending BeforeAll\n", outputFile);

	fputs("tellurium_suite_state.success = true;\n", outputFile);
	if (module->name != NULL) {
		fprintf(outputFile, "tellurium_suite_state.name = \"%s\";\n", module->name);
	}
	else  
		fprintf(outputFile, "tellurium_suite_state.name = \"Tellurium test module\";\n");
	fputs("tellurium_suite_state.count++;\n", outputFile);
	fputs("//starting module\n", outputFile);
	fputs("try ", outputFile);	
	ScopeGenerate(module->scope);
	fputs("//ending module\n", outputFile);
	fputs("catch (error) {\n", outputFile);
	fputs("\tconsole.error(`Module ${tellurium_suite_state.name} failed ${error}`)\n", outputFile);
	fputs("\ttellurium_suite_state.success = false;\n", outputFile);
	fputs("} finally {\n", outputFile);
	fputs("\tif (tellurium_suite_state.success) { tellurium_suite_state.passedCount++;\n", outputFile);
	fputs("\tconsole.info(`Module ${tellurium_suite_state.name} passed`);\n}\n", outputFile);
	fputs("\t//starting AfterAll\n", outputFile);
	if (state.afterAll != NULL)
		StatementListGenerate(state.afterAll->scope->statementList);
	fputs("\t//starting AfterAll\n", outputFile);
	fputs("}\n", outputFile);


	fputs("})();\n", outputFile);
}

void ScopeGenerate(ScopeNode * scope) {
	LogDebug("\tScopeGenerate\n");
    fputs("{\n", outputFile);
    StatementListGenerate(scope->statementList);
    fputc('}', outputFile);
}

void StatementListGenerate(StatementListNode * statementList) {
	LogDebug("\tStatementListGenerate\n");
    if (statementList->statement == NULL && statementList->statementList == NULL) {
        return;
    }
	StatementListGenerate(statementList->statementList);
	StatementGenerate(statementList->statement);
}

void StatementGenerate(StatementNode * statement) {
    LogDebug("\tStatementGenerate\n");
	if (statement->type != STATEMENT_FUNCTION && statement->type != STATEMENT_CONTROL) {
		switch(statement->type) {
			case STATEMENT_EXPRESSION:
				ExpressionGenerate(statement->statementUnion.expression);
				break;
			case STATEMENT_DECLARATION:
				DeclarationGenerateAux(statement->statementUnion.declaration->type, 
									   statement->statementUnion.declaration->name,
									   statement->statementUnion.declaration->op,
									   statement->statementUnion.declaration->expression);
				break;
			case STATEMENT_ASSIGNMENT:
				AssignmentGenerateAux(statement->statementUnion.assignment->variable,
									  statement->statementUnion.assignment->op,
									  statement->statementUnion.assignment->expression);
				break;
			case STATEMENT_ASSERTION:
				AssertionGenerateAux(statement->statementUnion.assertion);
				break;
			case STATEMENT_RETURN:
				fputs("return ", outputFile);
				if (statement->statementUnion.expression) {
					ExpressionGenerate(statement->statementUnion.expression);
				}
				break;
			default:
				break;
		}
		fputc(';', outputFile);
	} else if (statement->type == STATEMENT_FUNCTION) {
		FunctionGenerate(statement->statementUnion.function);
	} else if (statement->type == STATEMENT_CONTROL) {	
		ControlGenerate(statement->statementUnion.control);
	}
	fputc('\n', outputFile);
}

void DeclarationGenerateAux(variable_scope_t type, char * name, char * op, ExpressionNode * expression) {
	LogDebug("\tDeclarationGenerateAux\n");
	if (type == VAR) {
		fputs("var ", outputFile);
	}
	else if (type == CONST) {
		fputs("const ", outputFile);
	}
	else if (type == LET) {
		fputs("let ", outputFile);
	}
	fprintf(outputFile, "%s", name);
	if (expression != NULL) {
		fputs(" = ", outputFile);
		ExpressionGenerate(expression);
	}
}

void AssignmentGenerateAux(VariableNode * variable, char * op, ExpressionNode * expression) {
	LogDebug("\tAssignmentGenerateAux\n");
	VariableGenerate(variable);
	fprintf(outputFile, "%s", op);
	ExpressionGenerate(expression);
}

static void AssertionGenerateAux(Assertion * assertion)  {
	switch (assertion->type) {
		case TRUTHY:
			fputs("telluriumAssert(true, ", outputFile);
			ExpressionGenerate(assertion->expression);
			fputs(", false)", outputFile);
			break;
		case FALSY:
			fputs("telluriumAssert(false, ", outputFile);
			ExpressionGenerate(assertion->expression);
			fputs(", false)", outputFile);
			break;
		case EQUAL:
			fputs("telluriumAssert(", outputFile);
			ExpressionGenerate(assertion->expected);
			fputs(", ", outputFile);
			ExpressionGenerate(assertion->expression);
			fputs(", true)", outputFile);
			break;
		case NOT_EQUAL:
			fputs("telluriumAssertNotEquals(", outputFile);
			ExpressionGenerate(assertion->expected);
			fputs(", ", outputFile);
			ExpressionGenerate(assertion->expression);
			fputs(")", outputFile);
			break;
		default:
			break;
	}
}