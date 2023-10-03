%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT
/* %x TEMPLATE_STRING */

/* Patrones reutilizables. */
string          \"([^\\\"]|\\.)*\"  
digit			      [0-9]
endline			    [(\r)?\n]
whitespace		  [ \f\n\r\t\v]
char            [a-zA-Z]
name            {char}({char}|{digit})*
number           [-]?({digit}*[.])?{digit}+
single_line_comment  "//".*
/* url             [-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*) */

%%

"/*"										  { BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.								{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+				{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"							{ EndCommentPatternAction(); BEGIN(INITIAL); }

{single_line_comment}    { IgnoredPatternAction(yytext, yyleng); }  

"<|"            { return BeginSequencePatternAction(); }

"CONTROL"       { return KeyPatternAction(yytext, CONTROL); }
"SHIFT"         { return KeyPatternAction(yytext, SHIFT); }
"ALT"           { return KeyPatternAction(yytext, ALT); }

"LEFT"          { return KeyPatternAction(yytext, LEFT); }
"RIGHT"         { return KeyPatternAction(yytext, RIGHT); }
"UP"            { return KeyPatternAction(yytext, UP); }   
"DOWN"          { return KeyPatternAction(yytext, DOWN); }

"ENTER"         { return KeyPatternAction(yytext, ENTER); }
"SPACE"         { return KeyPatternAction(yytext, SPACE); }  
"BACKSPACE"     { return KeyPatternAction(yytext, BACKSPACE); }
"DELETE"        { return KeyPatternAction(yytext, DELETE); } 
"TAB"           { return KeyPatternAction(yytext, TAB); }
"ESCAPE"        { return KeyPatternAction(yytext, ESCAPE); }
"PAGEUP"        { return KeyPatternAction(yytext, PAGE_UP); }
"PAGEDOWN"      { return KeyPatternAction(yytext, PAGE_DOWN); }
"END"           { return KeyPatternAction(yytext, END); }
"HOME"          { return KeyPatternAction(yytext, HOME); }

"|>"            { return EndSequencePatternAction(); }


")"{whitespace}*"=>"                    { return ArrowPatternAction(); }

"+"											  { return PlusPatternAction(); }
"-"											  { return MinusPatternAction(); }
"*"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
"/"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
"%"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
"<"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
">"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
"<="											{ return BinaryOperatorPatternAction(yytext, yyleng); }
">="											{ return BinaryOperatorPatternAction(yytext, yyleng); }
"&&"											{ return BinaryOperatorPatternAction(yytext, yyleng); }
"||"											{ return BinaryOperatorPatternAction(yytext, yyleng); }
"=="									    { return BinaryOperatorPatternAction(yytext, yyleng); }
"==="									    { return BinaryOperatorPatternAction(yytext, yyleng); }
"!="									    { return BinaryOperatorPatternAction(yytext, yyleng); }
"!=="									    { return BinaryOperatorPatternAction(yytext, yyleng); }
"|"											{ return PipePatternAction(); }

"!"											  { return UnaryOperatorPatternAction(yytext, yyleng); }

"="											      { return AssignmentOperatorPatternAction(yytext, yyleng); }
"+="											  { return AssignmentOperatorPatternAction(yytext, yyleng); }
"-="											  { return AssignmentOperatorPatternAction(yytext, yyleng); }
"*="											  { return AssignmentOperatorPatternAction(yytext, yyleng); }
"/="											  { return AssignmentOperatorPatternAction(yytext, yyleng); }
"%="											  { return AssignmentOperatorPatternAction(yytext, yyleng); }
"&&="									          { return AssignmentOperatorPatternAction(yytext, yyleng); }
"||="										      { return AssignmentOperatorPatternAction(yytext, yyleng); }
"("											  { return OpenParenthesisPatternAction(); }
")"											  { return CloseParenthesisPatternAction(); }
"{"											  { return OpenBracePatternAction(); } 
"}"											  { return CloseBracePatternAction(); } 
"["											  { return OpenBracketPatternAction(); } 
"]"											  { return CloseBracketPatternAction(); } 
","											  { return CommaPatternAction(); }
";"											  { return SemicolonPatternAction(); }
":"											  { return ColonPatternAction(); }
"."											  { return DotPatternAction(); }

"$("                      { return XPathOperatorPatternAction(); }

"Module"                  { return ModulePatternAction(); }
"Suite"                   { return SuitePatternAction(); }
"BeforeAll"               { return BeforeAllPatternAction(); }
"AfterAll"                { return AfterAllPatternAction(); }

"assertTrue"              { return AssertPatternAction(); }
"assertFalse"             { return AssertPatternAction(); }
"assertEquals"            { return AssertComparePatternAction(); }
"assertNotEquals"         { return AssertComparePatternAction(); }

"retry"                   { return RetryPatternAction(); }
"try"                     { return TryPatternAction(); }
"catch"                   { return CatchPatternAction(); }
"finally"                 { return FinallyPatternAction(); }

"if"                      { return IfPatternAction(); }
"else"                    { return ElsePatternAction(); }
"while"                   { return WhilePatternAction(); }
"for"                     { return ForPatternAction(); }

"var"                     { return VarPatternAction(); }
"function"                { return FunctionPatternAction(); }
"return"                  { return ReturnPatternAction(); }

"true"                    { return BooleanPatternAction(yytext, TRUE); }
"false"                   { return BooleanPatternAction(yytext, FALSE); }

{name}                      { return NamePatternAction(yytext, yyleng); }
{digit}+				    { return IntegerPatternAction(yytext, yyleng); }
{number}				    { return NumberPatternAction(yytext, yyleng); }
{string}                    { return StringPatternAction(yytext, yyleng); }

{whitespace}+				{ IgnoredPatternAction(yytext, yyleng); }

.							{ return UnknownPatternAction(yytext, yyleng); }

%%
