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
%x STRING
%x SEQUENCE
/* %x TEMPLATE_STRING */

/* Patrones reutilizables. */
digit			      [0-9]
endline			    [(\r)?\n]
whitespace		  [ \f\n\r\t\v]
char            [a-zA-Z]
name            {char}({char}|{digit})*
number           [+-]?({digit}*[.])?{digit}+
/* url             [-a-zA-Z0-9@:%._\+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}\b([-a-zA-Z0-9()@:%_\+.~#?&//=]*) */

%%

"/*"										  { BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.								{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+				{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"							{ EndCommentPatternAction(); BEGIN(INITIAL); }

"\""                      { BEGIN(STRING); BeginStringPatternAction(); }
<STRING>[^"]*             { StringPatternAction(yytext, yyleng); }
<STRING>"\""              { EndStringPatternAction(); BEGIN(INITIAL); }

"<|"                      { BEGIN(SEQUENCE); BeginSequencePatternAction(); }
<SEQUENCE>"+"             { KeyDownPatternAction(); }        
<SEQUENCE>"-"             { KeyUpPatternAction(); } 

<SEQUENCE>"CONTROL"       { KeyPatternAction(yytext, CONTROL); }
<SEQUENCE>"SHIFT"         { KeyPatternAction(yytext, SHIFT); }
<SEQUENCE>"ALT"           { KeyPatternAction(yytext, ALT); }

<SEQUENCE>"LEFT"          { KeyPatternAction(yytext, LEFT); }
<SEQUENCE>"RIGHT"         { KeyPatternAction(yytext, RIGHT); }
<SEQUENCE>"UP"            { KeyPatternAction(yytext, UP); }   
<SEQUENCE>"DOWN"          { KeyPatternAction(yytext, DOWN); }

<SEQUENCE>"ENTER"         { KeyPatternAction(yytext, ENTER); }
<SEQUENCE>"SPACE"         { KeyPatternAction(yytext, SPACE); }  
<SEQUENCE>"BACKSPACE"     { KeyPatternAction(yytext, BACKSPACE); }
<SEQUENCE>"DELETE"        { KeyPatternAction(yytext, DELETE); } 
<SEQUENCE>"TAB"           { KeyPatternAction(yytext, TAB); }
<SEQUENCE>"ESCAPE"        { KeyPatternAction(yytext, ESCAPE); }
<SEQUENCE>"PAGEUP"        { KeyPatternAction(yytext, PAGE_UP); }
<SEQUENCE>"PAGEDOWN"      { KeyPatternAction(yytext, PAGE_DOWN); }
<SEQUENCE>"END"           { KeyPatternAction(yytext, END); }
<SEQUENCE>"HOME"          { KeyPatternAction(yytext, HOME); }

<SEQUENCE>"\""            { BEGIN(STRING); BeginStringPatternAction(); }

<SEQUENCE>"|>"            { EndSequencePatternAction(); BEGIN(INITIAL); }

"+"											  { return OperatorPatternAction(yytext, yyleng); }
"-"											  { return OperatorPatternAction(yytext, yyleng); }
"*"											  { return OperatorPatternAction(yytext, yyleng); }
"/"											  { return OperatorPatternAction(yytext, yyleng); }
"%"											  { return OperatorPatternAction(yytext, yyleng); }
"<"											  { return OperatorPatternAction(yytext, yyleng); }
">"											  { return OperatorPatternAction(yytext, yyleng); }
"<="											{ return OperatorPatternAction(yytext, yyleng); }
">="											{ return OperatorPatternAction(yytext, yyleng); }
"&&"											{ return OperatorPatternAction(yytext, yyleng); }
"||"											{ return OperatorPatternAction(yytext, yyleng); }
"!"											  { return OperatorPatternAction(yytext, yyleng); }
"=="									    { return OperatorPatternAction(yytext, yyleng); }
"==="									    { return OperatorPatternAction(yytext, yyleng); }
"!="									    { return OperatorPatternAction(yytext, yyleng); }
"!=="									    { return OperatorPatternAction(yytext, yyleng); }

"="											  { return AssignmentOperatorPatternAction(); }
"("											  { return OpenParenthesisPatternAction(); }
")"											  { return CloseParenthesisPatternAction(); }
"{"											  { return OpenBracePatternAction(); } 
"}"											  { return CloseBracePatternAction(); } 
"["											  { return OpenBracketPatternAction(); } 
"]"											  { return CloseBracketPatternAction(); } 
","											  { return CommaPatternAction(); }
";"											  { return SemicolonPatternAction(); }
"."											  { return DotPatternAction(); }

"$("                      { return XPathOperatorPatternAction(); }

"Module"                  { return ModulePatternAction(); }
"Suite"                   { return SuitePatternAction(); }
"BeforeAll"               { return BeforeAllPatternAction(); }
"AfterAll"                { return AfterAllPatternAction(); }

"assertTrue"              { return AssertTruePatternAction(); }
"assertFalse"             { return AssertFalsePatternAction(); }
"assertEquals"            { return AssertEqualsPatternAction(); }
"assertNotEquals"         { return AssertNotEqualsPatternAction(); }

"retry"                   { return RetryPatternAction(); }
"try"                     { return TryPatternAction(); }
"catch"                   { return CatchPatternAction(); }
"finally"                 { return FinallyPatternAction(); }

"if"                      { }
"else"                    { }
"while"                   { }
"for"                     { }
"break"                   { }
"continue"                { }

"var"                     { return VarPatternAction(); }
"function"                { return FunctionPatternAction(); }
"return"                  { return ReturnPatternAction(); }

"true"                    { return BooleanPatternAction(yytext, TRUE); }
"false"                   { return BooleanPatternAction(yytext, FALSE); }

{name}                    { return NamePatternAction(yytext, yyleng); }
{number}									{ return IntegerPatternAction(yytext, yyleng); }

{whitespace}+							{ IgnoredPatternAction(yytext, yyleng); }

.											    { return UnknownPatternAction(yytext, yyleng); }

%%
