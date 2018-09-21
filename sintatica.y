%{
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#define YYSTYPE atributos

using namespace std;

struct variavel
{
	string nome;
	string tipo;
	string identificacao;
};

struct atributos
{
	string label;
	string traducao;
	string tipo;
};

vector<variavel> tabelaVariaveis;

//Insere símbolo na tabela de variáveis
void insereVariavel(string nome, string tipo, string identificacao)
{
	variavel novaVariavel;
	novaVariavel.nome = nome;
	novaVariavel.tipo = tipo;
	novaVariavel.identificacao = identificacao;

	tabelaVariaveis.push_back(novaVariavel);
}

//Verifica se existe um nome na tabela de variáveis
bool existeNome(string nome)
{

	if (tabelaVariaveis.size() == 0)
			return false;
	

	for(std::vector<variavel>::iterator it = tabelaVariaveis.begin(); it != tabelaVariaveis.end(); it++)    
	{

		variavel temp = *it;

		if (!temp.nome.compare(nome))
			return true;
    
	}

	return false;
}

//Verifica qual o tipo resultante de uma operação entre dois tipos
string tipoResultanteExpressaoAritimetica(string tipo1, string tipo2)
{

	//Se os tipos são iguais
	if (!tipo1.compare(tipo2))
		return tipo1;

	else if ((!tipo1.compare("real") && !tipo2.compare("int"))
	|| (!tipo1.compare("int") && !tipo2.compare("real")))	//Generalizando
		return "real";

	else
		return "erro";

}


bool buscaVariavel(string nome, variavel &var)
{

	if (tabelaVariaveis.size() == 0)
			return false;
	

	for(std::vector<variavel>::iterator it = tabelaVariaveis.begin(); it != tabelaVariaveis.end(); it++)    
	{

		variavel temp = *it;

		if (!temp.nome.compare(nome))
		{
			var = temp;
			return true;
		}
    
	}

	return false;
}


int yylex(void);
void yyerror(string);
string genTemp();
string genNomeGen();
%}

%token TK_NUM TK_REAL TK_BOOL TK_CHAR
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_REAL TK_TIPO_BOOL TK_TIPO_CHAR
%token TK_FIM TK_ERROR 

%start S


%left '+' '-'
%left '*' '/'

%%

S 			: TK_TIPO_INT TK_MAIN '(' ')' BLOCO
			{
				cout << "int main (void)\n{\n" + $5.traducao + "}\n";
			}
			;

BLOCO		: '{' COMANDOS '}'
			{
				$$.traducao = $2.traducao;

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

			//Atribuição já declarada
COMANDO 	: TK_ID '=' E ';'
			{

				//Variável de nome ID
				variavel var;

				//Busca na tabela se existe variável com o nome de ID
				if (!buscaVariavel($1.label, var))
				{
					//Sinaliza erro
					cout << "\tErro: " + $1.label + " não declarado\n";
					exit(1);
				}

				//Verifica Tipo de ID com o tipo do E
				if ((!var.tipo.compare("int") && $3.tipo.compare("int"))	//Se TK_ID é int e E não é int, acusa erro
				|| !var.tipo.compare("bool") //Se TK_ID é bool, não é permitido operações
				|| !var.tipo.compare("char") )//Se TK_ID é char, não é permitido operações
				{
					cout << "\tErro: Não é possível converter de " + $3.tipo + " " +  "para " + var.tipo + "\n";
					cout << "\t******* \"" + var.nome +  "\" espera um " + var.tipo + "\n";

					exit(1);
				}

				//Transfere para tradução de comando a tradução
				$$.traducao =  $3.traducao + "\t" + var.identificacao + " = " + $3.label + ";\n";
			}

			//Declaração de char
			| TK_TIPO_CHAR ID ';'
			{

				//Criação de variável temporária
				string nomeTemp = genTemp();

				//Verifica se o nome já existe pois seria uma redeclaração
				if (!existeNome($2.label))
					insereVariavel($2.label, "char", nomeTemp);
				else
				{
					cout << "\tErro: Redeclaração do " + $2.label + "\n";	
					exit(1);	
				}

				$$.traducao = "\tchar " + nomeTemp + ";\n";
			}

			//Atribuição Char
			| TK_ID '=' TK_CHAR';'
			{

				//Variável de nome ID
				variavel var;

				//Busca na tabela se existe variável com o nome de ID
				if (!buscaVariavel($1.label, var))
				{
					//Sinaliza erro
					cout << "\tErro: " + $1.label + " não declarado\n";
					exit(1);
				}


				if (var.tipo.compare("char"))
				{
					cout << "\tErro: \"" + $1.label + "\" não é do tipo bool\n";
					exit(1);
				}

				$$.traducao = "\t" + var.identificacao + " = " + $4.label + ";\n";
			}


			//Declaração de char com atribuição
			| TK_TIPO_CHAR ID '=' TK_CHAR ';'
			{

				//Criação de variável temporária
				string nomeTemp = genTemp();

				//Verifica se não existe variável com esse nome para não haver redeclaração
				if (!existeNome($2.label))
					insereVariavel($2.label, "char", nomeTemp);
				else
				{
					cout << "\tErro: Redeclaração do " + $2.label + "\n";	
					exit(1);	
				}

				//Passa para comando a tradução
				$$.traducao = "\tchar " + nomeTemp + " = " + $4.label + ";\n"; 
			}


			//Declaração de Booleana co atribuição
			| TK_TIPO_BOOL ID '=' TK_BOOL ';'
			{

				//Criação de variável temporária
				string nomeTemp = genTemp();

				//Verifica se não existe variável com esse nome para não haver redeclaração
				if (!existeNome($2.label))
					insereVariavel($2.label, "bool", nomeTemp);
				else
				{
					cout << "\tErro: Redeclaração do " + $2.label + "\n";	
					exit(1);	
				}

				//Passa para comando a tradução
				$$.traducao = "\tbool " + nomeTemp + " = " + $4.label + ";\n"; 
			}

			//Declaração de Booleana
			| TK_TIPO_BOOL ID ';'
			{

				//Criação de variável temporária
				string nomeTemp = genTemp();

				//Verifica se o nome já existe pois seria uma redeclaração
				if (!existeNome($2.label))
					insereVariavel($2.label, "bool", nomeTemp);
				else
				{
					cout << "\tErro: Redeclaração do " + $2.label + "\n";	
					exit(1);	
				}

				$$.traducao = "\tbool " + nomeTemp + ";\n";
			}

			//Atribuição Booleana
			| TK_ID '=' TK_BOOL';'
			{

				//Variável de nome ID
				variavel var;

				//Busca na tabela se existe variável com o nome de ID
				if (!buscaVariavel($1.label, var))
				{
					//Sinaliza erro
					cout << "\tErro: " + $1.label + " não declarado\n";
					exit(1);
				}


				if (var.tipo.compare("bool"))
				{
					cout << "\tErro: \"" + $1.label + "\" não é do tipo bool\n";
					exit(1);
				}

				$$.traducao = "\t" + var.identificacao + " = " + $4.label + ";\n";
			}

			//Inicialização com expressão
			| TIPO ID '=' E ';'
			{

				//Verifica Tipo de ID com o tipo do E
				if (!$1.tipo.compare("int") && $4.tipo.compare("int"))	//Se TIPO é int e E não é int, acusa erro
				{
					cout << "\tErro: Não é possível converter de " + $4.tipo + " " +  "para int\n";
					cout << "\t******* \"" + $2.label + "\" espera um " + $1.tipo + "\n";

					exit(1);
				}

						
				//Cria variável temporária
				string idTemp = genTemp();

				//Verifica se já existe o nome para não redeclarar
				if (!existeNome($2.label))
					insereVariavel($2.label, $1.tipo, idTemp);
				else
				{
					cout << "\tErro: Redeclaração do " + $2.label + "\n";	
					exit(1);	
				}

				$$.traducao =  $4.traducao + "\t" + $1.tipo + " " + idTemp + " = " + $4.label + ";\n";
	
			}
			//Declaração
			| TIPO ID ';'
			{

				//Criação de variável temporária
				string nomeTemp = genTemp();

				//Verifica se o nome já existe pois seria uma redeclaração
				if (!existeNome($2.label))
					insereVariavel($2.label, $1.tipo, nomeTemp);
				else
				{
					cout << "\tErro: Redeclaração do " + $2.label + "\n";	
					exit(1);	
				}

				$$.traducao = "\t" + $1.tipo + " " + nomeTemp + ";\n";
			}
			;

TIPO        : TK_TIPO_INT 
			{
				$$.tipo = "int";
			}
			| TK_TIPO_REAL
			{
				$$.tipo = "real";
			}
			;
E 			: E '/' E
			{
				//Criação de variável temporária
				string nometemp = genTemp();

				//Armazena o tipo do resultado da expressão
				string tipo = tipoResultanteExpressaoAritimetica($1.tipo, $3.tipo);

				//Verifica se deu erro de conversão
				if (!tipo.compare("erro"))
				{
					cout << "\tErro: " + $1.tipo + " e " + $3.tipo + " não são compatíveis!" + "\n";	
					exit(1);
				}
				
				//Adiciona na tabela
				insereVariavel(genNomeGen(), tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = tipo;


				//Passa para E a tradução
				$$.traducao = $1.traducao + $3.traducao + 
				"\t" + tipo + " " + nometemp + " = " + $1.label + " / " + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;
			}
			| E '*' E
			{
				//Criação de variável temporária
				string nometemp = genTemp();

				//Armazena o tipo do resultado da expressão
				string tipo = tipoResultanteExpressaoAritimetica($1.tipo, $3.tipo);

				//Verifica se deu erro de conversão
				if (!tipo.compare("erro"))
				{
					cout << "\tErro: " + $1.tipo + " e " + $3.tipo + " não são compatíveis!" + "\n";	
					exit(1);
				}
				
				//Adiciona na tabela
				insereVariavel(genNomeGen(), tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = tipo;


				//Passa para E a tradução
				$$.traducao = $1.traducao + $3.traducao + 
				"\t" + tipo + " " + nometemp + " = " + $1.label + " * " + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;

			}
			| E '+' E
			{
				//Criação de variável temporária
				string nometemp = genTemp();

				//Armazena o tipo do resultado da expressão
				string tipo = tipoResultanteExpressaoAritimetica($1.tipo, $3.tipo);

				//Verifica se deu erro de conversão
				if (!tipo.compare("erro"))
				{
					cout << "\tErro: " + $1.tipo + " e " + $3.tipo + " não são compatíveis!" + "\n";	
					exit(1);
				}
				
				//Adiciona na tabela
				insereVariavel(genNomeGen(), tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = tipo;


				//Passa para E a tradução
				$$.traducao = $1.traducao + $3.traducao + 
				"\t" + tipo + " " + nometemp + " = " + $1.label + " + " + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;
			}
			| E '-' E
			{
				//Criação de variável temporária
				string nometemp = genTemp();

				//Armazena o tipo do resultado da expressão
				string tipo = tipoResultanteExpressaoAritimetica($1.tipo, $3.tipo);

				//Verifica se deu erro de conversão
				if (!tipo.compare("erro"))
				{
					cout << "\tErro: " + $1.tipo + " e " + $3.tipo + " não são compatíveis!" + "\n";	
					exit(1);
				}
				
				//Adiciona na tabela
				insereVariavel(genNomeGen(), tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = tipo;


				//Passa para E a tradução
				$$.traducao = $1.traducao + $3.traducao + 
				"\t" + tipo + " " + nometemp + " = " + $1.label + " - " + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;

			}
			| TK_NUM
			{
				//Passa para E o tipo e seu valor
				$$.tipo = "int";
				$$.label = $1.label;
			}
			| TK_REAL
			{
				//Passa para E o tipo e seu valor
				$$.tipo = "real";
				$$.label = $1.label;
			}
			//Uso de IDs do lado direito da expressão
			| TK_ID
			{
				//Busca na tabela
				variavel var;
				//se retorna falso, não foi declarada a variável TK_ID
				if (!buscaVariavel($1.label, var))
				{
					cout << "\tErro: " + $1.label + " não declarado\n";
					exit(1);
				}

				//Passa o tipo e o nome para E
				$$.tipo = var.tipo;
				$$.label = var.identificacao; 
				}
			;
ID		: TK_ID
			{
				//Passa seu nome literal para ID
				$$.label = $1.label;
			}
			;

%%

#include "lex.yy.c"

int yyparse();

int tempGenQtt = 0;
int nomeGenQtt = 0;

string genTemp()
{
	return "temp" + to_string(tempGenQtt++);
}
string genNomeGen()
{
	return to_string(nomeGenQtt++);
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
