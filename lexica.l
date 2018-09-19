%{
#include <string>

%}

ESPACO [ \t]
DIGITO [0-9]
LETRA [A-Za-z_]
BOOLEANA (true|false)

ATRIB {=}
DELIM {ESPACO}+
NUM {DIGITO}+
BOOL {BOOLEANA}
REAL {NUM}+("."{NUM}+)?
CHAR \'({LETRA}|{NUM})\'
ID {LETRA}({LETRA}|{NUM})*

%%

"\n"		{}

{DELIM} 	{}

"main"		{return TK_MAIN;}

"int"		{return TK_TIPO_INT;}

"bool"		{return TK_TIPO_BOOL;}

"real"		{return TK_TIPO_REAL;}

[(){}[\];,=]	{ return *yytext; }

[+\-*/]		{ return *yytext; }

{NUM}		{ yylval.label = yytext; return TK_NUM; }
{REAL}		{ yylval.label = yytext; return TK_REAL; }
{ID}		{ yylval.label = yytext; return TK_ID; }
{BOOL}		{ yylval.label = yytext; return TK_BOOL; }

.			{ *yytext; }

%%
