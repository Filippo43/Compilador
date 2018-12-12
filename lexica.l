%{
#include <string>

%}

ESPACO [ \t]
DIGITO [0-9]
LETRA [A-Za-z_]
BOOLEANA (true|false)
STR \"(\\.|[^"\\])*\"

ATRIB {=}
DELIM {ESPACO}+
NUM {DIGITO}+
BOOL {BOOLEANA}
REAL {NUM}+("."{NUM}+)?
CHAR \'({LETRA}|{NUM})\'
STRING {STR}
ID {LETRA}({LETRA}|{NUM})*
CASE (case){NUM}




%%

"\n"		{}

{DELIM} 	{}

"break"		{return TK_BREAK;}

"continue"	{return TK_CONTINUE;}
"function"	{return TK_FUNCTION;}
"procedure" {return TK_PROCEDURE;}
"return"	{return TK_RETURN;}

"case"		{return TK_CASE;}
"default"	{return TK_DEFAULT;}
"main"		{return TK_MAIN;}
"is"		{return TK_IS;}

"int"		{return TK_TIPO_INT;}

"for"		{return TK_FOR;}

"bool"		{return TK_TIPO_BOOL;}

"input" 	{return TK_INPUT;}

"output"	{return TK_OUTPUT;}

"do"		{return TK_DO;}

"while"		{return TK_WHILE;}

"if" 		{return TK_IF; }

"else" 		{return TK_ELSE;}

"switch"	{return TK_SWITCH;}

"breakall" {return TK_BREAKALL;}

"string"	{return TK_TIPO_STRING;}

"char"		{return TK_TIPO_CHAR;}

"real"		{return TK_TIPO_REAL;}

[(){}[\];,=<>!:]	{ return *yytext; }

[+\-*/]		{ return *yytext; }


{NUM}		{ yylval.label = yytext; return TK_NUM; }
{CHAR}		{ yylval.label = yytext; return TK_CHAR; }
{REAL}		{ yylval.label = yytext; return TK_REAL; }
{BOOL}		{ yylval.label = yytext; return TK_BOOL; }
{ID}		{ yylval.label = yytext; return TK_ID; }
{STRING}	{ yylval.label = yytext; return TK_STRING;}


.			{ *yytext; }

%%
