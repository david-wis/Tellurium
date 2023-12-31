#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>
#include <string.h>
#include "../../shared/types.h"

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

char * copyLexeme(const char * lexeme, const int length) {
	char * lexemeCopy = (char *) calloc(length + 1, sizeof(char));
	appendElement(state.elementsToFree, lexemeCopy);
	strncpy(lexemeCopy, lexeme, length);
	return lexemeCopy;
}

/*
*  COMMENT CONTEXT
*/

void BeginCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] BeginCommentPatternAction............................");
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogRaw("[DEBUG] [Flex] IgnoredPatternAction: '");
	LogText(lexemeCopy, length);
	LogRaw("' (length = %d).\n", length);
	free(lexemeCopy);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
	// No se emite ningún token.
}

void EndCommentPatternAction() {
	LogDebug("[Flex] [COMMENT] EndCommentPatternAction..............................");
}

/*
*  STRING CONTEXT
*/

token StringPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] StringPatternAction: '%s' (length = %d).", lexemeCopy, length);
	yylval.string = lexemeCopy;
	return STRING;
}

token BeginSequencePatternAction() {
	LogDebug("[Flex] BeginSequencePatternAction..........................");
	yylval.token = BEGIN_SEQUENCE;
	return BEGIN_SEQUENCE;
}

token KeyPatternAction(const char * lexeme, action_key_t key) {
	LogDebug("[Flex] KeyPatternAction: '%s' .", lexeme);
	yylval.key = key;
	return KEY;
}

token EndSequencePatternAction() {
	LogDebug("[Flex] EndSequencePatternAction............................");
	yylval.token = END_SEQUENCE;
	return END_SEQUENCE;
}

token BinaryOperatorPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] BinaryOperatorPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.operator = lexemeCopy;
	return BINARY_OPERATOR;
}

token PipePatternAction() {
	LogDebug("[Flex] PipePatternAction: '|'.");
	yylval.token = PIPE;
	return PIPE;
}

token PlusPatternAction() {
	LogDebug("[Flex] PlusPatternAction: '+'.");
	yylval.token = PLUS;
	return PLUS;
}

token MinusPatternAction() {
	LogDebug("[Flex] PlusPatternAction: '-'.");
	yylval.token = MINUS;
	return MINUS;
}

token MultiplicationPatternAction() {
	LogDebug("[Flex] MultiplicationPatternAction: '*'.");
	yylval.token = MULTIPLICATION;
	return MULTIPLICATION;
}

token UnaryOperatorPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] UnaryOperatorPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.operator = lexemeCopy;
	return UNARY_OPERATOR;
}

token AssignmentOperatorPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] AssignmentOperatorPatternAction.");
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.operator = lexemeCopy;
	return ASSIGNMENT_OPERATOR;
}

token OpenParenthesisPatternAction() {
	LogDebug("[Flex] OpenParenthesisPatternAction: '('.");
	yylval.token = OPEN_PARENTHESIS;
	return OPEN_PARENTHESIS;
}

token CloseParenthesisPatternAction() {
	LogDebug("[Flex] CloseParenthesisPatternAction: ')'.");
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token OpenBracePatternAction() {
	LogDebug("[Flex] OpenBracePatternAction: '{'.");
	yylval.token = OPEN_BRACE;
	return OPEN_BRACE;
}

token CloseBracePatternAction() {
	LogDebug("[Flex] CloseBracePatternAction: '}'.");
	yylval.token = CLOSE_BRACE;
	return CLOSE_BRACE;
}

token OpenBracketPatternAction() {
	LogDebug("[Flex] OpenBracketPatternAction: '['.");
	yylval.token = OPEN_BRACKET;
	return OPEN_BRACKET;
}

token CloseBracketPatternAction() {
	LogDebug("[Flex] CloseBracketPatternAction: ']'.");
	yylval.token = CLOSE_BRACKET;
	return CLOSE_BRACKET;
}

token CommaPatternAction() {
	LogDebug("[Flex] CommaPatternAction: ','.");
	yylval.token = COMMA;
	return COMMA;
}

token SemicolonPatternAction() {
	LogDebug("[Flex] SemicolonPatternAction: ';'.");
	yylval.token = SEMICOLON;
	return SEMICOLON;
}

token ColonPatternAction() {
	LogDebug("[Flex] ColonPatternAction: ':'.");
	yylval.token = COLON;
	return COLON;
}

token DotPatternAction() {
	LogDebug("[Flex] DotPatternAction: '.'.");
	yylval.token = DOT;
	return DOT;
}

token XPathOperatorPatternAction(cardinality_t cardinality) {
	LogDebug("[Flex] XPathOperatorPatternAction: '/'.");
	yylval.cardinality = cardinality; 
	return XPATH_OPERATOR;
}

token ModulePatternAction() {
	LogDebug("[Flex] ModulePatternAction.");
	yylval.token = MODULE;
	return MODULE;
}

token SuitePatternAction() {
	LogDebug("[Flex] SuitePatternAction.");
	yylval.token = SUITE;
	return SUITE;
}

token BeforeAllPatternAction() {
	LogDebug("[Flex] BeforeAllPatternAction.");
	yylval.token = BEFORE_ALL;
	return BEFORE_ALL;
}

token AfterAllPatternAction() {
	LogDebug("[Flex] AfterAllPatternAction.");
	yylval.token = AFTER_ALL;
	return AFTER_ALL;
}

token AssertPatternAction(assertion_t assertion) {
	LogDebug("[Flex] AssertPatternAction.");
	yylval.assertionType = assertion;
	return ASSERT;
}

token AssertComparePatternAction(assertion_t assertion) {
	LogDebug("[Flex] AssertComparePatternAction.");
	yylval.assertionType = assertion;
	return ASSERT_COMPARE;
}

token RetryPatternAction() {
	LogDebug("[Flex] RetryPatternAction.");
	yylval.token = RETRY;
	return RETRY;
}

token TryPatternAction() {
	LogDebug("[Flex] TryPatternAction.");
	yylval.token = TRY;
	return TRY;
}

token CatchPatternAction() {
	LogDebug("[Flex] CatchPatternAction.");
	yylval.token = CATCH;
	return CATCH;
}

token FinallyPatternAction() {
	LogDebug("[Flex] FinallyPatternAction.");
	yylval.token = FINALLY;
	return FINALLY;
}

token IfPatternAction() {
	LogDebug("[Flex] IfPatternAction.");
	yylval.token = IF;
	return IF;
}

token ElsePatternAction() {
	LogDebug("[Flex] ElsePatternAction.");
	yylval.token = ELSE;
	return ELSE;
}

token ForPatternAction() {
	LogDebug("[Flex] ForPatternAction.");
	yylval.token = FOR;
	return FOR;
}

token WhilePatternAction() {
	LogDebug("[Flex] WhilePatternAction.");
	yylval.token = WHILE;
	return WHILE;
}

token ArrowPatternAction() {
	LogDebug("[Flex] ArrowPatternAction.");
	yylval.token = ARROW;
	return ARROW;
}

token VarPatternAction(variable_scope_t variableScope) {
	LogDebug("[Flex] VarPatternAction.");
	yylval.variableScope = variableScope;
	return VARIABLE;
}

token FunctionPatternAction() {
	LogDebug("[Flex] FunctionPatternAction.");
	yylval.token = FUNCTION;
	return FUNCTION;
}

token AsyncPatternAction() {
	LogDebug("[Flex] AsyncPatternAction.");
	yylval.token = ASYNC;
	return ASYNC;
}

token AwaitPatternAction() {
	LogDebug("[Flex] AwaitPatternAction.");
	yylval.token = AWAIT;
	return AWAIT;
}

token ReturnPatternAction() {
	LogDebug("[Flex] ReturnPatternAction.");
	yylval.token = RETURN;
	return RETURN;
}

token BooleanPatternAction(const char * lexeme, bool boolean) {
	LogDebug("[Flex] BooleanPatternAction: '%s'.", lexeme);
	yylval.boolean = boolean; 
	return BOOLEAN;
} 

token NullPatternAction() {
	LogDebug("[Flex] NullPatternAction.");
	yylval.nullLiteral = NULL_VALUE;
	return NULL_LITERAL;
}

token UndefinedPatternAction() {
	LogDebug("[Flex] UndefinedPatternAction.");
	yylval.nullLiteral = UNDEFINED_VALUE;
	return UNDEFINED;
}

token NaNPatternAction() {
	LogDebug("[Flex] NaNPatternAction.");
	yylval.nullLiteral = NAN_VALUE;
	return NAN;
}

token NamePatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] NamePatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.name = lexemeCopy;
	return NAME;
}

token IntegerPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] IntegerPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.integer = atoi(lexemeCopy);
	// free(lexemeCopy);
	return INTEGER;
}

token NumberPatternAction(const char * lexeme, const int length) {
	LogDebug("[Flex] NumberPatternAction: '%s' (length = %d).", lexeme, length);
	char * lexemeCopy = copyLexeme(lexeme, length);
	yylval.number = lexemeCopy;
	return NUMBER;
}


token UnknownPatternAction(const char * lexeme, const int length) {
	char * lexemeCopy = copyLexeme(lexeme, length);
	LogDebug("[Flex] UnknownPatternAction: '%s' (length = %d).", lexemeCopy, length);
	// free(lexemeCopy);
	yylval.token = ERROR;
	// Al emitir este token, el compilador aborta la ejecución.
	return ERROR;
}
