#include "bison-actions.h"
/**
 * Implementación de "bison-actions.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogErrorRaw("[ERROR] Mensaje: '%s', debido a '", string);
	for (int i = 0; i < yyleng; ++i) {
		switch (yytext[i]) {
			case '\n':
				LogErrorRaw("\\n");
			default:
				LogErrorRaw("%c", yytext[i]);
		}
	}
	LogErrorRaw("' (length = %d, linea %d).\n\n", yyleng, yylineno);
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
Program * ProgramGrammarAction(ProgramUnion p, ProgramType type) {
	LogDebug("[Bison] ProgramGrammarAction");
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	Program * program = calloc(1, sizeof(*program));
	program->program = p;
	program->type = type;
	state.succeed = true;
	return program;
}

SuiteNode * SuiteGrammarAction(char * name, ModuleListNode * moduleList) {
	SuiteNode * suite = calloc(1, sizeof(*suite));
	suite->name = name;
	suite->moduleList = moduleList;
	return suite;
}

ModuleListNode * ModuleListGrammarAction(ModuleListNode * moduleList, ModuleNode * module) {
	ModuleListNode * node = calloc(1, sizeof(*node));
	node->moduleList = moduleList;
	node->module = module;
	return node;
}

ModuleNode * ModuleGrammarAction(char * name, ModuleType type, ScopeNode * scope) {
	ModuleNode * module = calloc(1, sizeof(*module));
	module->name = name;
	module->type = type;
	module->scope = scope;
	return module;
}

ScopeNode * ScopeGrammarAction(StatementListNode * statementList) {
	ScopeNode * scope = calloc(1, sizeof(*scope));
	scope->statementList = statementList;
	return scope;
}

StatementListNode * StatementListGrammarAction(StatementListNode * statementList, StatementNode * statement) {
	StatementListNode * node = calloc(1, sizeof(*node));
	node->statementList = statementList;
	node->statement = statement;
	return node;
}

StatementNode * StatementGrammarAction(StatementUnion statementUnion, StatementType type) {
	StatementNode * node = calloc(1, sizeof(*node));
	node->statementUnion = statementUnion;
	node->type = type;
	return node;
}

StatementNode * DeclarationStatementGrammarAction(variable_scope_t type, char * name, char * op, ExpressionNode * expression) {
	Declaration * declaration = calloc(1, sizeof(*declaration));
	declaration->type = type;
	declaration->name = name;
	declaration->op = op;
	declaration->expression = expression;
	return StatementGrammarAction((StatementUnion) { .declaration = declaration } , STATEMENT_DECLARATION);
}

StatementNode * AssignmentStatementGrammarAction(VariableNode * variable, char * op, ExpressionNode * expression) {
	Assignment * assignment = calloc(1, sizeof(*assignment));
	assignment->variable = variable;
	assignment->op = op;
	assignment->expression = expression;
	return StatementGrammarAction((StatementUnion) { .assignment = assignment }, STATEMENT_ASSIGNMENT);
}

StatementNode * AssertionStatementGrammarAction(assertion_t type, ExpressionNode * expression, ExpressionNode * expected) {
	Assertion * assertion = calloc(1, sizeof(*assertion));
	assertion->type = type;
	assertion->expression = expression;
	assertion->expected = expected;
	return StatementGrammarAction((StatementUnion) { .assertion = assertion }, STATEMENT_ASSERTION);
}