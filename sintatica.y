%{
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <stack>
#include <queue> 
#include <ctype.h>

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

struct laco
{
	string labelinicio;
	string labelfim;
};

vector < vector < variavel> > pilhaContextoVariavel;
vector < laco > pilhaLaco;
stack <string> labelStackEnd;

string declaracoes;

int tempGenQtt = 0;
int nomeGenQtt = 0;
int lacoQtt = 0;

//Verifica se já existe uma variável com esse nome
bool existeNome(string nome);

//Empilha um contexto
void empilhaContexto()
{
	vector <variavel> tabelaVariaveis;

	pilhaContextoVariavel.push_back(tabelaVariaveis);

}

string gerarLabelEndif(void)
{
	char buffer[64];
	static unsigned i;

	sprintf(buffer,"endif%i", i++);

	return buffer;
}

string gerarLabel(int base)
{
	char buffer[64];
	static unsigned i;

	sprintf(buffer,"L%i", base+i++);

	return buffer;
}


void desempilhaContexto()
{
	//Guarda as variáveis declaradas
	vector <variavel> tabelaVariaveis = pilhaContextoVariavel.back();

	//Percorre dentro de um contexto do mais recente ao mais antigo
	for(std::vector<variavel>::reverse_iterator it = tabelaVariaveis.rbegin(); it != tabelaVariaveis.rend(); it++)    
	{

		//Aponta pra uma variável
		variavel temp = *it;

		declaracoes = "\t" + temp.tipo + " " + temp.identificacao + ";\n" + declaracoes ;
    
	}

	pilhaContextoVariavel.pop_back();
}

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


	
	//Adiciona variável no último contexto
	pilhaContextoVariavel.back().push_back(novaVariavel);


}

//Verifica se existe um nome na tabela de variáveis
bool existeNome(string nome)
{

	vector <variavel> tabelaVariaveis = pilhaContextoVariavel.back();

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

	//Percorre os contextos do fim ao início
	for(std::vector< vector <variavel> >::reverse_iterator it = pilhaContextoVariavel.rbegin(); it != pilhaContextoVariavel.rend(); it++)    
	{

		//Aponta para um contexto
		vector <variavel> tabelaVariaveis = *it;

		//Se não tem variável declarada
		if (tabelaVariaveis.size() == 0)
			continue;
	
		//Percorre dentro de um contexto
		for(std::vector<variavel>::iterator it = tabelaVariaveis.begin(); it != tabelaVariaveis.end(); it++)    
		{

			//Aponta pra uma variável
			variavel temp = *it;

			//Se achou o nome
			if (!temp.nome.compare(nome))
			{
				var = temp;
				return;
			}
    
		}

	}

	//Sinaliza erro
	cout << "\tErro: " + nome + " não declarado\n";
	exit(1);	
	
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
		if(!tipo2.compare("int")
			||!tipo2.compare("BOOL"))
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
	else if (!tipo1.compare("BOOL"))
	{
		if(!tipo2.compare("BOOL")
			|| !tipo2.compare("int"))
			return;		
	}
	else if (!tipo1.compare("char*"))
	{
		if(!tipo2.compare("char*"))
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

%token TK_NUM TK_REAL TK_BOOL TK_CHAR TK_WHILE TK_FOR TK_DO TK_BREAK TK_CONTINUE
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_REAL TK_TIPO_BOOL TK_TIPO_CHAR TK_STRING TK_TIPO_STRING
%token TK_FIM TK_ERROR TK_INPUT TK_OUTPUT TK_SWITCH TK_CASE
%token TK_IF TK_ELSE

%start S


%left '+' '-'
%left '*' '/'

%%

S 			: CMDSGLOBAL
			{
				desempilhaContexto();

				cout << "#define TRUE 1\n#define FALSE 0\n#define BOOL int\n\n" 
				+ declaracoes + "\n" +  $1.traducao;
				
			}
			;

//Regra para emiplhar um contexto específico de laço
EMPLACO 	:
			{

				string inicioLabel = "inicioLaco" + to_string(lacoQtt);
				string fimLabel = "fimLaco" + to_string(lacoQtt);

				//Insere na pilha
				laco novoLaco;
				novoLaco.labelinicio = inicioLabel;
				novoLaco.labelfim = fimLabel;
				pilhaLaco.push_back(novoLaco);

				lacoQtt++;

			}
			;

LACO		: TK_WHILE '(' EL ')' EMPLACO BLOCO
			{
				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				//Criação de variável temporária
				string nometemp = genTemp();

				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);
				
				$$.traducao = "\n\t" + lacoAtual.labelinicio + ":\n" + 
					$3.traducao +
					"\t" + nometemp + " = !" + $3.label + ";\n" +
					"\tif " + nometemp + " go to " + lacoAtual.labelfim + ":\n" +
					$6.traducao +
					"\tgo to " + lacoAtual.labelinicio + ":\n" + 
					"\t" + lacoAtual.labelfim + ":\n\n";

				//Desempilha laço
				pilhaLaco.pop_back();

			}
			| TK_FOR '('E '=' E ';' EL ';' E OPATRIB ')' EMPLACO BLOCO
			{

				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				//Criação de variável temporária
				string nometemp = genTemp();

				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);		

				$$.traducao =   "\t" + $3.label + " = " + $5.label + "\n\t" + lacoAtual.labelinicio +  "\n" +  $7.traducao + "\t" + nometemp + " != " + $7.label + "\n" + "\tif(" + nometemp + 
				") goto" + lacoAtual.labelfim + "\n" + $13.traducao  + $10.traducao  + "\t" + $3.label + " = " + $10.label + "\n\tgoto" +  lacoAtual.labelinicio +  "\n" + "\t" + lacoAtual.labelfim + ":" "\n";

				//Desempilha laço
				pilhaLaco.pop_back();
			}
			| TK_DO EMPLACO BLOCO TK_WHILE '(' EL ')'';'{

				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				$$.traducao = "\t" + lacoAtual.labelinicio + "\n"+ $3.traducao + "\tif(" + $6.label + ") goto " + lacoAtual.labelinicio + "\n\t" + lacoAtual.labelfim + "\n";

				//Desempilha laço
				pilhaLaco.pop_back();

			}
			;

INTLACO 	: TK_BREAK ';'
			{
				//Verifica se há um contexto de laço em questão
				if (pilhaLaco.size() == 0)
				{
					cout << "\tbreak fora de um laço!\n";
					exit(3);
				}

				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				//Realiza o desvio do laço em questão
				$$.traducao = "\tgo to " + lacoAtual.labelfim + ";\n";

			}
			| TK_CONTINUE ';'
			{
				//Verifica se há um contexto de laço em questão
				if (pilhaLaco.size() == 0)
				{
					cout << "\tcontinue fora de um laço!\n";
					exit(3);
				}

				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				//Realiza o desvio do laço em questão
				$$.traducao = "\tgo to " + lacoAtual.labelinicio + ";\n";

			}
			;

MAIN 		: TK_TIPO_INT TK_MAIN '(' ')' BLOCO
			{
				$$.traducao = "\nint main (void)\n{\n" + $5.traducao + "}\n";
			}
			;

BLOCO		: EMPCONTEXTO '{' COMANDOS '}' DESCONTEXTO
			{
				$$.traducao = $3.traducao;

			}
			;

DESCONTEXTO :	
			{

				desempilhaContexto();
			}
			;

EMPCONTEXTO : 
			{

				
				empilhaContexto();
			}	
			;
CMDSGLOBAL	: COMANDO CMDSGLOBAL
			{
				$$.traducao = $1.traducao + $2.traducao;
			}
			| MAIN
			{
				$$.traducao = $1.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;

COMANDOS	: COMANDO COMANDOS
			{
				$$.traducao = $1.traducao + $2.traducao;
				
			}
			| BLOCO
			{
				$$.traducao = $1.traducao;
			}
			|
			{
				$$.traducao = "";
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

				//$$.label = $2.label;
			if ($2.label.compare("true"))
				$$.label = "FALSE";
			else
				$$.label = "TRUE";

				$$.tipo = "BOOL";
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
			| '=' ES
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

//Expressões com Strings
ES     		: TK_STRING
			{

				//Cria e insere variáveis
				string nomeTemp1 = genTemp();
				insereVariavel(genNomeGen(), "char*" , nomeTemp1);

				string nomeTemp2 = nomeTemp1 + "_s";
				insereVariavel(genNomeGen(), "int" , nomeTemp2);

				int size = $1.label.length() - 1;

				$$.traducao = "\t" + nomeTemp2 + " = " + to_string(size) + ";\n"
				+ "\t" + nomeTemp1 + " = (char*) malloc (sizeof(char) * " + nomeTemp2 + ");\n"
				+ "\t" + "strcpy(" + nomeTemp1 + ", " + $1.label + ");\n";

				$$.tipo = "char*";

				$$.label = nomeTemp1;
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
				if ($2.tipo.compare("null"))
				{
					if (var.tipo.compare($2.tipo))
						$$.traducao = $2.traducao + "\t" + var.identificacao + " = (" + var.tipo + ") " + $2.label + ";\n";
					else
						$$.traducao = $2.traducao + "\t" + var.identificacao + " = " + $2.label + ";\n";
				}
			}

			
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
				{
					if ($1.tipo.compare($3.tipo))
						$$.traducao = $3.traducao + "\t" + nomeTemp + " = (" + $1.tipo + ") " + $3.label + ";\n";
					else
						$$.traducao = $3.traducao + "\t" + nomeTemp + " = " + $3.label + ";\n";
				}
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
			| CONDICIONAL
			{
				$$.traducao = $1.traducao;
			}
			| LACO
			{
				$$.traducao = $1.traducao;
			}
			| INTLACO
			{
				$$.traducao = $1.traducao;
			}
			| INPUT
			{
				$$.traducao = $1.traducao;
			}
			| OUTPUT
			{
				$$.traducao = $1.traducao;
			}
			;

INPUT 		: TK_INPUT '(' ID ')' ';'
			{
				//Busca na tabela
				variavel var;

				//Tenta buscar a variável
				buscaVariavel($3.label, var);

				$$.traducao = "\tcin >> " + var.identificacao + ";\n";
			}
			;

OUTPUT		: TK_OUTPUT '(' OUTTERM ')' ';'
			{
				$$.traducao = $3.traducao 
				+ "\tcout >> " + $3.label + ";\n";	
			}

OUTTERM		:  TK_STRING
			{
				//Cria e insere variáveis
				string nomeTemp1 = genTemp();
				insereVariavel(genNomeGen(), "char*" , nomeTemp1);

				string nomeTemp2 = nomeTemp1 + "_s";
				insereVariavel(genNomeGen(), "int" , nomeTemp2);

				int size = $1.label.length() - 1;

				$$.traducao = "\t" + nomeTemp2 + " = " + to_string(size) + ";\n"
				+ "\t" + nomeTemp1 + " = (char*) malloc (sizeof(char) * " + nomeTemp2 + ");\n"
				+ "\t" + "strcpy(" + nomeTemp1 + ", " + $1.label + ");\n";

				$$.tipo = "char*";

				$$.label = nomeTemp1;


				//$$.traducao =  $1.label;
			}
			| ID
			{
				//Busca na tabela
				variavel var;

				//Tenta buscar a variável
				buscaVariavel($1.label, var);

				$$.label = var.identificacao;
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
				$$.tipo = "BOOL";
			}
			| TK_TIPO_CHAR
			{
				$$.tipo = "char";
			}
			|  TK_TIPO_STRING
			{
				$$.tipo = "char*";

			}
			;

//Expresões Lógicas
EL 			: OPNDOLOGIC OPLOGIC OPNDOLOGIC
			{

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = "BOOL";

				//Passa para EL a tradução
				$$.traducao = $1.traducao + $3.traducao
				+ "\t" + nometemp + " = " + $1.label + $2.traducao + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;
			}
			| OPNDOLOGIC
			{
				$$.traducao = "";
			}
			;

OPNDOLOGIC	: E
			{
				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);



				$$.traducao = $1.traducao 
				+ "\t" + nometemp + " = " + $1.label + " != 0;\n"
				+ "\tif(" + nometemp + ")" 
				+ "\n\t\t" + $1.label + " = TRUE;\n";
				$$.tipo = $1.tipo;
				$$.label = $1.label;
			}
			| TK_CHAR
			{
				$$.tipo = "char";
				$$.label = $1.label;
			}
			| TK_BOOL
			{

			if ($1.label.compare("true"))
				$$.label = "FALSE";
			else
				$$.label = "TRUE";

				$$.tipo = "BOOL";
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
			| '(' TIPO ')' TK_ID
			{
				//Busca na tabela
				variavel var;
				
				//Tenta buscar a variável
				buscaVariavel($4.label, var);

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), $2.tipo, nometemp);

				$$.traducao = "\t" + nometemp + " = (" + $2.tipo + ") " + var.identificacao + "\n";

				//Passa o tipo e o nome para E
				$$.tipo = $2.tipo;
				$$.label = nometemp; 
			}
			;
ID		: TK_ID
			{
				//Passa seu nome literal para ID

				$$.label = $1.label;
			}
			;
CONDICIONAL : TK_IF '(' EL ')' BLOCO CONDMODIF
			{
				
				string nometemp = genTemp();
				insereVariavel(genNomeGen(), "BOOL", nometemp);
				string label = labelStackEnd.top();
				labelStackEnd.pop();

				$$.traducao = $3.traducao + "\t" + nometemp + "= !" + 
				$3.label + ";\n" + "\tif" + "(" + nometemp + ")" + "\n\tgoto " + label + ";\n" + 
				$5.traducao + "\tgoto " + $6.tipo + ";\n" + $6.traducao +"\t"+ $6.tipo + ":\n";
			}
			| TK_SWITCH '(' TRMSWITCH ')' '{' CASES '}'
			{
				$$.traducao = "";
			}
			;

TRMSWITCH	: ID
			{
				//Busca na tabela
				variavel var;
				
				//Tenta buscar a variável
				buscaVariavel($1.label, var);

				//Verifica se é do tipo inteiro
				if (var.tipo.compare("int"))
				{
					cout << "Switch espera um inteiro!";
					exit(1);
				}

				$$.label = var.identificacao; 
			}
			|	TK_NUM
			{
				$$.label = $1.label;
			}
			;

CASES		: CASE CASES
			{
				$$.traducao = "";
			}
			|
			{
				$$.traducao = "";
			}
			;

CASE 		: TK_CASE ':' COMANDOS 
			{
				$$.traducao = "";
			}
			;
			
CONDMODIF   :TK_ELSE TK_IF '(' EL ')' BLOCO CONDMODIF
			{
				
				string nometemp = genTemp();
				string labelInit = gerarLabel(0);
				string labelEnd = labelStackEnd.top();
				labelStackEnd.pop();		

				insereVariavel(genNomeGen(), "BOOL", nometemp);

				$$.traducao = "\t" + labelInit + ":\n"+ $4.traducao + "\t" + nometemp + "= !" + 
				$4.label + ";\n\tif" + "(" + nometemp + ")" + "\n\tgoto " + labelEnd + ";\n" + 
				$6.traducao + "\tgoto " + $7.tipo + ";\n" + $7.traducao ;
				$$.tipo = $7.tipo;

				labelEnd = gerarLabel(-1);
				labelStackEnd.push(labelEnd);
			}
			| TK_ELSE BLOCO

			{
				
				string label = gerarLabelEndif();
				string labelelse = gerarLabel(0);
				labelStackEnd.push(label);
				labelStackEnd.push(labelelse);
				$$.tipo = label;
				$$.traducao = "\t" + labelelse + ":\n" + $2.traducao;

			}
			|
			{
				string label = gerarLabelEndif();
				labelStackEnd.push(label);
				$$.tipo = label;
			}
			;

%%

#include "lex.yy.c"

int yyparse();

int main( int argc, char* argv[] )
{

	empilhaContexto();

	yyparse();

	return 0;
}

void yyerror( string MSG )
{
	cout << MSG << endl;
	exit (0);
}				
