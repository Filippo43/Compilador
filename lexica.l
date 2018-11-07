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
ID {LETRA}({LETRA}|{NUM})*
STRING {STR}



%%

"\n"		{}

{DELIM} 	{}

"main"		{return TK_MAIN;}

"int"		{return TK_TIPO_INT;}

"for"		{return TK_FOR;}

"bool"		{return TK_TIPO_BOOL;}

"while"		{return TK_WHILE;}

"if" 		{ return TK_IF; }

"input"		{ return TK_INPUT;}

"output"	{ return TK_OUTPUT;}

"else" 		{return TK_ELSE;}

"char"		{return TK_TIPO_CHAR;}

"real"		{return TK_TIPO_REAL;}

[(){}[\];,=<>!\"]	{ return *yytext; }

[+\-*/]		{ return *yytext; }


{NUM}		{ yylval.label = yytext; return TK_NUM; }
{CHAR}		{ yylval.label = yytext; return TK_CHAR; }
{REAL}		{ yylval.label = yytext; return TK_REAL; }
{BOOL}		{ yylval.label = yytext; return TK_BOOL; }
{ID}		{ yylval.label = yytext; return TK_ID; }
{STRING}	{ yylval.label = yytext; return TK_STRING; }


.			{ return *yytext;}

%%
