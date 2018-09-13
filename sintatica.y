%{
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#define YYSTYPE atributos

using namespace std;

//Criar o vector de struct simbolos

//funcoes: exite id? inseresimbolo e buscasimbolo

struct simbolo
{
	string id;
	string tipo;
	string temp;
};

struct atributos
{
	string label;
	string traducao;
	string tipo;
};


int yylex(void);
void yyerror(string);
string genTemp();
%}

%token TK_NUM
%token TK_MAIN TK_ID TK_TIPO_INT 
%token TK_FIM TK_ERROR

%start S


%left '+' '-'
%left '*' '/'

%%

S 			: TK_TIPO_INT TK_MAIN '(' ')' BLOCO
			{
				//string declaracao = "\tint temp1;";
				//for (int = 0)
				cout << "int main (void)\n{\n" + $5.traducao + "}\n";
			}
			;

BLOCO		: '{' COMANDOS '}'
			{
				$$.traducao = $2.traducao;

				//cout << $$.traducao, $2.traducao;
			}
			;

COMANDOS	: COMANDO COMANDOS
			{
				$$.traducao = $1.traducao + $2.traducao;
			}
			|
			{
				
			}
			;

COMANDO 	: TK_ID '=' E ';'
			{

				$$.traducao = $1.traducao + $3.traducao;
			}
			| TIPO TK_ID ';'
			{
				//$2.tipo = $1.traducao;
				if(verificamapa($2.label))
				{
					erro
				}
				insertmapa($1.tipo, $2.label, genTemp());
				$$.traducao = "";
			}
			;

TIPO        : TK_TIPO_INT 
			{
				$$.tipo = "int";
			}
			;
E 			: E '/' E
			{
				$$.label = genTemp();
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label + " = " + $1.label + " / " + $3.label + ";\n";
			}
			|E '*' E
			{
				$$.label = genTemp();
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label + " = " + $1.label + " * " + $3.label + ";\n";
			}
			| E '+' E
			{
				$$.label = genTemp();
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label + " = " + $1.label + " + " + $3.label + ";\n";
			}
			| E '-' E
			{
				$$.label = genTemp();
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label + " = " + $1.label + " - " + $3.label + ";\n";
			}
			| TK_NUM
			{
				$$.label = genTemp();
				$$.traducao = "\t" + $$.tipo + "" + $$.label + " = " + $1.label + ";\n";
			}
			| TK_ID
			{
				//Busca no mapa
				$$.label = genTemp(); //Atribuir se a temporária existe
				$$.traducao = "\t" + $$.tipo + "" + $$.label + " = " + $1.label + ";\n";
			}
			;

%%

#include "lex.yy.c"

int yyparse();

int num_temp = 0;

string genTemp()
{
	return "temp" + to_string(num_temp++);
}

int main( int argc, char* argv[] )
{
	yyparse();

	return 0;
}

void yyerror( string MSG )
{
	cout << MSG << endl;
	exit (0);
}				
