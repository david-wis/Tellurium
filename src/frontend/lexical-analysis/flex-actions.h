#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios multilínea.
void BeginCommentPatternAction();
// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);

void EndCommentPatternAction();

token StringPatternAction(const char * lexeme, const int length);

token BeginSequencePatternAction();

token KeyDownPatternAction();

token KeyUpPatternAction();

token KeyPatternAction(const char * lexeme, actionkey_t key);

token EndSequencePatternAction();

token BinaryOperatorPatternAction(const char * lexeme, const int length);

token UnaryOperatorPatternAction(const char * lexeme, const int length);

token AssignmentOperatorPatternAction(const char * lexeme, const int length);

token OpenParenthesisPatternAction();

token CloseParenthesisPatternAction();

token OpenBracePatternAction();

token CloseBracePatternAction();

token OpenBracketPatternAction();

token CloseBracketPatternAction();

token CommaPatternAction();

token SemicolonPatternAction();

token ColonPatternAction();

token DotPatternAction();

token XPathOperatorPatternAction();

// Patrones terminales del lenguaje diseñado.
token ModulePatternAction();

token SuitePatternAction();

token BeforeAllPatternAction();

token AfterAllPatternAction();

token AssertPatternAction();

token AssertComparePatternAction();

token RetryPatternAction();

token TryPatternAction();

token CatchPatternAction();

token FinallyPatternAction();

token IfPatternAction();

token ElsePatternAction();

token ForPatternAction();

token WhilePatternAction();

token ArrowPatternAction();

token VarPatternAction();

token FunctionPatternAction();

token ReturnPatternAction();

token BooleanPatternAction(const char * lexeme, bool_t boolean);

token NamePatternAction(const char * lexeme, const int length);

token IntegerPatternAction(const char * lexeme, const int length);

token NumberPatternAction(const char * lexeme, const int length);

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

#endif
