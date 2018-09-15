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

vector<simbolo> tabelaSimbolos;

//Insere símbolo na tabela de símbolos
void insereSimbolo(string id, string tipo, string temp)
{
	simbolo novoSimbolo;
	novoSimbolo.id = id;
	novoSimbolo.tipo = tipo;
	novoSimbolo.temp = temp;

	tabelaSimbolos.push_back(novoSimbolo);
}

bool existeId(string id)
{

	if (tabelaSimbolos.size() == 0)
			return false;
	

	for(std::vector<simbolo>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++)    
	{

		simbolo temp = *it;

		if (!temp.id.compare(id))
			return true;
    
	}

	return false;
}


bool buscaSimbolo(string id, simbolo &simb)
{

	if (tabelaSimbolos.size() == 0)
			return false;
	

	for(std::vector<simbolo>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++)    
	{

		simbolo temp = *it;

		if (!temp.id.compare(id))
		{
			simb = temp;
			return true;
		}
    
	}

	return false;
}


int yylex(void);
void yyerror(string);
string genTemp();
string genNumId();
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
				cout << tabelaSimbolos.size();
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

COMANDO 	: ID '=' E ';'
			{
				simbolo simb;
				if (!buscaSimbolo($1.label, simb))
				{
					cout << "\tErro: " + $1.label + " não declarado\n";
					exit(1);
				}

				//$1.tipo = simb.tipo;
				//$1.traducao = simb.temp;

				$$.traducao =  $3.traducao + "\t" + simb.temp + " = " + $3.label;
			}
			| TIPO ID ';'
			{

				string idTemp = genTemp();

				if (!existeId($2.label))
					insereSimbolo($2.label, $1.tipo, idTemp);
				else
				{
					cout << "\tErro: Redeclaração do " + $2.label + "\n";	
					exit(1);	
				}

				//$2.traducao = idTemp;
				//$2.tipo = $1.traducao;
			/*	if(verificamapa($2.label))
				{
					erro
				}
				insertmapa($1.tipo, $2.label, genTemp());*/
				$$.traducao = "\t" + $1.tipo + " " + idTemp + ";\n";
			}
			;

TIPO        : TK_TIPO_INT 
			{
				$$.tipo = "int";
				//$$.traducao = "int";
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
				//$$.label = genTemp();
				$$.traducao = $1.traducao + $3.traducao;
				$$.label = $1.label + " + " + $3.label + ";\n";
			}
			| E '-' E
			{
				$$.label = genTemp();
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label + " = " + $1.label + " - " + $3.label + ";\n";
			}
			| TK_NUM
			{
				string id = genNumId();
				string temp = genTemp();

				insereSimbolo(id,"int",temp);

				$$.traducao = "\tint " + temp + " = " + $1.label + ";\n";
				$$.label = temp;
			}
			| TK_ID
			{
				//Busca no mapa
				$$.label = genTemp(); //Atribuir se a temporária existe
				$$.traducao = "\t" + $$.tipo + "" + $$.label + " = " + $1.label + ";\n";
			}
			;
ID		: TK_ID
			{
				//$$.label = genTemp(); 
				//Atribuir se a temporária existe
				$$.label = $1.label;
			}
			;

%%

#include "lex.yy.c"

int yyparse();

int num_temp = 0;
int numGenId = 0;
string genTemp()
{
	return "temp" + to_string(num_temp++);
}
string genNumId()
{
	return to_string(numGenId++);
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
