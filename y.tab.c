/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "sintatica.y" /* yacc.c:339  */

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

#line 404 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_IS = 258,
    TK_NUM = 259,
    TK_REAL = 260,
    TK_BOOL = 261,
    TK_CHAR = 262,
    TK_WHILE = 263,
    TK_FOR = 264,
    TK_DO = 265,
    TK_BREAK = 266,
    TK_CONTINUE = 267,
    TK_BREAKALL = 268,
    TK_MAIN = 269,
    TK_ID = 270,
    TK_TIPO_INT = 271,
    TK_TIPO_REAL = 272,
    TK_TIPO_BOOL = 273,
    TK_TIPO_CHAR = 274,
    TK_STRING = 275,
    TK_TIPO_STRING = 276,
    TK_FIM = 277,
    TK_ERROR = 278,
    TK_INPUT = 279,
    TK_OUTPUT = 280,
    TK_SWITCH = 281,
    TK_CASE = 282,
    TK_DEFAULT = 283,
    TK_IF = 284,
    TK_ELSE = 285,
    TK_FUNCTION = 286,
    TK_RETURN = 287,
    TK_PROCEDURE = 288
  };
#endif
/* Tokens.  */
#define TK_IS 258
#define TK_NUM 259
#define TK_REAL 260
#define TK_BOOL 261
#define TK_CHAR 262
#define TK_WHILE 263
#define TK_FOR 264
#define TK_DO 265
#define TK_BREAK 266
#define TK_CONTINUE 267
#define TK_BREAKALL 268
#define TK_MAIN 269
#define TK_ID 270
#define TK_TIPO_INT 271
#define TK_TIPO_REAL 272
#define TK_TIPO_BOOL 273
#define TK_TIPO_CHAR 274
#define TK_STRING 275
#define TK_TIPO_STRING 276
#define TK_FIM 277
#define TK_ERROR 278
#define TK_INPUT 279
#define TK_OUTPUT 280
#define TK_SWITCH 281
#define TK_CASE 282
#define TK_DEFAULT 283
#define TK_IF 284
#define TK_ELSE 285
#define TK_FUNCTION 286
#define TK_RETURN 287
#define TK_PROCEDURE 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 521 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  59
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   378

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  129
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  270

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   288

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,     2,     2,     2,     2,     2,
      38,    39,    36,    34,    50,    35,     2,    37,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    49,    41,
      47,    40,    48,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    44,     2,    45,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,    43,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   354,   354,   395,   412,   427,   450,   468,   481,   497,
     510,   546,   552,   560,   567,   573,   577,   582,   587,   592,
     597,   604,   610,   621,   629,   636,   642,   657,   672,   679,
     702,   722,   752,   782,   797,   808,   812,   816,   820,   824,
     828,   834,   859,   888,   925,   930,   935,   939,   943,   947,
     951,   955,   959,   963,   967,   971,   977,   989,   995,  1017,
    1029,  1034,  1038,  1042,  1046,  1054,  1139,  1158,  1176,  1181,
    1191,  1199,  1203,  1207,  1211,  1215,  1219,  1225,  1255,  1285,
    1328,  1348,  1379,  1385,  1391,  1411,  1432,  1447,  1470,  1477,
    1489,  1511,  1517,  1522,  1528,  1532,  1537,  1542,  1573,  1578,
    1596,  1608,  1614,  1648,  1660,  1665,  1669,  1674,  1678,  1683,
    1688,  1693,  1697,  1702,  1706,  1711,  1724,  1738,  1757,  1762,
    1767,  1771,  1780,  1791,  1802,  1813,  1827,  1832,  1836,  1841
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_IS", "TK_NUM", "TK_REAL", "TK_BOOL",
  "TK_CHAR", "TK_WHILE", "TK_FOR", "TK_DO", "TK_BREAK", "TK_CONTINUE",
  "TK_BREAKALL", "TK_MAIN", "TK_ID", "TK_TIPO_INT", "TK_TIPO_REAL",
  "TK_TIPO_BOOL", "TK_TIPO_CHAR", "TK_STRING", "TK_TIPO_STRING", "TK_FIM",
  "TK_ERROR", "TK_INPUT", "TK_OUTPUT", "TK_SWITCH", "TK_CASE",
  "TK_DEFAULT", "TK_IF", "TK_ELSE", "TK_FUNCTION", "TK_RETURN",
  "TK_PROCEDURE", "'+'", "'-'", "'*'", "'/'", "'('", "')'", "'='", "';'",
  "'{'", "'}'", "'['", "']'", "'!'", "'<'", "'>'", "':'", "','", "$accept",
  "S", "EMPLACO", "EMPSWITCH", "LACO", "INTLACO", "MAIN", "BLOCO",
  "DESCONTEXTO", "EMPCONTEXTO", "CMDSGLOBAL", "COMANDOS", "OPATRIB", "ES",
  "ATRIBUICAO", "OPUNARIO", "OPCOMPOSTO", "DECLARACAO", "COMANDO", "INPUT",
  "OUTPUT", "OUTTERM", "TIPO", "EL", "OPNDOLOGIC", "OPLOGIC", "E", "ID",
  "CONDICIONAL", "CASE_COMP", "CASES", "CASE", "DEFAULT", "CONDMODIF",
  "FUNCTION", "PROCEDURE", "RETURN", "IT", "ARGS", "PARGS", "VIRG", "PAR",
  "PPAR", "CALL_FUNC", "CALL_PROC", "MORE_PARS", "ONE_PAR", "PARS",
  "P_MORE_ARGS", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,    43,    45,    42,    47,    40,    41,
      61,    59,   123,   125,    91,    93,    33,    60,    62,    58,
      44
};
# endif

#define YYPACT_NINF -161

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-161)))

#define YYTABLE_NINF -89

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     297,   -28,   -20,  -161,    -3,    22,    28,  -161,    62,  -161,
    -161,  -161,  -161,    52,    59,    64,    72,    35,    36,    35,
     117,  -161,  -161,  -161,  -161,  -161,  -161,   297,  -161,  -161,
      35,   200,  -161,  -161,  -161,  -161,  -161,  -161,    17,   115,
    -161,  -161,  -161,  -161,    88,    35,    16,    35,    17,    91,
    -161,  -161,   -18,  -161,   133,   336,   114,    95,   106,  -161,
    -161,    43,    -6,     4,   123,   135,   108,    55,   115,   137,
    -161,   115,  -161,  -161,  -161,   140,    25,   336,   322,   183,
     151,   158,   168,  -161,   170,  -161,   172,   174,   133,  -161,
     182,   115,   115,   115,   115,   115,  -161,    90,    55,   115,
     186,   187,  -161,   198,  -161,  -161,  -161,  -161,  -161,  -161,
    -161,  -161,   173,   185,    50,    92,   100,  -161,   104,   298,
      20,   131,  -161,   306,  -161,   203,   205,   206,   207,    17,
     115,   222,   240,  -161,   221,   226,  -161,  -161,    35,   173,
     248,    12,    12,  -161,  -161,   136,  -161,    35,   229,   133,
      90,   230,   149,  -161,  -161,  -161,   108,  -161,   234,   108,
     115,    56,  -161,  -161,  -161,  -161,  -161,  -161,  -161,    25,
     314,    17,  -161,   227,   269,  -161,  -161,  -161,   247,   246,
    -161,   252,   133,  -161,   253,  -161,   289,    90,  -161,    77,
     173,  -161,   173,   153,   115,   255,  -161,    17,   260,  -161,
    -161,   134,   274,  -161,   272,   173,   115,  -161,  -161,  -161,
     115,  -161,   108,   278,    83,   165,  -161,   279,   283,  -161,
       9,   270,   282,   134,  -161,   293,  -161,   133,  -161,   169,
    -161,   181,   173,   286,   115,   296,   115,  -161,  -161,  -161,
    -161,   295,   240,  -161,  -161,    17,   335,  -161,   304,  -161,
    -161,   196,   305,   329,   269,  -161,   313,  -161,  -161,   116,
    -161,   315,  -161,  -161,  -161,  -161,   246,  -161,  -161,  -161
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      17,     0,     0,     3,     0,     0,     0,    88,    60,    61,
      62,    63,    64,     0,     0,     0,     0,     0,   107,     0,
       0,    47,    48,    16,     2,    45,    44,    17,    49,    50,
       0,    28,    46,    51,    54,    53,    52,    55,     0,     0,
      14,     8,    10,     9,     0,     0,     0,     0,     0,     0,
      82,    83,    86,    80,     0,   105,     0,     0,     0,     1,
      15,    28,     0,     0,     0,     0,   127,     0,     0,     0,
      33,     0,    69,    68,    80,    70,    66,    67,     0,     0,
       0,     0,     0,    58,     0,    59,     0,    70,     0,    60,
       0,     0,     0,     0,     0,     0,   104,   111,     0,     0,
       0,     0,    37,     0,    38,    39,    40,   122,   124,   125,
     123,   121,   129,     0,    69,    68,    80,    25,    70,    67,
       0,     0,    30,     0,     3,     0,     0,    76,    75,     0,
       0,     0,    14,    14,     0,     0,     4,    14,     0,   109,
       0,    79,    81,    78,    77,     0,   114,     0,     0,     0,
     111,     0,     0,    41,    35,    36,     0,   126,     0,     0,
       0,    28,    34,    14,    71,    72,    73,    74,    70,    65,
       0,     0,    19,     0,    20,    11,    56,    57,     0,   101,
     115,     0,     0,    87,    84,   116,     0,   111,   110,     0,
     129,   118,   120,     0,     0,     0,     5,     0,    70,    13,
      18,    96,    14,    89,     0,   109,     0,    14,   112,    42,
       0,   128,     0,     0,    84,     0,    31,    70,     0,    12,
       0,     0,     0,    96,    95,     0,   100,     0,   108,     0,
     103,     0,   120,     0,     0,    28,     0,     7,    91,    92,
      93,     0,    14,    90,    94,     0,     0,    85,     0,   119,
     117,     0,     0,    28,    20,    98,    70,    14,    43,    85,
      32,     0,    97,    14,   102,     3,   101,    14,    99,     6
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -161,  -161,  -117,  -161,  -161,  -161,  -161,   -38,  -161,  -161,
     326,  -160,   -58,  -161,  -161,  -161,  -161,  -161,     6,  -161,
    -161,  -161,   -45,   -37,   231,  -161,   -14,     0,  -161,  -161,
     138,  -161,  -161,   101,  -161,  -161,  -161,  -161,   163,  -145,
    -104,   192,   228,  -161,  -161,   143,  -144,  -161,   188
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    20,    40,   178,    21,    22,    23,   172,   219,    80,
      24,   173,    69,   117,    25,    70,    71,    26,   174,    28,
      29,    84,    30,   118,    76,   129,    77,    56,    32,   241,
     222,   223,   224,   203,    33,    34,    35,    57,   181,   148,
     149,   139,   150,    36,    37,   213,   112,   113,   157
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,    75,    79,   100,    55,   188,    27,   163,   156,    90,
      38,    87,   190,   238,   200,   192,   239,    49,    39,    58,
     -88,    50,    51,    72,    73,    78,   -88,    31,   101,   240,
      61,     7,    52,    27,   102,   182,    83,    74,    41,   103,
      50,    51,   208,   138,   104,    82,    85,    86,    93,    94,
       7,    52,   147,   119,   121,    54,    53,   123,   159,    50,
      51,   114,   115,    42,   160,   125,   111,   120,   232,    43,
      52,   126,   127,   128,    54,   116,    44,   141,   142,   143,
     144,   145,   255,    98,   119,   152,   156,    99,   212,   -22,
      45,   -22,   168,    54,   262,   175,    98,    46,   151,   179,
     194,   182,    47,   195,   147,   147,    89,     9,    10,    11,
      48,    12,   107,   108,   109,   110,   170,    59,   209,    50,
      51,   210,   -26,     7,   -26,   196,    81,   234,   212,    88,
      52,   -21,    31,   -21,   198,    74,    96,   138,   180,   -29,
     146,   -29,   147,   -24,    97,   -24,   193,   185,   267,    89,
       9,    10,    11,    54,    12,   -27,   111,   -27,    95,   111,
     217,   220,   221,   105,   226,    91,    92,    93,    94,   230,
      91,    92,    93,    94,    31,   106,   161,   252,   122,   124,
     215,   184,   246,    91,    92,    93,    94,    91,    92,    93,
      94,   131,   229,   132,   189,   261,   231,   133,   214,    91,
      92,    93,    94,    91,    92,    93,    94,   134,   256,   135,
     235,   136,   111,   137,   247,    91,    92,    93,    94,   264,
     251,   140,   253,   146,   158,   266,   248,   153,   154,   269,
      91,    92,    93,    94,    62,    63,    64,    65,    66,   155,
      67,   259,    31,   164,    68,   165,   166,   167,     1,     2,
       3,     4,     5,     6,    31,     7,    89,     9,    10,    11,
     171,    12,   176,   183,    13,    14,    15,   177,   186,    16,
     199,    17,    18,    19,   160,   191,   202,     1,     2,     3,
       4,     5,     6,   -20,     7,    89,     9,    10,    11,   201,
      12,   204,   207,    13,    14,    15,   216,   206,    16,   218,
      17,    18,    19,   225,   227,     1,     2,     3,     4,     5,
       6,   -14,     7,     8,     9,    10,    11,   233,    12,   242,
     236,    13,    14,    15,   237,   243,    16,   250,    17,    18,
      19,   245,    91,    92,    93,    94,    98,   -23,   257,   -23,
      91,    92,    93,    94,   254,   258,   260,   162,    91,    92,
      93,    94,   263,    60,   265,   197,    91,    92,    93,    94,
     169,   244,   130,    91,    92,    93,    94,   268,   228,    98,
      91,    92,    93,    94,   205,   249,     0,   187,   211
};

static const yytype_int16 yycheck[] =
{
       0,    38,    40,    61,    18,   150,     0,   124,   112,    54,
      38,    48,   156,     4,   174,   159,     7,    17,    38,    19,
      38,     4,     5,     6,     7,    39,    44,    27,    34,    20,
      30,    15,    15,    27,    40,   139,    20,    20,    41,    35,
       4,     5,   187,    88,    40,    45,    46,    47,    36,    37,
      15,    15,    97,    67,    68,    38,    20,    71,    38,     4,
       5,     6,     7,    41,    44,    40,    66,    67,   212,    41,
      15,    46,    47,    48,    38,    20,    14,    91,    92,    93,
      94,    95,   242,    40,    98,    99,   190,    44,   192,    39,
      38,    41,   129,    38,   254,   133,    40,    38,    98,   137,
      44,   205,    38,   161,   149,   150,    16,    17,    18,    19,
      38,    21,     4,     5,     6,     7,   130,     0,    41,     4,
       5,    44,    39,    15,    41,   163,    38,    44,   232,    38,
      15,    39,   132,    41,   171,    20,    41,   182,   138,    39,
      50,    41,   187,    39,    38,    41,   160,   147,   265,    16,
      17,    18,    19,    38,    21,    39,   156,    41,    44,   159,
     197,    27,    28,    40,   202,    34,    35,    36,    37,   207,
      34,    35,    36,    37,   174,    40,    45,   235,    41,    39,
     194,    45,   227,    34,    35,    36,    37,    34,    35,    36,
      37,     8,   206,    42,    45,   253,   210,    39,    45,    34,
      35,    36,    37,    34,    35,    36,    37,    39,   245,    39,
      45,    39,   212,    39,    45,    34,    35,    36,    37,   257,
     234,    39,   236,    50,    39,   263,    45,    41,    41,   267,
      34,    35,    36,    37,    34,    35,    36,    37,    38,    41,
      40,    45,   242,    40,    44,    40,    40,    40,     8,     9,
      10,    11,    12,    13,   254,    15,    16,    17,    18,    19,
      38,    21,    41,    15,    24,    25,    26,    41,    39,    29,
      43,    31,    32,    33,    44,    41,    30,     8,     9,    10,
      11,    12,    13,    43,    15,    16,    17,    18,    19,    42,
      21,    39,     3,    24,    25,    26,    41,    44,    29,    39,
      31,    32,    33,    29,    32,     8,     9,    10,    11,    12,
      13,    42,    15,    16,    17,    18,    19,    39,    21,    49,
      41,    24,    25,    26,    41,    43,    29,    41,    31,    32,
      33,    38,    34,    35,    36,    37,    40,    39,     3,    41,
      34,    35,    36,    37,    49,    41,    41,    41,    34,    35,
      36,    37,    39,    27,    39,    41,    34,    35,    36,    37,
     129,   223,    40,    34,    35,    36,    37,   266,   205,    40,
      34,    35,    36,    37,   182,   232,    -1,   149,   190
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,     9,    10,    11,    12,    13,    15,    16,    17,
      18,    19,    21,    24,    25,    26,    29,    31,    32,    33,
      52,    55,    56,    57,    61,    65,    68,    69,    70,    71,
      73,    78,    79,    85,    86,    87,    94,    95,    38,    38,
      53,    41,    41,    41,    14,    38,    38,    38,    38,    78,
       4,     5,    15,    20,    38,    77,    78,    88,    78,     0,
      61,    78,    34,    35,    36,    37,    38,    40,    44,    63,
      66,    67,     6,     7,    20,    74,    75,    77,    77,    58,
      60,    38,    78,    20,    72,    78,    78,    74,    38,    16,
      73,    34,    35,    36,    37,    44,    41,    38,    40,    44,
      63,    34,    40,    35,    40,    40,    40,     4,     5,     6,
       7,    78,    97,    98,     6,     7,    20,    64,    74,    77,
      78,    77,    41,    77,    39,    40,    46,    47,    48,    76,
      40,     8,    42,    39,    39,    39,    39,    39,    73,    92,
      39,    77,    77,    77,    77,    77,    50,    73,    90,    91,
      93,    78,    77,    41,    41,    41,    91,    99,    39,    38,
      44,    45,    41,    53,    40,    40,    40,    40,    74,    75,
      77,    38,    58,    62,    69,    58,    41,    41,    54,    58,
      78,    89,    91,    15,    45,    78,    39,    93,    90,    45,
      97,    41,    97,    77,    44,    63,    58,    41,    74,    43,
      62,    42,    30,    84,    39,    92,    44,     3,    90,    41,
      44,    99,    91,    96,    45,    77,    41,    74,    39,    59,
      27,    28,    81,    82,    83,    29,    58,    32,    89,    77,
      58,    77,    97,    39,    44,    45,    41,    41,     4,     7,
      20,    80,    49,    43,    81,    38,    73,    45,    45,    96,
      41,    77,    63,    77,    49,    62,    74,     3,    41,    45,
      41,    63,    62,    39,    58,    39,    58,    53,    84,    58
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    51,    52,    53,    54,    55,    55,    55,    56,    56,
      56,    57,    58,    59,    60,    61,    61,    61,    62,    62,
      62,    63,    63,    63,    63,    63,    63,    63,    63,    64,
      65,    65,    65,    65,    65,    66,    66,    67,    67,    67,
      67,    68,    68,    68,    69,    69,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    70,    71,    72,    72,
      73,    73,    73,    73,    73,    74,    74,    75,    75,    75,
      75,    76,    76,    76,    76,    76,    76,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    78,    79,
      79,    80,    80,    80,    81,    81,    81,    82,    83,    84,
      84,    84,    85,    86,    87,    88,    88,    88,    89,    89,
      90,    90,    90,    90,    91,    92,    93,    94,    95,    96,
      96,    97,    97,    97,    97,    97,    98,    98,    99,    99
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     0,     6,    13,     8,     2,     2,
       2,     5,     5,     0,     0,     2,     1,     0,     2,     1,
       0,     2,     2,     2,     2,     2,     5,     8,     0,     1,
       3,     6,     9,     2,     4,     3,     3,     2,     2,     2,
       2,     4,     6,     9,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     5,     5,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     1,     1,     3,     3,     3,
       1,     3,     1,     1,     4,     7,     1,     4,     1,     6,
       8,     1,     1,     1,     2,     1,     0,     4,     3,     7,
       2,     0,    10,     7,     3,     1,     1,     0,     3,     0,
       2,     0,     3,     0,     1,     2,     2,     8,     5,     3,
       0,     1,     1,     1,     1,     1,     2,     0,     3,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 355 "sintatica.y" /* yacc.c:1646  */
    {
				desempilhaContexto();

				cout << "#define TRUE 1\n#define FALSE 0\n#define BOOL int\n#define REAL float\n\n" 
				+ declaracoes + "\n" +  (yyvsp[0]).traducao + "\n";

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
#line 1839 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 395 "sintatica.y" /* yacc.c:1646  */
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
#line 1858 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 412 "sintatica.y" /* yacc.c:1646  */
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
#line 1876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 428 "sintatica.y" /* yacc.c:1646  */
    {
				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				//Criação de variável temporária
				string nometemp = genTemp();

				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);
				
				(yyval).traducao = "\n\t" + lacoAtual.labelinicio + ":\n" + 
					(yyvsp[-3]).traducao +
					"\t" + nometemp + " = !" + (yyvsp[-3]).label + ";\n" +
					"\tif(" + nometemp + ")\n\t" + "goto " + lacoAtual.labelfim + ";\n" +
					(yyvsp[0]).traducao +
					"\tgoto " + lacoAtual.labelinicio + ";\n" + 
					"\t" + lacoAtual.labelfim + ":\n\n";

				//Desempilha laço
				pilhaLaco.pop_back();

			}
#line 1903 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 451 "sintatica.y" /* yacc.c:1646  */
    {

				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				//Criação de variável temporária
				string nometemp = genTemp();

				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);		

				(yyval).traducao =   "\t" + (yyvsp[-10]).label + " = " + (yyvsp[-8]).label + "\n\t" + lacoAtual.labelinicio +  "\n" +  (yyvsp[-6]).traducao + "\t" + nometemp + " != " + (yyvsp[-6]).label + "\n" + "\tif(" + nometemp + 
				") goto" + lacoAtual.labelfim + "\n" + (yyvsp[0]).traducao  + (yyvsp[-3]).traducao  + "\t" + (yyvsp[-10]).label + " = " + (yyvsp[-3]).label + "\n\tgoto" +  lacoAtual.labelinicio +  "\n" + "\t" + lacoAtual.labelfim + ":" "\n";

				//Desempilha laço
				pilhaLaco.pop_back();
			}
#line 1925 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 468 "sintatica.y" /* yacc.c:1646  */
    {

				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				(yyval).traducao = "\t" + lacoAtual.labelinicio + "\n"+ (yyvsp[-5]).traducao + "\tif(" + (yyvsp[-2]).label + ") goto " + lacoAtual.labelinicio + "\n\t" + lacoAtual.labelfim + "\n";

				//Desempilha laço
				pilhaLaco.pop_back();

			}
#line 1941 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 482 "sintatica.y" /* yacc.c:1646  */
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
				(yyval).traducao = "\tgoto " + lacoAtual.labelfim + ";\n";

			}
#line 1961 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 498 "sintatica.y" /* yacc.c:1646  */
    {
				//Verifica se há um contexto de laço em questão
				if (pilhaLaco.size() == 0)
				{
					cout << "\tbreak fora de um laço!\n";
					exit(3);
				}

				laco lacoAtual = pilhaLaco.front();
				(yyval).traducao = "\tgoto " + lacoAtual.labelfim + ";\n";

			}
#line 1978 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 511 "sintatica.y" /* yacc.c:1646  */
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
				(yyval).traducao = "\tgo to " + lacoAtual.labelinicio + ";\n";

			}
#line 2016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 547 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\nint main (void)\n{\n" + (yyvsp[0]).traducao;
			}
#line 2024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 553 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao;

			}
#line 2033 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 560 "sintatica.y" /* yacc.c:1646  */
    {

				desempilhaContexto();
			}
#line 2042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 567 "sintatica.y" /* yacc.c:1646  */
    {

				
				empilhaContexto();
			}
#line 2052 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 574 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 2060 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 578 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2068 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 582 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 2076 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 588 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
				
			}
#line 2085 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 593 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2093 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 597 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 2101 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 605 "sintatica.y" /* yacc.c:1646  */
    {

				(yyval).label = (yyvsp[0]).label;
				(yyval).tipo = "char";
			}
#line 2111 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 611 "sintatica.y" /* yacc.c:1646  */
    {

				//$$.label = $2.label;
			if ((yyvsp[0]).label.compare("true"))
				(yyval).label = "FALSE";
			else
				(yyval).label = "TRUE";

				(yyval).tipo = "BOOL";
			}
#line 2126 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 622 "sintatica.y" /* yacc.c:1646  */
    {
				
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2137 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 630 "sintatica.y" /* yacc.c:1646  */
    {
				
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2148 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 637 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 643 "sintatica.y" /* yacc.c:1646  */
    {

				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-3]).label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				(yyval).traducao = "";
				(yyval).tipo = var.tipo;
				(yyval).label = "*(" + var.identificacao + " + " + (yyvsp[-1]).label + ")";

			}
#line 2177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 658 "sintatica.y" /* yacc.c:1646  */
    {
				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-6]).label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				(yyval).traducao = "";
				(yyval).tipo = var.tipo;
				(yyval).label = "*(" + var.identificacao + " + " + (yyvsp[-4]).label + " * " + var.identificacao + "_d + " + (yyvsp[-1]).label + ")";

			}
#line 2195 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 672 "sintatica.y" /* yacc.c:1646  */
    {
			
				(yyval).tipo = "null";
			}
#line 2204 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 680 "sintatica.y" /* yacc.c:1646  */
    {

				//Cria e insere variáveis
				string nomeTemp1 = genTemp();
				insereVariavel(genNomeGen(), "char*" , nomeTemp1);

				string nomeTemp2 = nomeTemp1 + "_s";
				insereVariavel(genNomeGen(), "int" , nomeTemp2);

				int size = (yyvsp[0]).label.length() - 1;

				(yyval).traducao = "\t" + nomeTemp2 + " = " + to_string(size) + ";\n"
				+ "\t" + nomeTemp1 + " = (char*) malloc (sizeof(char) * " + nomeTemp2 + ");\n"
				+ "\t" + "strcpy(" + nomeTemp1 + ", " + (yyvsp[0]).label + ");\n";

				(yyval).tipo = "char*";

				(yyval).label = nomeTemp1;
			}
#line 2228 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 703 "sintatica.y" /* yacc.c:1646  */
    {
				
				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-2]).label, var);
				
				//Compara atribuição 
				verificaAtribuicao(var.tipo, (yyvsp[-1]).tipo);
				
				//Verifica se teve atribuição
				if ((yyvsp[-1]).tipo.compare("null"))
				{
					if (var.tipo.compare((yyvsp[-1]).tipo))
						(yyval).traducao = (yyvsp[-1]).traducao + "\t" + var.identificacao + " = (" + var.tipo + ") " + (yyvsp[-1]).label + ";\n";
					else
						(yyval).traducao = (yyvsp[-1]).traducao + "\t" + var.identificacao + " = " + (yyvsp[-1]).label + ";\n";
				}
			}
#line 2252 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 723 "sintatica.y" /* yacc.c:1646  */
    {
				
				if ((yyvsp[-3]).tipo.compare("int"))
				{
					cout << "\tErro: O índice de um array deve ser inteiro!\n";
					exit(1);
				}

				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-5]).label, var);
				
				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				//Compara atribuição 
				verificaAtribuicao(var.tipo, (yyvsp[-1]).tipo);
				
				//Verifica se teve atribuição
				if ((yyvsp[-1]).tipo.compare("null"))
				{
					if (var.tipo.compare((yyvsp[-1]).tipo))
						(yyval).traducao = (yyvsp[-3]).traducao 
						+ (yyvsp[-1]).traducao + "\t" + "(" + var.identificacao + " + " + (yyvsp[-3]).label + ")* = (" + var.tipo + ") " + (yyvsp[-1]).label + ";\n";
					else
						(yyval).traducao = (yyvsp[-3]).traducao 
						+ (yyvsp[-1]).traducao + "\t" + "(" + var.identificacao + " + " + (yyvsp[-3]).label + ")* = " + (yyvsp[-1]).label + ";\n";
				}
			}
#line 2286 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 753 "sintatica.y" /* yacc.c:1646  */
    {
				
				if ((yyvsp[-6]).tipo.compare("int") && (yyvsp[-3]).tipo.compare("int"))
				{
					cout << "\tErro: O índice de um array deve ser inteiro!\n";
					exit(1);
				}

				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-8]).label, var);
				
				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				//Compara atribuição 
				verificaAtribuicao(var.tipo, (yyvsp[-1]).tipo);
				
				//Verifica se teve atribuição
				if ((yyvsp[-1]).tipo.compare("null"))
				{
					if (var.tipo.compare((yyvsp[-1]).tipo))
						(yyval).traducao = (yyvsp[-6]).traducao + (yyvsp[-3]).traducao + (yyvsp[-1]).traducao +
						"\t" + "(" + var.identificacao + " + " + (yyvsp[-6]).label + " * " + var.identificacao + "_d + " + (yyvsp[-3]).label + ")* = (" + var.tipo + ") " + (yyvsp[-1]).label + ";\n";
					else
						(yyval).traducao = (yyvsp[-6]).traducao + (yyvsp[-3]).traducao + (yyvsp[-1]).traducao +
						"\t" + "(" + var.identificacao + " + " + (yyvsp[-6]).label + " * " + var.identificacao + "_d + " + (yyvsp[-3]).label + ")* = " + (yyvsp[-1]).label + ";\n";
				}
			}
#line 2320 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 783 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;
				
				buscaVariavel((yyvsp[-1]).label, var);

				if((!var.tipo.compare("int") || !var.tipo.compare("real"))){
					(yyval).traducao = "\t" + var.identificacao + " = " + var.identificacao + " " + (yyvsp[0]).traducao + " " +"1;\n";
				}
				else{
					cout << "Esse tipo de variavel não pode ter um operador unário.\n";	
					exit(1);
				}

			}
#line 2339 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 798 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;
				
				buscaVariavel((yyvsp[-3]).label, var);

				
					(yyval).traducao =   (yyvsp[-1]).traducao + "\t" + var.identificacao + " = " + var.identificacao + " " + (yyvsp[-2]).traducao  + (yyvsp[-1]).label + ";\n";
				
			}
#line 2353 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 809 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = '+';
			}
#line 2361 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 813 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = '-';
			}
#line 2369 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 817 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = '+';
			}
#line 2377 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 821 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = '-';
			}
#line 2385 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 825 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = '*';
			}
#line 2393 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 829 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = '/';
			}
#line 2401 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 835 "sintatica.y" /* yacc.c:1646  */
    {

				//Criação de variável temporária
				string nomeTemp = genTemp();
				
				
				//Tenta inserir variável
				insereVariavel((yyvsp[-2]).label, (yyvsp[-3]).tipo , nomeTemp);

				if (!(yyvsp[-3]).tipo.compare("char*"))
					insereVariavel(genNomeGen(), "int" , nomeTemp + "_s");

				//Verifica se a atribuição pode ocorrer de acordo com os tipos
				verificaAtribuicao((yyvsp[-3]).tipo, (yyvsp[-1]).tipo);

				//Verifica se teve atribuição
				if ((yyvsp[-1]).tipo.compare("null"))
				{
					if ((yyvsp[-3]).tipo.compare((yyvsp[-1]).tipo))
						(yyval).traducao = (yyvsp[-1]).traducao + "\t" + nomeTemp + " = (" + (yyvsp[-3]).tipo + ") " + (yyvsp[-1]).label + ";\n";
					else
						(yyval).traducao = (yyvsp[-1]).traducao + "\t" + nomeTemp + " = " + (yyvsp[-1]).label + ";\n";
				}
			}
#line 2430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 860 "sintatica.y" /* yacc.c:1646  */
    {

				if ((yyvsp[-2]).tipo.compare("int"))
				{
					cout << "\tErro: O tamanho de um array é inteiro!\n";
					exit(1);
				}

				//Criação de variável temporária
				string nomeTemp = genTemp();
				string nomeTemp1 = nomeTemp;

				nomeTemp = nomeTemp + "_s";

				string tipoBase = (yyvsp[-5]).tipo;
				(yyvsp[-5]).tipo = (yyvsp[-5]).tipo + "*";
	
				
				//Tenta inserir variável
				insereVariavel((yyvsp[-4]).label, (yyvsp[-5]).tipo , nomeTemp1);
				insereVariavel(genNomeGen(), "int" , nomeTemp);

				//Verifica se teve atribuição
				(yyval).traducao = (yyvsp[-2]).traducao + "\t" + nomeTemp + " = " + (yyvsp[-2]).label + ";\n"
				+ "\t" + nomeTemp1 + " = (" + (yyvsp[-5]).tipo + ") malloc (" + nomeTemp + " * sizeof(" + tipoBase + "));\n";
				

			}
#line 2463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 889 "sintatica.y" /* yacc.c:1646  */
    {

				if ((yyvsp[-5]).tipo.compare("int") && (yyvsp[-2]).tipo.compare("int"))
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

				string tipoBase = (yyvsp[-8]).tipo;
				(yyvsp[-8]).tipo = (yyvsp[-8]).tipo + "*";
	
				
				//Tenta inserir variável
				insereVariavel((yyvsp[-7]).label, (yyvsp[-8]).tipo , nomeTemp);
				insereVariavel(genNomeGen(), "int" , nomeTemp1);
				insereVariavel(genNomeGen(), "int" , nomeTemp2);

				//Declara
				(yyval).traducao = (yyvsp[-5]).traducao + (yyvsp[-2]).traducao + 
				"\t" + nomeTemp1 + " = " + (yyvsp[-5]).label + ";\n" +
				"\t" + nomeTemp2 + " = " + (yyvsp[-2]).label + ";\n" +
				"\t" + nomeTemp + " = (" + (yyvsp[-8]).tipo + ") malloc (" + nomeTemp1 + " * " + nomeTemp2 + " * sizeof(" + tipoBase + "));\n";
				

			}
#line 2501 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 926 "sintatica.y" /* yacc.c:1646  */
    {
				//Transfere para tradução de comando a tradução de DECLARACAO
				(yyval).traducao =  (yyvsp[0]).traducao;
			}
#line 2510 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 931 "sintatica.y" /* yacc.c:1646  */
    {
				//Transfere para tradução de comando a tradução de ATRIBUICAO
				(yyval).traducao =  (yyvsp[0]).traducao;
			}
#line 2519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 936 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 940 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2535 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 944 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2543 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 948 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2551 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 952 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2559 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 956 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2567 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 960 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2575 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 964 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2583 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 968 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2591 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 972 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2599 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 978 "sintatica.y" /* yacc.c:1646  */
    {
				//Busca na tabela
				variavel var;

				//Tenta buscar a variável
				buscaVariavel((yyvsp[-2]).label, var);

				(yyval).traducao = "\tcin >> " + var.identificacao + ";\n";
			}
#line 2613 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 990 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao 
				+ "\tcout >> " + (yyvsp[-2]).label + ";\n";	
			}
#line 2622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 996 "sintatica.y" /* yacc.c:1646  */
    {
				//Cria e insere variáveis
				string nomeTemp1 = genTemp();
				insereVariavel(genNomeGen(), "char*" , nomeTemp1);

				string nomeTemp2 = nomeTemp1 + "_s";
				insereVariavel(genNomeGen(), "int" , nomeTemp2);

				int size = (yyvsp[0]).label.length() - 1;

				(yyval).traducao = "\t" + nomeTemp2 + " = " + to_string(size) + ";\n"
				+ "\t" + nomeTemp1 + " = (char*) malloc (sizeof(char) * " + nomeTemp2 + ");\n"
				+ "\t" + "strcpy(" + nomeTemp1 + ", " + (yyvsp[0]).label + ");\n";

				(yyval).tipo = "char*";

				(yyval).label = nomeTemp1;


				//$$.traducao =  $1.label;
			}
#line 2648 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1018 "sintatica.y" /* yacc.c:1646  */
    {
				//Busca na tabela
				variavel var;

				//Tenta buscar a variável
				buscaVariavel((yyvsp[0]).label, var);

				(yyval).label = var.identificacao;
			}
#line 2662 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1030 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "int";

			}
#line 2671 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1035 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "REAL";
			}
#line 2679 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1039 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "BOOL";
			}
#line 2687 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1043 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
			}
#line 2695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1047 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char*";

			}
#line 2704 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1055 "sintatica.y" /* yacc.c:1646  */
    {

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);

				//Guarda o tipo da Expressão resultante em E
				(yyval).tipo = "BOOL";


				//Verifica a conversão int para bool
				if (!(yyvsp[-2]).tipo.compare("BOOL") && !(yyvsp[0]).tipo.compare("int"))
				{

					//Verifica se veio apenas um número
					if (!((yyvsp[0]).label.find("temp") != std::string::npos)) 
					{
    					//Criação de variável temporária
						string nometemp1_b = genTemp();
						//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
						//Adiciona na tabela
						insereVariavel(genNomeGen(), "int", nometemp1_b);

						(yyvsp[0]).traducao = (yyvsp[0]).traducao +
						"\t" + nometemp1_b + " = " + (yyvsp[0]).label + ";\n";

						(yyvsp[0]).label = nometemp1_b;
					}


					//Criação de variável temporária
					string nometemp1 = genTemp();
					//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
					//Adiciona na tabela
					insereVariavel(genNomeGen(), "BOOL", nometemp1);

					//Transforma de inteiro para bool
					(yyvsp[0]).traducao = (yyvsp[0]).traducao 
					+ "\t" + nometemp1 + " = " + (yyvsp[0]).label + " != 0;\n"
					+ "\tif(" + nometemp1 + ")" 
					+ "\n\t" + (yyvsp[0]).label + " = TRUE;\n";

				}
				else if (!(yyvsp[0]).tipo.compare("BOOL") && !(yyvsp[-2]).tipo.compare("int"))
				{
					//Verifica se veio apenas um número
					if (!((yyvsp[-2]).label.find("temp") != std::string::npos)) 
					{
    					//Criação de variável temporária
						string nometemp1_b = genTemp();
						//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
						//Adiciona na tabela
						insereVariavel(genNomeGen(), "int", nometemp1_b);

						(yyvsp[-2]).traducao = (yyvsp[-2]).traducao +
						"\t" + nometemp1_b + " = " + (yyvsp[-2]).label + ";\n";

						(yyvsp[-2]).label = nometemp1_b;
					}


					//Criação de variável temporária
					string nometemp1 = genTemp();
					//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
					//Adiciona na tabela
					insereVariavel(genNomeGen(), "BOOL", nometemp1);

					//Transforma de inteiro para bool
					(yyvsp[-2]).traducao = (yyvsp[-2]).traducao 
					+ "\t" + nometemp1 + " = " + (yyvsp[-2]).label + " != 0;\n"
					+ "\tif(" + nometemp1 + ")" 
					+ "\n\t" + (yyvsp[-2]).label + " = TRUE;\n";
				}

				//Passa para EL a tradução
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao
				+ "\t" + nometemp + " = " + (yyvsp[-2]).label + (yyvsp[-1]).traducao + (yyvsp[0]).label + ";\n";

				//Passa para E seu valor de temporária
				(yyval).label = nometemp;
			}
#line 2793 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1140 "sintatica.y" /* yacc.c:1646  */
    {

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), "BOOL", nometemp);

				//Guarda o tipo da Expressão resultante em E
				(yyval).tipo = "BOOL";
				//Passa para E seu valor de temporária
				(yyval).label = nometemp;

				(yyval).traducao = "\t" + nometemp + " = " + (yyvsp[0]).label + " != 0;\n";
			}
#line 2814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1159 "sintatica.y" /* yacc.c:1646  */
    {
				//Criação de variável temporária
				//string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				//insereVariavel(genNomeGen(), "BOOL", nometemp);



				(yyval).traducao = (yyvsp[0]).traducao ;
				//+ "\t" + nometemp + " = " + $1.label + " != 0;\n"
				//+ "\tif(" + nometemp + ")" 
				//+ "\n\t\t" + $1.label + " = TRUE;\n";
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = (yyvsp[0]).label;
			}
#line 2836 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1177 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2845 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1182 "sintatica.y" /* yacc.c:1646  */
    {

			if ((yyvsp[0]).label.compare("true"))
				(yyval).label = "FALSE";
			else
				(yyval).label = "TRUE";

				(yyval).tipo = "BOOL";
			}
#line 2859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1192 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = (yyvsp[0]).label;
			}
#line 2869 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1200 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " == ";
			}
#line 2877 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1204 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " != ";
			}
#line 2885 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1208 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " <= ";
			}
#line 2893 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1212 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " >= ";
			}
#line 2901 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1216 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " > ";
			}
#line 2909 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1220 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " < ";
			}
#line 2917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1226 "sintatica.y" /* yacc.c:1646  */
    {

				if (!(yyvsp[-2]).tipo.compare("char*") && !(yyvsp[0]).tipo.compare("char*"))
				{
					cout << "\tNão é possível operação de divisão com strings!\n";
					exit(1);
				}

				//Verifica se a expressão é válida
				atualizaRegraExprAritimetica((yyvsp[-2]), (yyvsp[0]));

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), (yyvsp[-2]).tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				(yyval).tipo = (yyvsp[-2]).tipo;

				//Passa para E a tradução
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t" + nometemp + " = " + (yyvsp[-2]).label + " / " + (yyvsp[0]).label + ";\n";

				//Passa para E seu valor de temporária
				(yyval).label = nometemp;
			}
#line 2951 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1256 "sintatica.y" /* yacc.c:1646  */
    {
				//Verifica se a expressão é válida
				atualizaRegraExprAritimetica((yyvsp[-2]), (yyvsp[0]));

					if (!(yyvsp[-2]).tipo.compare("char*") && !(yyvsp[0]).tipo.compare("char*"))
				{
					cout << "\tNão é possível operação de subtração com strings!\n";
					exit(1);
				}

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), (yyvsp[-2]).tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				(yyval).tipo = (yyvsp[-2]).tipo;

				//Passa para E a tradução
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t" + nometemp + " = " + (yyvsp[-2]).label + " * " + (yyvsp[0]).label + ";\n";

				//Passa para E seu valor de temporária
				(yyval).label = nometemp;

			}
#line 2985 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1286 "sintatica.y" /* yacc.c:1646  */
    {

				
				//Verifica se a expressão é válida
				atualizaRegraExprAritimetica((yyvsp[-2]), (yyvsp[0]));

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), (yyvsp[-2]).tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				(yyval).tipo = (yyvsp[-2]).tipo;

				if (!(yyvsp[-2]).tipo.compare("char*") && !(yyvsp[0]).tipo.compare("char*"))
				{

					//Insere nova variável de tamanho
					insereVariavel(genNomeGen(), "int", nometemp + "_s");

					//Passa para E a tradução
					(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
					+ "\t" + nometemp + "_s = " + (yyvsp[-2]).label + "_s + " + (yyvsp[0]).label + "_s;\n"
					+ "\t" + nometemp + "_s = " + nometemp + "_s - 1;\n" 
					+ "\t" + nometemp + " = (" + (yyvsp[-2]).tipo + ") malloc (" + nometemp + "_s * sizeof(" + (yyvsp[-2]).tipo.substr(0, (yyvsp[-2]).tipo.size() -1) + ");\n" 
					+ "\tstrcpy(" + nometemp + ", " + (yyvsp[-2]).label + ");\n"
					+ "\tstrcat(" + nometemp + ", " + (yyvsp[0]).label + ");\n";

				}
				else
				{
					//Passa para E a tradução
					(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
					+ "\t" + nometemp + " = " + (yyvsp[-2]).label + " + " + (yyvsp[0]).label + ";\n";
				}
				
				//Passa para E seu valor de temporária
				(yyval).label = nometemp;

			}
#line 3032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1329 "sintatica.y" /* yacc.c:1646  */
    {

				//Cria e insere variáveis
				string nomeTemp1 = genTemp();
				insereVariavel(genNomeGen(), "char*" , nomeTemp1);

				string nomeTemp2 = nomeTemp1 + "_s";
				insereVariavel(genNomeGen(), "int" , nomeTemp2);

				int size = (yyvsp[0]).label.length() - 1;

				(yyval).traducao = "\t" + nomeTemp2 + " = " + to_string(size) + ";\n"
				+ "\t" + nomeTemp1 + " = (char*) malloc (sizeof(char) * " + nomeTemp2 + ");\n"
				+ "\t" + "strcpy(" + nomeTemp1 + ", " + (yyvsp[0]).label + ");\n";

				(yyval).tipo = "char*";

				(yyval).label = nomeTemp1;
			}
#line 3056 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1349 "sintatica.y" /* yacc.c:1646  */
    {

				if (!(yyvsp[-2]).tipo.compare("char*") && !(yyvsp[0]).tipo.compare("char*"))
				{
					cout << "\tNão é possível operação de subtração com strings!\n";
					exit(1);
				}

				//Verifica se a expressão é válida
				atualizaRegraExprAritimetica((yyvsp[-2]), (yyvsp[0]));

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), (yyvsp[-2]).tipo, nometemp);

				//Guarda o tipo da Expressão resultante em E
				(yyval).tipo = (yyvsp[-2]).tipo;

				//Passa para E a tradução
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t"  + nometemp + " = " + (yyvsp[-2]).label + " - " + (yyvsp[0]).label + ";\n";

				//Passa para E seu valor de temporária
				(yyval).label = nometemp;

			}
#line 3091 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1380 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "int";
				(yyval).label = (yyvsp[0]).label;
			}
#line 3101 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1386 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "REAL";
				(yyval).label = (yyvsp[0]).label;
			}
#line 3111 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1392 "sintatica.y" /* yacc.c:1646  */
    {

				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-3]).label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				if (var.tipo.compare("int") && var.tipo.compare("REAL"))
				{
					cout << "\tNão é possível operações aritméticas com " + var.tipo + "!\n";
					exit(1);
				}

				(yyval).traducao = (yyvsp[-1]).traducao;
				(yyval).tipo = var.tipo;
				(yyval).label = "*(" + var.identificacao + " + " + (yyvsp[-1]).label + ")";
			}
#line 3135 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1412 "sintatica.y" /* yacc.c:1646  */
    {

				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-6]).label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				if (var.tipo.compare("int") && var.tipo.compare("REAL"))
				{
					cout << "\tNão é possível operações aritméticas com " + var.tipo + "!\n";
					exit(1);
				}

				(yyval).traducao = (yyvsp[-4]).traducao + (yyvsp[-1]).traducao;
				(yyval).tipo = var.tipo;
				(yyval).label = "*(" + var.identificacao + " + " + (yyvsp[-4]).label + " * " + var.identificacao + "_d" + " + " + (yyvsp[-1]).label + ")";
			}
#line 3159 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1433 "sintatica.y" /* yacc.c:1646  */
    {

				//Busca na tabela
				variavel var;
				

				//Tenta buscar a variável
				
				buscaVariavel((yyvsp[0]).label, var);

				//Passa o tipo e o nome para E
				(yyval).tipo = var.tipo;
				(yyval).label = var.identificacao; 
			}
#line 3178 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1448 "sintatica.y" /* yacc.c:1646  */
    {
				//Busca na tabela
				variavel var;
				
				//Tenta buscar a variável
				buscaVariavel((yyvsp[0]).label, var);

				//Criação de variável temporária
				string nometemp = genTemp();

				//Ja foram convertidas se era possível, basta pegar o tipo de qualquer  um
				//Adiciona na tabela
				insereVariavel(genNomeGen(), (yyvsp[-2]).tipo, nometemp);

				(yyval).traducao = "\t" + nometemp + " = (" + (yyvsp[-2]).tipo + ") " + var.identificacao + "\n";

				//Passa o tipo e o nome para E
				(yyval).tipo = (yyvsp[-2]).tipo;
				(yyval).label = nometemp; 
			}
#line 3203 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1471 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa seu nome literal para ID

				(yyval).label = (yyvsp[0]).label;
			}
#line 3213 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1478 "sintatica.y" /* yacc.c:1646  */
    {
				
				string nometemp = genTemp();
				insereVariavel(genNomeGen(), "BOOL", nometemp);
				string label = labelStackEnd.top();
				labelStackEnd.pop();

				(yyval).traducao = (yyvsp[-3]).traducao + "\t" + nometemp + " = !" + 
				(yyvsp[-3]).label + ";\n" + "\tif" + "(" + nometemp + ")" + "\n\tgoto " + label + ";\n" + 
				(yyvsp[-1]).traducao + "\tgoto " + (yyvsp[0]).tipo + ";\n" + (yyvsp[0]).traducao +"\t"+ (yyvsp[0]).tipo + ":\n";
			}
#line 3229 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1490 "sintatica.y" /* yacc.c:1646  */
    {	
				variavel var;
				_switch swt = switchVar.top();
				switchVar.pop();
				laco swAtual = pilhaLaco.back();
								
				buscaVariavel((yyvsp[-5]).label, var);

				if(var.tipo.compare(swt.tipo))
				{
					cout << "switch and case have different types\n";
					exit(1);
				}

				(yyval).traducao = "\t" + swAtual.labelinicio + ":\n\t" + swt.comp + " = " + 
				var.identificacao + ";\n" + (yyvsp[-1]).traducao + "\t" + swAtual.labelfim + ":\n";

				 //Desempilha switch
				pilhaLaco.pop_back();
			}
#line 3254 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1512 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "int";
				(yyval).label = (yyvsp[0]).label;
			}
#line 3264 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1518 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
				(yyval).label = (yyvsp[0]).label;
			}
#line 3273 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1523 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "string";
				(yyval).label = (yyvsp[0]).label;
			}
#line 3282 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1529 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3290 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1533 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3298 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1537 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3306 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1543 "sintatica.y" /* yacc.c:1646  */
    {
				_switch swt;
				string labelAt = gerarLabel(0);
				if(switchVar.empty())
				{
					swt.var = genTemp();
					swt.comp = genTemp();
					swt.res = genTemp();
					swt.tipo = (yyvsp[-2]).tipo;
					switchVar.push(swt);
					insereVariavel(genNomeGen(), (yyvsp[-2]).tipo, swt.var);
					insereVariavel(genNomeGen(), (yyvsp[-2]).tipo, swt.res);
					insereVariavel(genNomeGen(), (yyvsp[-2]).tipo, swt.comp);
					
				}

				swt = switchVar.top();
				
				if(swt.tipo.compare((yyvsp[-2]).tipo))
				{
					cout << "Error in type switch case " + (yyvsp[-2]).tipo + " not equal " + swt.tipo + "\n";
					exit(1);
				}
				
				(yyval).traducao = "\t" + swt.var + " = " + (yyvsp[-2]).label + ";\n\t" + 
				swt.res + " = " + "!(" + swt.comp + " == " + swt.var + ");" + "\n\t"
				"if(" + swt.res + ")" + "\n\t" + "goto " + labelAt + ";"+ "\n" + 
				(yyvsp[0]).traducao + "\t" + labelAt + ":" + "\n" ;
			}
#line 3340 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1574 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3348 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1579 "sintatica.y" /* yacc.c:1646  */
    {
				
				string nometemp = genTemp();
				string labelInit = gerarLabel(0);
				string labelEnd = labelStackEnd.top();
				labelStackEnd.pop();		

				insereVariavel(genNomeGen(), "BOOL", nometemp);

				(yyval).traducao = "\t" + labelInit + ":\n"+ (yyvsp[-3]).traducao + "\t" + nometemp + "= !" + 
				(yyvsp[-3]).label + ";\n\tif" + "(" + nometemp + ")" + "\n\tgoto " + labelEnd + ";\n" + 
				(yyvsp[-1]).traducao + "\tgoto " + (yyvsp[0]).tipo + ";\n" + (yyvsp[0]).traducao ;
				(yyval).tipo = (yyvsp[0]).tipo;

				labelEnd = gerarLabel(-1);
				labelStackEnd.push(labelEnd);
			}
#line 3370 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1597 "sintatica.y" /* yacc.c:1646  */
    {
				
				string label = gerarLabelEndif();
				string labelelse = gerarLabel(0);
				labelStackEnd.push(label);
				labelStackEnd.push(labelelse);
				(yyval).tipo = label;
				(yyval).traducao = "\t" + labelelse + ":\n" + (yyvsp[0]).traducao;

			}
#line 3385 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1608 "sintatica.y" /* yacc.c:1646  */
    {
				string label = gerarLabelEndif();
				labelStackEnd.push(label);
				(yyval).tipo = label;
			}
#line 3395 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1615 "sintatica.y" /* yacc.c:1646  */
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
						if(!temp.nome.compare((yyvsp[-8]).label))
						{
							cout << "variavel com este nome";
							exit(0);
						}
					}
				}

				func.functionContext = funcContext;
				func.label =  (yyvsp[-8]).label;
				funcContext.erase(funcContext.begin(), funcContext.end());
				functions.push_back(func);
				funcQtt++;
				(yyval).traducao = (yyvsp[-2]).tipo + " " + (yyvsp[-8]).label  + 
				"(" + (yyvsp[-6]).traducao + (yyvsp[-5]).traducao + ")\n{\n" + (yyvsp[0]).traducao + "\n}\n";
			}
#line 3432 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1649 "sintatica.y" /* yacc.c:1646  */
    {
				_procedure proc;
				proc.procedureContext = procContext;
				proc.label = (yyvsp[-5]).label;
				procContext.erase(procContext.begin(), procContext.end());
				procedures.push_back(proc);

				(yyval).traducao = "void " + (yyvsp[-5]).label + "(" + (yyvsp[-3]).traducao + ")" +
				"\n{\n" + (yyvsp[0]).traducao + "\n}\n";
			}
#line 3447 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1661 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\treturn " + (yyvsp[-1]).traducao + ";\n";
			}
#line 3455 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1666 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).label;
			}
#line 3463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1670 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1674 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3479 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1679 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3487 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1683 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3495 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1689 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3503 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1693 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3511 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1698 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3519 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1702 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3527 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1707 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = ", ";
			}
#line 3535 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1712 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp();
				var.tipo = (yyvsp[-1]).tipo;
				var.nome = (yyvsp[0]).label;//genNomeGen();
				funcContext.push_back(var);

				(yyval).traducao = (yyvsp[-1]).tipo + " " + var.identificacao;
			}
#line 3550 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1725 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp();
				var.tipo = (yyvsp[-1]).tipo;
				var.nome = (yyvsp[0]).label;//genNomeGen();
				procContext.push_back(var);

				(yyval).traducao = (yyvsp[-1]).tipo + " " + var.identificacao;
			}
#line 3565 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1739 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				buscaVariavel((yyvsp[-7]).label, var);

				for(vector<_function>::iterator f = functions.begin(); f != functions.end(); f++)
				{
					_function temp = *f;
					if(!temp.label.compare((yyvsp[-5]).label))
					{
						break;
					}
				}

				(yyval).traducao = "\t" + var.identificacao + " = " + (yyvsp[-5]).label + "(" +
				(yyvsp[-3]).traducao + (yyvsp[-2]).traducao + ")" + ";\n";
			}
#line 3587 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1758 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\t" + (yyvsp[-4]).label + "(" + (yyvsp[-2]).traducao + ")"+ ";\n";
			}
#line 3595 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1763 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3603 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1767 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3611 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1772 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				buscaVariavel((yyvsp[0]).label, var);

				(yyval).traducao = var.identificacao;
				(yyval).label = (yyvsp[0]).label;
			}
#line 3624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1781 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp() ;
				var.tipo = "const int";
				var.nome = (yyvsp[0]).label;
				cout << (yyvsp[0]).tipo;
				insereVariavel((yyvsp[0]).label, "const int", var.identificacao + " = " + (yyvsp[0]).label);
				(yyval).traducao = var.identificacao;
			}
#line 3639 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1792 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp() ;
				var.tipo = "const char";
				var.nome = (yyvsp[0]).label;
				cout << (yyvsp[0]).tipo;
				insereVariavel((yyvsp[0]).label, "const char", var.identificacao + " = " + (yyvsp[0]).label);
				(yyval).traducao = var.identificacao;
			}
#line 3654 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1803 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp() ;
				var.tipo = "const float";
				var.nome = (yyvsp[0]).label;
				cout << (yyvsp[0]).tipo;
				insereVariavel((yyvsp[0]).label, "const float", var.identificacao + " = " + (yyvsp[0]).label);
				(yyval).traducao = var.identificacao;
			}
#line 3669 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1814 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp() ;
				var.tipo = "const BOOL";
				var.nome = (yyvsp[0]).label;
				cout << (yyvsp[0]).tipo;
				insereVariavel((yyvsp[0]).label, "const BOOL", var.identificacao + " = " + (yyvsp[0]).label);
				(yyval).traducao = var.identificacao;
			}
#line 3684 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1828 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3692 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1832 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3700 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1837 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3708 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1841 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3716 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3720 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1846 "sintatica.y" /* yacc.c:1906  */


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
