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
	state.succeed = true;




	return NULL;
}

SuiteNode * SuiteGrammarAction(char * name, ModuleListNode * moduleList) {
	return NULL;
}

ModuleListNode * ModuleListGrammarAction(ModuleListNode * moduleList, ModuleNode * module) {
	return NULL;
}

ModuleNode * ModuleGrammarAction(char * name, ModuleType type, ScopeNode * scope) {
	return NULL;
}

ScopeNode * ScopeGrammarAction(StatementListNode * statementList) {
	return NULL;
}

StatementListNode * StatementListGrammarAction(StatementListNode * statementList, StatementNode * statement) {
	return NULL;
}

StatementNode * StatementGrammarAction(StatementUnion statementUnion, StatementType type) {
	return NULL;
}

StatementNode * DeclarationStatementGrammarAction(variable_scope_t type, char * name, char * op, ExpressionNode * expression) {
	return NULL;
}

StatementNode * AssignmentStatementGrammarAction(VariableNode * variable, char * op, ExpressionNode * expression) {
	return NULL;
}

StatementNode * AssertionStatementGrammarAction(assertion_t type, ExpressionNode * expression, ExpressionNode * expected) {
	return NULL;
}