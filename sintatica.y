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
#include <regex>

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

struct _switch
{
	string var;
	string comp;
	string res;
	string tipo;
	bool hasDefault;
	int defaultQtt;
};

struct _function
{
	vector <variavel> functionContext;
	stack <_function> fctx;
	string label;
};

struct _procedure
{
	vector <variavel> procedureContext;
	stack <_function> pctx;
	string label;
};


vector < vector < variavel> > pilhaContextoVariavel;
vector <variavel> funcContext;
vector <variavel> procContext;
vector < laco > pilhaLaco;
vector <_function> functions;
vector <_procedure> procedures;
stack <string> labelStackEnd;

stack <_switch> switchVar;

string declaracoes;

int tempGenQtt = 0;
int nomeGenQtt = 0;
int lacoQtt = 0;
int caseQtt = 0;
int funcQtt = 0;

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

	vector <variavel> localsVar;
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

			if (!temp.nome.compare(nome))
			{
				var = temp;
				return;
			}
    
		}
	}

	if(!funcContext.empty())
	{
		for(std::vector<variavel>::iterator i = funcContext.begin(); i != funcContext.end(); i++)
		{
			variavel temp = *i;
			if(!temp.nome.compare(nome))
			{
				var = temp;
				return;
			}
		}
	}

	if(!procContext.empty())
	{
		for(std::vector<variavel>::iterator i = procContext.begin(); i != procContext.end(); i++)
		{
			variavel temp = *i;
			if(!temp.nome.compare(nome))
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
	&& !E2.tipo.compare("REAL"))
	{
		//Criação de variável temporária
		string nomeTemp = genTemp();

		//Tenta inserir variável
		insereVariavel(genNomeGen(), "REAL", nomeTemp);

		E1.tipo = "REAL";
		E1.traducao = E1.traducao + "\t" + nomeTemp + " = (REAL) " + E1.label + ";\n"; 
		E1.label = nomeTemp;
	}
	else if (!E1.tipo.compare("REAL")
	&& !E2.tipo.compare("int"))
	{
		//Criação de variável temporária
		string nomeTemp = genTemp();

		//Tenta inserir variável
		insereVariavel(genNomeGen(), "REAL", nomeTemp);

		E2.tipo = "REAL";
		E2.traducao = E2.traducao + "\t" + nomeTemp + " = (REAL) " + E2.label + ";\n"; 
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
	else if (!tipo1.compare("REAL"))
	{
		if(!tipo2.compare("int")
			|| !tipo2.compare("REAL"))
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

%token TK_IS
%token TK_NUM TK_REAL TK_BOOL TK_CHAR TK_WHILE TK_FOR TK_DO TK_BREAK TK_CONTINUE TK_BREAKALL
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_REAL TK_TIPO_BOOL TK_TIPO_CHAR TK_STRING TK_TIPO_STRING
%token TK_FIM TK_ERROR TK_INPUT TK_OUTPUT TK_SWITCH TK_CASE TK_DEFAULT
%token TK_IF TK_ELSE
%token TK_FUNCTION TK_RETURN TK_PROCEDURE 

%start S


%left '+' '-'
%left '*' '/'

%%

S 			: CMDSGLOBAL
			{
				desempilhaContexto();

				cout << "#define TRUE 1\n#define FALSE 0\n#define BOOL int\n#define REAL float\n\n" 
				+ declaracoes + "\n" +  $1.traducao + "\n";

				//Regex para dar free em char*
				//Toda variável temp_s tem um char* com mesmo numero de temp
				regex rgx("int temp\\d*\\_s");

				string prefixo = "int ";
				string posfixo = "_s";
				
				//Percorre o Regex e acha uma temporária temp_s
				for(sregex_iterator it(declaracoes.begin(), declaracoes.end(), rgx), it_end; it != it_end; ++it )
        		{
        			//Recebe a temporária
        			string temp = (*it)[0];
					
					//Remove o prefixo
					std::string::size_type i = temp.find(prefixo);
					if (i != std::string::npos)
   						temp.erase(i, prefixo.length());

   					//Remove o posfixo
   					i = temp.find(posfixo);
					if (i != std::string::npos)
   						temp.erase(i, posfixo.length());

   					cout << "\tfree(" + temp + ");\n";
        		}	

        		cout << "}\n";

        		//cout << (*it)[0] << "\n";
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

EMPSWITCH	:
			{

				string inicioLabel = "inicioSwitch" + to_string(lacoQtt);
				string fimLabel = "fimSwitch" + to_string(lacoQtt);

				//Insere na pilha
				laco newSwitch;
				newSwitch.labelinicio = inicioLabel;
				newSwitch.labelfim = fimLabel;
				pilhaLaco.push_back(newSwitch);

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
					"\tif(" + nometemp + ")\n\t" + "goto " + lacoAtual.labelfim + ";\n" +
					$6.traducao +
					"\tgoto " + lacoAtual.labelinicio + ";\n" + 
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
				$$.traducao = "\tgoto " + lacoAtual.labelfim + ";\n";

			}
			| TK_BREAKALL ';'
			{
				//Verifica se há um contexto de laço em questão
				if (pilhaLaco.size() == 0)
				{
					cout << "\tbreak fora de um laço!\n";
					exit(3);
				}

				laco lacoAtual = pilhaLaco.front();
				$$.traducao = "\tgoto " + lacoAtual.labelfim + ";\n";

			}
			| TK_CONTINUE ';'
			{
				//Verifica se há um contexto de laço em questão
				if (pilhaLaco.size() == 0)
				{
					cout << "\tcontinue fora de um laço!\n";
					exit(3);
				}

				laco lacoAtual;

				//Enquanto o contexto atual for de case percorre todos os contextos em busca de um laço
				for(std::vector< laco >::reverse_iterator it = pilhaLaco.rbegin(); it != pilhaLaco.rend(); it++)
				{
					//Pega as labels na pilha do Laço atual
					lacoAtual = *it;

					if (!(lacoAtual.labelinicio.find("case") != std::string::npos))
						break;
				}

				//Se ele parou em um contexto de case então acusa erro
				if (lacoAtual.labelinicio.find("case") != std::string::npos)
				{

					cout << "\tcontinue fora de um laço!\n";
					exit(3);
				}


				//Realiza o desvio do laço em questão
				$$.traducao = "\tgo to " + lacoAtual.labelinicio + ";\n";

			}
			;

MAIN 		: TK_TIPO_INT TK_MAIN '(' ')' BLOCO
			{
				$$.traducao = "\nint main (void)\n{\n" + $5.traducao;
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
			| '=' ID '[' E ']'
			{

				//Variavel ID
				variavel var;
				
				buscaVariavel($2.label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				$$.traducao = "";
				$$.tipo = var.tipo;
				$$.label = "*(" + var.identificacao + " + " + $4.label + ")";

			}
			| '=' ID '[' E ']' '[' E ']'
			{
				//Variavel ID
				variavel var;
				
				buscaVariavel($2.label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				$$.traducao = "";
				$$.tipo = var.tipo;
				$$.label = "*(" + var.identificacao + " + " + $4.label + " * " + var.identificacao + "_d + " + $7.label + ")";

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
			| ID '[' E ']' OPATRIB ';'
			{
				
				if ($3.tipo.compare("int"))
				{
					cout << "\tErro: O índice de um array deve ser inteiro!\n";
					exit(1);
				}

				//Variavel ID
				variavel var;
				
				buscaVariavel($1.label, var);
				
				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				//Compara atribuição 
				verificaAtribuicao(var.tipo, $5.tipo);
				
				//Verifica se teve atribuição
				if ($5.tipo.compare("null"))
				{
					if (var.tipo.compare($5.tipo))
						$$.traducao = $3.traducao 
						+ $5.traducao + "\t" + "(" + var.identificacao + " + " + $3.label + ")* = (" + var.tipo + ") " + $5.label + ";\n";
					else
						$$.traducao = $3.traducao 
						+ $5.traducao + "\t" + "(" + var.identificacao + " + " + $3.label + ")* = " + $5.label + ";\n";
				}
			}
			| ID '[' E ']' '[' E ']' OPATRIB ';'
			{
				
				if ($3.tipo.compare("int") && $6.tipo.compare("int"))
				{
					cout << "\tErro: O índice de um array deve ser inteiro!\n";
					exit(1);
				}

				//Variavel ID
				variavel var;
				
				buscaVariavel($1.label, var);
				
				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				//Compara atribuição 
				verificaAtribuicao(var.tipo, $8.tipo);
				
				//Verifica se teve atribuição
				if ($8.tipo.compare("null"))
				{
					if (var.tipo.compare($8.tipo))
						$$.traducao = $3.traducao + $6.traducao + $8.traducao +
						"\t" + "(" + var.identificacao + " + " + $3.label + " * " + var.identificacao + "_d + " + $6.label + ")* = (" + var.tipo + ") " + $8.label + ";\n";
					else
						$$.traducao = $3.traducao + $6.traducao + $8.traducao +
						"\t" + "(" + var.identificacao + " + " + $3.label + " * " + var.identificacao + "_d + " + $6.label + ")* = " + $8.label + ";\n";
				}
			}
			| ID OPUNARIO
			{
				variavel var;
				
				buscaVariavel($1.label, var);

				if((!var.tipo.compare("int") || !var.tipo.compare("real"))){
					$$.traducao = "\t" + var.identificacao + " = " + var.identificacao + " " + $2.traducao + " " +"1;\n";
				}
				else{
					cout << "Esse tipo de variavel não pode ter um operador unário.\n";	
					exit(1);
				}

			}
			| ID OPCOMPOSTO E ';'
			{
				variavel var;
				
				buscaVariavel($1.label, var);

				
					$$.traducao =   $3.traducao + "\t" + var.identificacao + " = " + var.identificacao + " " + $2.traducao  + $3.label + ";\n";
				
			}
			;
OPUNARIO : '+''+'';'
			{
				$$.traducao = '+';
			}
			| '-''-'';'
			{
				$$.traducao = '-';
			}
OPCOMPOSTO 		: '+''='
			{
				$$.traducao = '+';
			}
			| '-''='
			{
				$$.traducao = '-';
			}
			| '*''='
			{
				$$.traducao = '*';
			}
			| '/''='
			{
				$$.traducao = '/';
			};
			
//Declarações
DECLARACAO	: TIPO ID OPATRIB ';'
			{

				//Criação de variável temporária
				string nomeTemp = genTemp();
				
				
				//Tenta inserir variável
				insereVariavel($2.label, $1.tipo , nomeTemp);

				if (!$1.tipo.compare("char*"))
					insereVariavel(genNomeGen(), "int" , nomeTemp + "_s");

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
			| TIPO ID '[' E ']' ';'
			{

				if ($4.tipo.compare("int"))
				{
					cout << "\tErro: O tamanho de um array é inteiro!\n";
					exit(1);
				}

				//Criação de variável temporária
				string nomeTemp = genTemp();
				string nomeTemp1 = nomeTemp;

				nomeTemp = nomeTemp + "_s";

				string tipoBase = $1.tipo;
				$1.tipo = $1.tipo + "*";
	
				
				//Tenta inserir variável
				insereVariavel($2.label, $1.tipo , nomeTemp1);
				insereVariavel(genNomeGen(), "int" , nomeTemp);

				//Verifica se teve atribuição
				$$.traducao = $4.traducao + "\t" + nomeTemp + " = " + $4.label + ";\n"
				+ "\t" + nomeTemp1 + " = (" + $1.tipo + ") malloc (" + nomeTemp + " * sizeof(" + tipoBase + "));\n";
				

			}
			| TIPO ID '[' E ']' '[' E ']' ';'
			{

				if ($4.tipo.compare("int") && $7.tipo.compare("int"))
				{
					cout << "\tErro: O tamanho de um array é inteiro!\n";
					exit(1);
				}

				//Criação de variável temporária
				string nomeTemp = genTemp();
				string nomeTemp1 = nomeTemp;
				string nomeTemp2 = nomeTemp;

				nomeTemp1 = nomeTemp1 + "_s";
				nomeTemp2 = nomeTemp2 + "_d";

				string tipoBase = $1.tipo;
				$1.tipo = $1.tipo + "*";
	
				
				//Tenta inserir variável
				insereVariavel($2.label, $1.tipo , nomeTemp);
				insereVariavel(genNomeGen(), "int" , nomeTemp1);
				insereVariavel(genNomeGen(), "int" , nomeTemp2);

				//Declara
				$$.traducao = $4.traducao + $7.traducao + 
				"\t" + nomeTemp1 + " = " + $4.label + ";\n" +
				"\t" + nomeTemp2 + " = " + $7.label + ";\n" +
				"\t" + nomeTemp + " = (" + $1.tipo + ") malloc (" + nomeTemp1 + " * " + nomeTemp2 + " * sizeof(" + tipoBase + "));\n";
				

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
			| FUNCTION
			{
				$$.traducao = $1.traducao;
			}
			| CALL_FUNC
			{
				$$.traducao = $1.traducao;
			}
			| RETURN 
			{
				$$.traducao = $1.traducao;
			}
			| PROCEDURE
			{
				$$.traducao = $1.traducao;
			}
			| CALL_PROC
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
				$$.tipo = "REAL";
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


				//Verifica a conversão int para bool
				if (!$1.tipo.compare("BOOL") && !$3.tipo.compare("int"))
				{

					//Verifica se veio apenas um número
					if (!($3.label.find("temp") != std::string::npos)) 
					{
    					//Criação de variável temporária
						string nometemp1_b = genTemp();
						//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
						//Adiciona na tabela
						insereVariavel(genNomeGen(), "int", nometemp1_b);

						$3.traducao = $3.traducao +
						"\t" + nometemp1_b + " = " + $3.label + ";\n";

						$3.label = nometemp1_b;
					}


					//Criação de variável temporária
					string nometemp1 = genTemp();
					//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
					//Adiciona na tabela
					insereVariavel(genNomeGen(), "BOOL", nometemp1);

					//Transforma de inteiro para bool
					$3.traducao = $3.traducao 
					+ "\t" + nometemp1 + " = " + $3.label + " != 0;\n"
					+ "\tif(" + nometemp1 + ")" 
					+ "\n\t" + $3.label + " = TRUE;\n";

				}
				else if (!$3.tipo.compare("BOOL") && !$1.tipo.compare("int"))
				{
					//Verifica se veio apenas um número
					if (!($1.label.find("temp") != std::string::npos)) 
					{
    					//Criação de variável temporária
						string nometemp1_b = genTemp();
						//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
						//Adiciona na tabela
						insereVariavel(genNomeGen(), "int", nometemp1_b);

						$1.traducao = $1.traducao +
						"\t" + nometemp1_b + " = " + $1.label + ";\n";

						$1.label = nometemp1_b;
					}


					//Criação de variável temporária
					string nometemp1 = genTemp();
					//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
					//Adiciona na tabela
					insereVariavel(genNomeGen(), "BOOL", nometemp1);

					//Transforma de inteiro para bool
					$1.traducao = $1.traducao 
					+ "\t" + nometemp1 + " = " + $1.label + " != 0;\n"
					+ "\tif(" + nometemp1 + ")" 
					+ "\n\t" + $1.label + " = TRUE;\n";
				}

				//Passa para EL a tradução
				$$.traducao = $1.traducao + $3.traducao
				+ "\t" + nometemp + " = " + $1.label + $2.traducao + $3.label + ";\n";

				//Passa para E seu valor de temporária
				$$.label = nometemp;
			}
			| OPNDOLOGIC
			{

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);

				//Guarda o tipo da Expressão resultante em E
				$$.tipo = "BOOL";
				//Passa para E seu valor de temporária
				$$.label = nometemp;

				$$.traducao = "\t" + nometemp + " = " + $1.label + " != 0;\n";
			}
			;

OPNDOLOGIC	: E
			{
				//Criação de variável temporária
				//string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				//insereVariavel(genNomeGen(), "BOOL", nometemp);



				$$.traducao = $1.traducao ;
				//+ "\t" + nometemp + " = " + $1.label + " != 0;\n"
				//+ "\tif(" + nometemp + ")" 
				//+ "\n\t\t" + $1.label + " = TRUE;\n";
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
			| EL
			{
				$$.traducao = $1.traducao;
				$$.tipo = $1.tipo;
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

				if (!$1.tipo.compare("char*") && !$3.tipo.compare("char*"))
				{
					cout << "\tNão é possível operação de divisão com strings!\n";
					exit(1);
				}

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

					if (!$1.tipo.compare("char*") && !$3.tipo.compare("char*"))
				{
					cout << "\tNão é possível operação de subtração com strings!\n";
					exit(1);
				}

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

				if (!$1.tipo.compare("char*") && !$3.tipo.compare("char*"))
				{

					//Insere nova variável de tamanho
					insereVariavel(genNomeGen(), "int", nometemp + "_s");

					//Passa para E a tradução
					$$.traducao = $1.traducao + $3.traducao 
					+ "\t" + nometemp + "_s = " + $1.label + "_s + " + $3.label + "_s;\n"
					+ "\t" + nometemp + "_s = " + nometemp + "_s - 1;\n" 
					+ "\t" + nometemp + " = (" + $1.tipo + ") malloc (" + nometemp + "_s * sizeof(" + $1.tipo.substr(0, $1.tipo.size() -1) + ");\n" 
					+ "\tstrcpy(" + nometemp + ", " + $1.label + ");\n"
					+ "\tstrcat(" + nometemp + ", " + $3.label + ");\n";

				}
				else
				{
					//Passa para E a tradução
					$$.traducao = $1.traducao + $3.traducao 
					+ "\t" + nometemp + " = " + $1.label + " + " + $3.label + ";\n";
				}
				
				//Passa para E seu valor de temporária
				$$.label = nometemp;

			}
			| TK_STRING
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
			| E '-' E
			{

				if (!$1.tipo.compare("char*") && !$3.tipo.compare("char*"))
				{
					cout << "\tNão é possível operação de subtração com strings!\n";
					exit(1);
				}

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
				$$.tipo = "REAL";
				$$.label = $1.label;
			}
			| ID '[' E ']'
			{

				//Variavel ID
				variavel var;
				
				buscaVariavel($1.label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				if (var.tipo.compare("int") && var.tipo.compare("REAL"))
				{
					cout << "\tNão é possível operações aritméticas com " + var.tipo + "!\n";
					exit(1);
				}

				$$.traducao = $3.traducao;
				$$.tipo = var.tipo;
				$$.label = "*(" + var.identificacao + " + " + $3.label + ")";
			}
			| ID '[' E ']' '[' E ']'
			{

				//Variavel ID
				variavel var;
				
				buscaVariavel($1.label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				if (var.tipo.compare("int") && var.tipo.compare("REAL"))
				{
					cout << "\tNão é possível operações aritméticas com " + var.tipo + "!\n";
					exit(1);
				}

				$$.traducao = $3.traducao + $6.traducao;
				$$.tipo = var.tipo;
				$$.label = "*(" + var.identificacao + " + " + $3.label + " * " + var.identificacao + "_d" + " + " + $6.label + ")";
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

ID			: TK_ID
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

				$$.traducao = $3.traducao + "\t" + nometemp + " = !" + 
				$3.label + ";\n" + "\tif" + "(" + nometemp + ")" + "\n\tgoto " + label + ";\n" + 
				$5.traducao + "\tgoto " + $6.tipo + ";\n" + $6.traducao +"\t"+ $6.tipo + ":\n";
			}
			| TK_SWITCH '(' ID ')' EMPSWITCH '{' CASES '}' 
			{	
				variavel var;
				_switch swt = switchVar.top();
				switchVar.pop();
				laco swAtual = pilhaLaco.back();
								
				buscaVariavel($3.label, var);

				if(var.tipo.compare(swt.tipo))
				{
					cout << "switch and case have different types\n";
					exit(1);
				}

				$$.traducao = "\t" + swAtual.labelinicio + ":\n\t" + swt.comp + " = " + 
				var.identificacao + ";\n" + $7.traducao + "\t" + swAtual.labelfim + ":\n";

				 //Desempilha switch
				pilhaLaco.pop_back();
			}
			;
CASE_COMP 	: TK_NUM
			{
				//Passa para E o tipo e seu valor
				$$.tipo = "int";
				$$.label = $1.label;
			}
			| TK_CHAR
			{
				$$.tipo = "char";
				$$.label = $1.label;
			}
			| TK_STRING
			{
				$$.tipo = "string";
				$$.label = $1.label;
			}
			;
CASES 		: CASE CASES
			{
				$$.traducao = $1.traducao + $2.traducao;
			}
			| DEFAULT 
			{
				$$.traducao = $1.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;

CASE 		: TK_CASE CASE_COMP ':' COMANDOS
			{
				_switch swt;
				string labelAt = gerarLabel(0);
				if(switchVar.empty())
				{
					swt.var = genTemp();
					swt.comp = genTemp();
					swt.res = genTemp();
					swt.tipo = $2.tipo;
					switchVar.push(swt);
					insereVariavel(genNomeGen(), $2.tipo, swt.var);
					insereVariavel(genNomeGen(), $2.tipo, swt.res);
					insereVariavel(genNomeGen(), $2.tipo, swt.comp);
					
				}

				swt = switchVar.top();
				
				if(swt.tipo.compare($2.tipo))
				{
					cout << "Error in type switch case " + $2.tipo + " not equal " + swt.tipo + "\n";
					exit(1);
				}
				
				$$.traducao = "\t" + swt.var + " = " + $2.label + ";\n\t" + 
				swt.res + " = " + "!(" + swt.comp + " == " + swt.var + ");" + "\n\t"
				"if(" + swt.res + ")" + "\n\t" + "goto " + labelAt + ";"+ "\n" + 
				$4.traducao + "\t" + labelAt + ":" + "\n" ;
			}
			;
DEFAULT     : TK_DEFAULT ':' COMANDOS
			{
				$$.traducao = $3.traducao;
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
FUNCTION    : TK_FUNCTION ID '(' PAR ARGS ')' TK_RETURN TIPO TK_IS BLOCO
			{
				_function func;
				
				for(std::vector< vector <variavel> >::reverse_iterator it = pilhaContextoVariavel.rbegin(); it != pilhaContextoVariavel.rend(); it++)    
				{
					vector <variavel> tabelaVariaveis = *it;

					if(tabelaVariaveis.size() == 0)
					{
						continue;
					}
					for(std::vector<variavel>::iterator it = tabelaVariaveis.begin(); it != tabelaVariaveis.end(); it++)    
					{
						//Aponta pra uma variável
						variavel temp = *it;
						cout << temp.nome;
						if(!temp.nome.compare($2.label))
						{
							cout << "variavel com este nome";
							exit(0);
						}
					}
				}

				func.functionContext = funcContext;
				func.label =  $2.label;
				funcContext.erase(funcContext.begin(), funcContext.end());
				functions.push_back(func);
				funcQtt++;
				$$.traducao = $8.tipo + " " + $2.label  + 
				"(" + $4.traducao + $5.traducao + ")\n{\n" + $10.traducao + "\n}\n";
			}
			;
PROCEDURE   : TK_PROCEDURE ID '(' PARGS ')' TK_IS BLOCO
			{
				_procedure proc;
				proc.procedureContext = procContext;
				proc.label = $2.label;
				procContext.erase(procContext.begin(), procContext.end());
				procedures.push_back(proc);

				$$.traducao = "void " + $2.label + "(" + $4.traducao + ")" +
				"\n{\n" + $7.traducao + "\n}\n";
			}
			;
RETURN      : TK_RETURN IT ';'
			{
				$$.traducao = "\treturn " + $2.traducao + ";\n";
			}
			;
IT          : E
			{
				$$.traducao = $1.label;
			}
			| TK_STRING 
			{
				$$.traducao = $1.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;
ARGS		: VIRG PAR ARGS
			{
				$$.traducao = $1.traducao + $2.traducao + $3.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;

PARGS       : PPAR PARGS
			{
				$$.traducao = $1.traducao + $2.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;
PARGS       : VIRG PPAR PARGS
			{
				$$.traducao = $1.traducao + $2.traducao + $3.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;
VIRG        : ','
			{
				$$.traducao = ", ";
			}
			;
PAR         : TIPO ID
			{
				variavel var;

				var.identificacao = genTemp();
				var.tipo = $1.tipo;
				var.nome = $2.label;//genNomeGen();
				funcContext.push_back(var);

				$$.traducao = $1.tipo + " " + var.identificacao;
			}
			;

PPAR        : TIPO ID
			{
				variavel var;

				var.identificacao = genTemp();
				var.tipo = $1.tipo;
				var.nome = $2.label;//genNomeGen();
				procContext.push_back(var);

				$$.traducao = $1.tipo + " " + var.identificacao;
			}
			;


CALL_FUNC   : ID '=' ID '(' ONE_PAR MORE_PARS ')' ';'
			{
				variavel var;

				buscaVariavel($1.label, var);

				for(vector<_function>::iterator f = functions.begin(); f != functions.end(); f++)
				{
					_function temp = *f;
					if(!temp.label.compare($3.label))
					{
						break;
					}
				}

				$$.traducao = "\t" + var.identificacao + " = " + $3.label + "(" +
				$5.traducao + $6.traducao + ")" + ";\n";
			}
			;
CALL_PROC   : ID '('PARS ')' ';'
			{
				$$.traducao = "\t" + $1.label + "(" + $3.traducao + ")"+ ";\n";
			}
			;
MORE_PARS   : VIRG ONE_PAR MORE_PARS
			{
				$$.traducao = $1.traducao + $2.traducao + $3.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;
ONE_PAR     : ID
			{
				variavel var;

				buscaVariavel($1.label, var);

				$$.traducao = var.identificacao;
				$$.label = $1.label;
			}
			| TK_NUM
			{
				variavel var;

				var.identificacao = genTemp() ;
				var.tipo = "const int";
				var.nome = $1.label;
				cout << $1.tipo;
				insereVariavel($1.label, "const int", var.identificacao + " = " + $1.label);
				$$.traducao = var.identificacao;
			}
			| TK_CHAR
			{
				variavel var;

				var.identificacao = genTemp() ;
				var.tipo = "const char";
				var.nome = $1.label;
				cout << $1.tipo;
				insereVariavel($1.label, "const char", var.identificacao + " = " + $1.label);
				$$.traducao = var.identificacao;
			}
			| TK_REAL
			{
				variavel var;

				var.identificacao = genTemp() ;
				var.tipo = "const float";
				var.nome = $1.label;
				cout << $1.tipo;
				insereVariavel($1.label, "const float", var.identificacao + " = " + $1.label);
				$$.traducao = var.identificacao;
			}
			| TK_BOOL
			{
				variavel var;

				var.identificacao = genTemp() ;
				var.tipo = "const BOOL";
				var.nome = $1.label;
				cout << $1.tipo;
				insereVariavel($1.label, "const BOOL", var.identificacao + " = " + $1.label);
				$$.traducao = var.identificacao;
			}

			;

PARS        : ONE_PAR P_MORE_ARGS
			{
				$$.traducao = $1.traducao + $2.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;
P_MORE_ARGS : VIRG ONE_PAR P_MORE_ARGS
			{
				$$.traducao = $1.traducao + $2.traducao + $3.traducao;
			}
			|
			{
				$$.traducao = "";
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
