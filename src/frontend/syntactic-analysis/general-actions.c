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
	Program * program = gcCalloc(sizeof(*program));
	program->program = p;
	program->type = type;
	if (!isEmpty(state.errorMessages)) {
		state.succeed = false;
	} else {
		state.succeed = true;
	}
	state.program = program;
	return program;
}

SuiteNode * SuiteGrammarAction(char * name, ModuleListNode * moduleList) {
	SuiteNode * suite = gcCalloc(sizeof(*suite));
	suite->name = name;
	suite->moduleList = moduleList;
	return suite;
}

ModuleListNode * ModuleListGrammarAction(ModuleListNode * moduleList, ModuleNode * module) {
	ModuleListNode * node = gcCalloc(sizeof(*node));
	node->moduleList = moduleList;
	node->module = module;
	return node;
}

static bool cmpStr(void * a, void * b) {
	return strcmp((char*) a, (char *) b);
}

ModuleNode * ModuleGrammarAction(char * name, ModuleType type, ScopeNode * scope) {
	ModuleNode * module = gcCalloc(sizeof(*module));
	module->name = name;
	module->type = type;
	if (module->type == MODULE_AFTER_ALL) {
		if (state.afterAll != NULL) {
			LogError("%d: Ya se declaró un módulo 'after all'.", yylineno);
			addError("Ya se declaró un módulo 'after all'.");
			state.succeed = false;
		} else {
			state.afterAll = module;

		}
	} else if (module->type == MODULE_BEFORE_ALL) {
		if (state.beforeAll != NULL) {
			LogError("%d: Ya se declaró un módulo 'before all'.", yylineno);
			addError("Ya se declaró un módulo 'before all'.");
			state.succeed = false;
		} else {
			LogDebug("before all asignado!!!");
			state.beforeAll = module;
		}
	}
	module->scope = scope;
	if (name != NULL) {
		bool isPresent = contains(state.modules, module->name, cmpStr);
		if (isPresent) {
			LogError("%d: El módulo '%s' ya fue declarado.", yylineno, module->name);
			addError("El módulo ya fue declarado.");
			state.succeed = false;
		} else {
			appendElement(state.modules, module->name);
		}
	}
	return module;
}

ScopeNode * ScopeGrammarAction(StatementListNode * statementList) {
	ScopeNode * scope = gcCalloc(sizeof(*scope));
	scope->statementList = statementList;
	return scope;
}

StatementListNode * StatementListGrammarAction(StatementListNode * statementList, StatementNode * statement) {
	StatementListNode * node = gcCalloc(sizeof(*node));
	node->statementList = statementList;
	node->statement = statement;
	return node;
}

StatementNode * StatementGrammarAction(StatementUnion statementUnion, StatementType type) {
	StatementNode * node = gcCalloc(sizeof(*node));
	node->statementUnion = statementUnion;
	node->type = type;
	return node;
}

StatementNode * DeclarationStatementGrammarAction(variable_scope_t type, char * name, char * op, ExpressionNode * expression) {
	Declaration * declaration = gcCalloc(sizeof(*declaration));
	declaration->type = type;
	declaration->name = name;
	declaration->op = op;
	declaration->expression = expression;
	return StatementGrammarAction((StatementUnion) { .declaration = declaration } , STATEMENT_DECLARATION);
}

StatementNode * AssignmentStatementGrammarAction(VariableNode * variable, char * op, ExpressionNode * expression) {
	Assignment * assignment = gcCalloc(sizeof(*assignment));
	assignment->variable = variable;
	assignment->op = op;
	assignment->expression = expression;
	return StatementGrammarAction((StatementUnion) { .assignment = assignment }, STATEMENT_ASSIGNMENT);
}

StatementNode * AssertionStatementGrammarAction(assertion_t type, ExpressionNode * expression, ExpressionNode * expected) {
	Assertion * assertion = gcCalloc(sizeof(*assertion));
	assertion->type = type;
	assertion->expression = expression;
	assertion->expected = expected;
	return StatementGrammarAction((StatementUnion) { .assertion = assertion }, STATEMENT_ASSERTION);
}