%{
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <stack>

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

stack <variavel> pilhaContextoVariavel;
vector<variavel> tabelaVariaveis;
int tempGenQtt = 0;
int nomeGenQtt = 0;

bool existeNome(string nome);

string genTemp()
{
	return "temp" + to_string(tempGenQtt++);
}
string genNomeGen()
{
	return to_string(nomeGenQtt++);
}

//Insere símbolo na tabela de variáveis
void insereVariavel(string nome, string tipo, string identificacao)
{

	//Se o nome existe na tabela
	if (existeNome(nome))
	{
		cout << "\tErro: Redeclaração do " + nome + "\n";	
		exit(1);	
	}

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

//Busca por uma variável declarada
void buscaVariavel(string nome, variavel &var)
{

	if (tabelaVariaveis.size() == 0)
	{
		//Sinaliza erro
		cout << "\tErro: " + nome + " não declarado\n";
		exit(1);	
	}
	

	for(std::vector<variavel>::iterator it = tabelaVariaveis.begin(); it != tabelaVariaveis.end(); it++)    
	{

		variavel temp = *it;

		if (!temp.nome.compare(nome))
		{
			var = temp;
			return;
		}
    
	}

		
	//Sinaliza erro
	cout << "\tErro: " + nome + " não declarado\n";
	exit(1);	
	
}

//Lista as declarações das variáveis
void listaDeclaracoes()
{
	for(std::vector<variavel>::iterator it = tabelaVariaveis.begin(); it != tabelaVariaveis.end(); it++)    
	{

		variavel temp = *it;

		cout << "\t" + temp.tipo + " " + temp.identificacao + ";\n"; 
    
	}


}

//Atualiza os valores de uma expressão aritmética
void atualizaRegraExprAritimetica(atributos &E1, atributos &E2)
{
	//INT x FLOAT -> (float)
	if (!E1.tipo.compare("int")
	&& !E2.tipo.compare("real"))
	{
		//Criação de variável temporária
		string nomeTemp = genTemp();

		//Tenta inserir variável
		insereVariavel(genNomeGen(), "real", nomeTemp);

		E1.tipo = "real";
		E1.traducao = E1.traducao + "\t" + nomeTemp + " = (Real) " + E1.label + ";\n"; 
		E1.label = nomeTemp;
	}
	else if (!E1.tipo.compare("real")
	&& !E2.tipo.compare("int"))
	{
		//Criação de variável temporária
		string nomeTemp = genTemp();

		//Tenta inserir variável
		insereVariavel(genNomeGen(), "real", nomeTemp);

		E2.tipo = "real";
		E2.traducao = E2.traducao + "\t" + nomeTemp + " = (Real) " + E2.label + ";\n"; 
		E2.label = nomeTemp;
	}
	//Se os tipos são diferentes e desconhecidos
	else if (E1.tipo.compare(E2.tipo))
	{
		cout << "\tErro: Não é possível conversão entre " + E1.tipo + " e " + E2.tipo + ";\n";
		exit(1);
	}
}

//Verifica se pode uma atribuição
void verificaAtribuicao (string tipo1, string tipo2)
{
	if (!tipo1.compare("null") || !tipo2.compare("null"))
		return;

	if (!tipo1.compare("int"))
	{
		if(!tipo2.compare("int"))
			return;
	}
	else if (!tipo1.compare("real"))
	{
		if(!tipo2.compare("int")
			|| !tipo2.compare("real"))
			return;
		
	}
	else if (!tipo1.compare("char"))
	{
		if(!tipo2.compare("char"))
			return;
	}
	else if (!tipo1.compare("bool"))
	{
		if(!tipo2.compare("bool"))
			return;		
	}

	cout << "\tErro: não pôde converter de " + tipo2 + " para " + tipo1 + "\n";
	exit(1);
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
				cout << "int main (void)\n{\n";

				listaDeclaracoes();

				cout << "\n" + $5.traducao + "}\n";
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
//Atribuições do lado direito
OPATRIB		: '=' TK_CHAR
			{
				$$.label = $2.label;
				$$.tipo = "char";
			}
			| '=' TK_BOOL
			{
				$$.label = $2.label;
				$$.tipo = "bool";
			}
			| '=' E 
			{
				$$.label = $2.label;
				$$.traducao = $2.traducao;
				$$.tipo = $2.tipo;
			}
			| '=' EL
			{
				$$.label = $2.label;
				$$.traducao = $2.traducao;
				$$.tipo = $2.tipo;
			}
			|
			{
				$$.tipo = "null";
			}
			;
//Atribuições
ATRIBUICAO 	: ID OPATRIB ';'
			{

				//Variavel ID
				variavel var;

				buscaVariavel($1.label, var);

				//Compara atribuição 
				verificaAtribuicao(var.tipo, $2.tipo);
				
				//Verifica se teve atribuição
				if ($3.tipo.compare("null"))
					$$.traducao = $2.traducao + "\t" + var.identificacao + " = " + $2.label + ";\n";
			
			}
			;
//Declarações
DECLARACAO	: TIPO ID OPATRIB ';'
			{

				//Criação de variável temporária
				string nomeTemp = genTemp();

				//Tenta inserir variável
				insereVariavel($2.label, $1.tipo , nomeTemp);

				//Verifica se a atribuição pode ocorrer de acordo com os tipos
				verificaAtribuicao($1.tipo, $3.tipo);

				//Verifica se teve atribuição
				if ($3.tipo.compare("null"))
					$$.traducao =  $3.traducao + "\t" + nomeTemp + " = " + $3.label + ";\n";
			}
			;

			//Atribuição já declarada
COMANDO 	: DECLARACAO
			{
				//Transfere para tradução de comando a tradução de DECLARACAO
				$$.traducao =  $1.traducao;
			}
			| ATRIBUICAO
			{
				//Transfere para tradução de comando a tradução de ATRIBUICAO
				$$.traducao =  $1.traducao;
			}
			;

TIPO 	    : TK_TIPO_INT 
			{
				$$.tipo = "int";
			}
			| TK_TIPO_REAL
			{
				$$.tipo = "real";
			}
			| TK_TIPO_BOOL
			{
				$$.tipo = "bool";
			}
			| TK_TIPO_CHAR
			{
				$$.tipo = "char";
			}
			;
//Expresões Lógicas
EL 			: OPNDOLOGIC OPLOGIC OPNDOLOGIC
			{
				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), "bool", nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = "bool";

				//Passa para EL a tradução
				$$.traducao = $1.traducao + $3.traducao 

				+ "\t" + nometemp + " = " + $1.label + $2.traducao + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;
			}
			;

OPNDOLOGIC	: E
			{
				$$.traducao = $1.traducao;
				$$.tipo = $1.tipo;
				$$.label = $1.label;
			}
			| TK_CHAR
			{
				$$.tipo = "char";
				$$.label = $1.label;
			}
			;
//Operadores Lógicos
OPLOGIC		: '=' '='
			{
				$$.traducao = " == ";
			}
			| '!' '='
			{
				$$.traducao = " != ";
			}
			| '<' '='
			{
				$$.traducao = " <= ";
			}
			| '>' '='
			{
				$$.traducao = " >= ";
			}
			| '>'
			{
				$$.traducao = " > ";
			}
			| '<'
			{
				$$.traducao = " < ";
			}
			;

E 			: E '/' E
			{
				//Verifica se a expressão é válida
				atualizaRegraExprAritimetica($1, $3);

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), $1.tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = $1.tipo;

				//Passa para E a tradução
				$$.traducao = $1.traducao + $3.traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t" + nometemp + " = " + $1.label + " / " + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;
			}
			| E '*' E
			{
				//Verifica se a expressão é válida
				atualizaRegraExprAritimetica($1, $3);

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), $1.tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = $1.tipo;

				//Passa para E a tradução
				$$.traducao = $1.traducao + $3.traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t" + nometemp + " = " + $1.label + " * " + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;

			}
			| E '+' E
			{

				//Verifica se a expressão é válida
				atualizaRegraExprAritimetica($1, $3);

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), $1.tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = $1.tipo;

				//Passa para E a tradução
				$$.traducao = $1.traducao + $3.traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t" + nometemp + " = " + $1.label + " + " + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;

			}
			| E '-' E
			{
				//Verifica se a expressão é válida
				atualizaRegraExprAritimetica($1, $3);

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), $1.tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = $1.tipo;

				//Passa para E a tradução
				$$.traducao = $1.traducao + $3.traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t"  + nometemp + " = " + $1.label + " - " + $3.label + ";\n";

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
				
				//Tenta buscar a variável
				buscaVariavel($1.label, var);

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
