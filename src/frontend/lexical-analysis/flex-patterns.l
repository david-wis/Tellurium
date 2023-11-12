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

/* Patrones reutilizables. */
string          \"([^\\\"]|\\.)*\"  
digit			      [0-9]
endline			    [(\r)?\n]
whitespace		  [ \f\n\r\t\v]
char            [a-zA-Z]
name            {char}({char}|{digit})*
number           ({digit}*[.])?{digit}+
single_line_comment  "//".*

%%

"/*"							{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.						{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>{endline}+				{ IgnoredPatternAction(yytext, yyleng); }
<COMMENT>"*/"					{ EndCommentPatternAction(); BEGIN(INITIAL); }

{single_line_comment}    { IgnoredPatternAction(yytext, yyleng); }  

"<|"            { return BeginSequencePatternAction(); }

"CONTROL"       { return KeyPatternAction(yytext, KEY_CONTROL); }
"SHIFT"         { return KeyPatternAction(yytext, KEY_SHIFT); }
"ALT"           { return KeyPatternAction(yytext, KEY_ALT); }

"LEFT"          { return KeyPatternAction(yytext, KEY_LEFT); }
"RIGHT"         { return KeyPatternAction(yytext, KEY_RIGHT); }
"UP"            { return KeyPatternAction(yytext, KEY_UP); }   
"DOWN"          { return KeyPatternAction(yytext, KEY_DOWN); }

"ENTER"         { return KeyPatternAction(yytext, KEY_ENTER); }
"SPACE"         { return KeyPatternAction(yytext, KEY_SPACE); }  
"BACKSPACE"     { return KeyPatternAction(yytext, KEY_BACKSPACE); }
"DELETE"        { return KeyPatternAction(yytext, KEY_DELETE); } 
"TAB"           { return KeyPatternAction(yytext, KEY_TAB); }
"ESCAPE"        { return KeyPatternAction(yytext, KEY_ESCAPE); }
"PAGEUP"        { return KeyPatternAction(yytext, KEY_PAGE_UP); }
"PAGEDOWN"      { return KeyPatternAction(yytext, KEY_PAGE_DOWN); }
"END"           { return KeyPatternAction(yytext, KEY_END); }
"HOME"          { return KeyPatternAction(yytext, KEY_HOME); }

"|>"            { return EndSequencePatternAction(); }


")"{whitespace}*"=>"                    { return ArrowPatternAction(); }

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

"+"											  { return PlusPatternAction(); }
"-"											  { return MinusPatternAction(); }
"*"											  { return MultiplicationPatternAction(); }
"/"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
"%"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
"<="											{ return BinaryOperatorPatternAction(yytext, yyleng); }
">="											{ return BinaryOperatorPatternAction(yytext, yyleng); }
"<"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
">"											  { return BinaryOperatorPatternAction(yytext, yyleng); }
"&&"											{ return BinaryOperatorPatternAction(yytext, yyleng); }
"||"											{ return BinaryOperatorPatternAction(yytext, yyleng); }
"==="									    { return BinaryOperatorPatternAction(yytext, yyleng); }
"=="									    { return BinaryOperatorPatternAction(yytext, yyleng); }
"!=="									    { return BinaryOperatorPatternAction(yytext, yyleng); }
"!="									    { return BinaryOperatorPatternAction(yytext, yyleng); }
"!"											  { return UnaryOperatorPatternAction(yytext, yyleng); }
"|"											{ return PipePatternAction(); }


"$("                      { return XPathOperatorPatternAction(ONE); }
"#("                      { return XPathOperatorPatternAction(MANY); }

"Module"                  { return ModulePatternAction(); }
"Suite"                   { return SuitePatternAction(); }
"BeforeAll"               { return BeforeAllPatternAction(); }
"AfterAll"                { return AfterAllPatternAction(); }

"assertTrue"              { return AssertPatternAction(TRUTHY); }
"assertFalse"             { return AssertPatternAction(FALSY); }
"assertEquals"            { return AssertComparePatternAction(EQUAL); }
"assertNotEquals"         { return AssertComparePatternAction(NOT_EQUAL); }

"retry"                   { return RetryPatternAction(); }
"try"                     { return TryPatternAction(); }
"catch"                   { return CatchPatternAction(); }
"finally"                 { return FinallyPatternAction(); }

"if"                      { return IfPatternAction(); }
"else"                    { return ElsePatternAction(); }
"while"                   { return WhilePatternAction(); }
"for"                     { return ForPatternAction(); }

"var"                     { return VarPatternAction(VAR); }
"let"                     { return VarPatternAction(LET); }
"const"                   { return VarPatternAction(CONST); }
"function"                { return FunctionPatternAction(); }
"async"                   { return AsyncPatternAction(); }
"await"                   { return AwaitPatternAction(); }
"return"                  { return ReturnPatternAction(); }

"true"                    { return BooleanPatternAction(yytext, true); }
"false"                   { return BooleanPatternAction(yytext, false); }

"null"                   { return NullPatternAction(); }
"undefined"              { return UndefinedPatternAction(); }
"nan"                    { return NaNPatternAction(); }

{name}                      { return NamePatternAction(yytext, yyleng); }
{digit}+				    { return IntegerPatternAction(yytext, yyleng); }
{number}				    { return NumberPatternAction(yytext, yyleng); }
{string}                    { return StringPatternAction(yytext, yyleng); }

{whitespace}+				{ IgnoredPatternAction(yytext, yyleng); }

.							{ return UnknownPatternAction(yytext, yyleng); }

%%
