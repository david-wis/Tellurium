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
%x SEQUENCE
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

"<|"                      { BEGIN(SEQUENCE); BeginSequencePatternAction(); }
<SEQUENCE>"+"             { return KeyDownPatternAction(); }        
<SEQUENCE>"-"             { return KeyUpPatternAction(); } 

<SEQUENCE>"CONTROL"       { return KeyPatternAction(yytext, CONTROL); }
<SEQUENCE>"SHIFT"         { return KeyPatternAction(yytext, SHIFT); }
<SEQUENCE>"ALT"           { return KeyPatternAction(yytext, ALT); }

<SEQUENCE>"LEFT"          { return KeyPatternAction(yytext, LEFT); }
<SEQUENCE>"RIGHT"         { return KeyPatternAction(yytext, RIGHT); }
<SEQUENCE>"UP"            { return KeyPatternAction(yytext, UP); }   
<SEQUENCE>"DOWN"          { return KeyPatternAction(yytext, DOWN); }

<SEQUENCE>"ENTER"         { return KeyPatternAction(yytext, ENTER); }
<SEQUENCE>"SPACE"         { return KeyPatternAction(yytext, SPACE); }  
<SEQUENCE>"BACKSPACE"     { return KeyPatternAction(yytext, BACKSPACE); }
<SEQUENCE>"DELETE"        { return KeyPatternAction(yytext, DELETE); } 
<SEQUENCE>"TAB"           { return KeyPatternAction(yytext, TAB); }
<SEQUENCE>"ESCAPE"        { return KeyPatternAction(yytext, ESCAPE); }
<SEQUENCE>"PAGEUP"        { return KeyPatternAction(yytext, PAGE_UP); }
<SEQUENCE>"PAGEDOWN"      { return KeyPatternAction(yytext, PAGE_DOWN); }
<SEQUENCE>"END"           { return KeyPatternAction(yytext, END); }
<SEQUENCE>"HOME"          { return KeyPatternAction(yytext, HOME); }

<SEQUENCE>{string}        { return StringPatternAction(yytext, yyleng); }

<SEQUENCE>"|>"            { EndSequencePatternAction(); BEGIN(INITIAL); }

")"{whitespace}*"=>"              { return ArrowPatternAction(); }

"+"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
"-"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
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
