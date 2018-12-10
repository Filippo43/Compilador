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

#line 403 "y.tab.c" /* yacc.c:339  */

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
    TK_MAIN = 268,
    TK_ID = 269,
    TK_TIPO_INT = 270,
    TK_TIPO_REAL = 271,
    TK_TIPO_BOOL = 272,
    TK_TIPO_CHAR = 273,
    TK_STRING = 274,
    TK_TIPO_STRING = 275,
    TK_FIM = 276,
    TK_ERROR = 277,
    TK_INPUT = 278,
    TK_OUTPUT = 279,
    TK_SWITCH = 280,
    TK_CASE = 281,
    TK_DEFAULT = 282,
    TK_IF = 283,
    TK_ELSE = 284,
    TK_FUNCTION = 285,
    TK_RETURN = 286,
    TK_PROCEDURE = 287
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
#define TK_MAIN 268
#define TK_ID 269
#define TK_TIPO_INT 270
#define TK_TIPO_REAL 271
#define TK_TIPO_BOOL 272
#define TK_TIPO_CHAR 273
#define TK_STRING 274
#define TK_TIPO_STRING 275
#define TK_FIM 276
#define TK_ERROR 277
#define TK_INPUT 278
#define TK_OUTPUT 279
#define TK_SWITCH 280
#define TK_CASE 281
#define TK_DEFAULT 282
#define TK_IF 283
#define TK_ELSE 284
#define TK_FUNCTION 285
#define TK_RETURN 286
#define TK_PROCEDURE 287

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

#line 518 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  56
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   289

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  108
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  216

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   287

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    43,     2,     2,     2,     2,     2,     2,
      37,    38,    35,    33,    47,    34,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    46,    40,
      44,    39,    45,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,     2,    42,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   353,   353,   394,   411,   426,   449,   467,   480,   496,
     532,   538,   546,   553,   559,   563,   568,   573,   578,   583,
     590,   596,   607,   615,   622,   629,   636,   659,   682,   707,
     712,   717,   721,   725,   729,   733,   737,   741,   745,   749,
     753,   759,   771,   777,   799,   811,   816,   820,   824,   828,
     836,   921,   940,   958,   963,   973,   981,   985,   989,   993,
     997,  1001,  1007,  1030,  1054,  1078,  1102,  1108,  1116,  1131,
    1153,  1160,  1172,  1194,  1200,  1205,  1211,  1215,  1220,  1225,
    1256,  1261,  1279,  1291,  1297,  1309,  1311,  1323,  1328,  1332,
    1337,  1341,  1346,  1351,  1356,  1360,  1365,  1369,  1374,  1387,
    1401,  1420,  1425,  1430,  1434,  1444,  1449,  1453,  1458
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_IS", "TK_NUM", "TK_REAL", "TK_BOOL",
  "TK_CHAR", "TK_WHILE", "TK_FOR", "TK_DO", "TK_BREAK", "TK_CONTINUE",
  "TK_MAIN", "TK_ID", "TK_TIPO_INT", "TK_TIPO_REAL", "TK_TIPO_BOOL",
  "TK_TIPO_CHAR", "TK_STRING", "TK_TIPO_STRING", "TK_FIM", "TK_ERROR",
  "TK_INPUT", "TK_OUTPUT", "TK_SWITCH", "TK_CASE", "TK_DEFAULT", "TK_IF",
  "TK_ELSE", "TK_FUNCTION", "TK_RETURN", "TK_PROCEDURE", "'+'", "'-'",
  "'*'", "'/'", "'('", "')'", "'='", "';'", "'{'", "'}'", "'!'", "'<'",
  "'>'", "':'", "','", "$accept", "S", "EMPLACO", "EMPSWITCH", "LACO",
  "INTLACO", "MAIN", "BLOCO", "DESCONTEXTO", "EMPCONTEXTO", "CMDSGLOBAL",
  "COMANDOS", "OPATRIB", "ES", "ATRIBUICAO", "DECLARACAO", "COMANDO",
  "INPUT", "OUTPUT", "OUTTERM", "TIPO", "EL", "OPNDOLOGIC", "OPLOGIC", "E",
  "ID", "CONDICIONAL", "CASE_COMP", "CASES", "CASE", "DEFAULT",
  "CONDMODIF", "FUNCTION", "PROCEDURE", "RETURN", "IT", "ARGS", "PARGS",
  "VIRG", "PAR", "PPAR", "CALL_FUNC", "CALL_PROC", "MORE_PARS", "ONE_PAR",
  "PARS", "P_MORE_ARGS", YY_NULLPTR
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
     285,   286,   287,    43,    45,    42,    47,    40,    41,    61,
      59,   123,   125,    33,    60,    62,    58,    44
};
# endif

#define YYPACT_NINF -138

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-138)))

#define YYTABLE_NINF -71

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     228,   -23,   -15,  -138,   -12,     4,  -138,    28,  -138,  -138,
    -138,  -138,    19,    23,    30,    31,    40,   106,    40,    72,
    -138,  -138,  -138,  -138,  -138,  -138,   228,  -138,  -138,    40,
     -18,  -138,  -138,  -138,  -138,  -138,  -138,    82,    57,  -138,
    -138,  -138,    36,    40,    -1,    40,    82,    47,  -138,  -138,
    -138,  -138,   269,   142,    50,    69,  -138,  -138,    52,    40,
      20,    67,  -138,  -138,    74,    54,   142,   237,   107,    77,
      84,    89,  -138,    90,  -138,    91,    97,   269,  -138,    99,
      57,    57,    57,    57,  -138,   116,    45,    98,  -138,    93,
     103,    25,    64,   105,  -138,  -138,    83,   115,   108,  -138,
    -138,   113,   117,   118,   120,    82,    57,   109,   171,  -138,
     104,   114,  -138,  -138,    40,    93,   146,     1,     1,  -138,
    -138,  -138,    40,   123,   269,   116,  -138,    40,  -138,   124,
      40,  -138,  -138,  -138,  -138,  -138,  -138,    54,   229,    82,
    -138,   126,   200,  -138,  -138,  -138,   125,   136,  -138,   131,
     269,  -138,  -138,   168,   116,  -138,    93,  -138,    93,  -138,
      82,   134,  -138,  -138,    21,   145,  -138,   159,    93,  -138,
    -138,  -138,    40,   154,   153,   157,  -138,    16,   152,   158,
      21,  -138,   167,  -138,   269,  -138,  -138,    93,   165,    57,
    -138,  -138,  -138,  -138,   160,   171,  -138,  -138,    82,   204,
    -138,  -138,   244,   200,  -138,   181,  -138,   183,  -138,  -138,
    -138,  -138,   136,  -138,  -138,  -138
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      16,     0,     0,     3,     0,     0,    70,    45,    46,    47,
      48,    49,     0,     0,     0,     0,     0,    90,     0,     0,
      32,    33,    15,     2,    30,    29,    16,    34,    35,     0,
      25,    31,    36,    39,    38,    37,    40,     0,     0,    13,
       8,     9,     0,     0,     0,     0,     0,     0,    66,    67,
      68,    89,     0,    88,     0,     0,     1,    14,    25,   106,
       0,     0,    54,    53,    55,    51,    52,     0,     0,     0,
       0,     0,    43,     0,    44,     0,    55,     0,    45,     0,
       0,     0,     0,     0,    87,    94,     0,     0,   104,   108,
       0,    54,    53,    68,    26,    24,    55,    52,     0,    27,
       3,     0,     0,    61,    60,     0,     0,     0,    13,    13,
       0,     0,     4,    13,     0,    92,     0,    64,    65,    63,
      62,    97,     0,     0,     0,    94,    28,     0,   105,     0,
       0,    13,    56,    57,    58,    59,    55,    50,     0,     0,
      18,     0,    19,    10,    41,    42,     0,    83,    98,     0,
       0,    69,    99,     0,    94,    93,   108,   101,   103,     5,
       0,    55,    12,    17,    78,    13,    71,     0,    92,    13,
      95,   107,     0,     0,    55,     0,    11,     0,     0,     0,
      78,    77,     0,    82,     0,    91,    86,   103,     0,     0,
       7,    73,    74,    75,     0,    13,    72,    76,     0,     0,
     102,   100,    25,    19,    80,    55,    13,     0,    79,    13,
      84,     3,    83,    13,    81,     6
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -138,  -138,   -94,  -138,  -138,  -138,  -138,   -39,  -138,  -138,
     196,  -137,   -55,  -138,  -138,  -138,    17,  -138,  -138,  -138,
     -45,   -36,   121,  -138,    -5,   -14,  -138,  -138,    49,  -138,
    -138,    15,  -138,  -138,  -138,  -138,    65,  -116,   -73,    85,
     110,  -138,  -138,    60,  -119,  -138,    94
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    19,    39,   146,    20,    21,    22,   140,   176,    69,
      23,   141,    61,    95,    24,    25,   142,    27,    28,    73,
      29,    96,    65,   105,    66,    30,    31,   194,   179,   180,
     181,   166,    32,    33,    34,    54,   149,   123,   124,   115,
     125,    35,    36,   173,    89,    90,   128
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      68,    64,    47,    87,    55,   163,   131,    79,   156,   155,
      76,   158,    53,     6,    37,    58,   127,    26,    72,    59,
     191,    60,    38,   192,    48,    49,    91,    92,    40,    71,
      74,    75,   114,    67,    93,   193,    82,    83,   170,    94,
     122,    42,   150,    26,    41,    88,    98,   177,   178,    48,
      49,    91,    92,   187,     6,    97,    43,    52,   204,    50,
      44,    48,    49,   -21,    94,   -21,   208,    45,    46,   136,
     143,    50,    56,    70,   147,   117,   118,   119,   120,   122,
     122,    97,    52,   127,    77,   172,    48,    49,    62,    63,
      84,    86,   159,   101,    52,   150,    50,   102,   103,   104,
     148,   138,   -20,   161,   -20,   114,    85,    99,   152,   122,
      48,    49,   100,    88,   172,   107,    88,   213,   108,    52,
      50,   -23,   109,   -23,   174,    51,   183,   110,   111,   112,
     186,    78,     8,     9,    10,   113,    11,   116,   126,   199,
     121,   129,   -70,    52,   144,   130,   139,   207,    80,    81,
      82,    83,   132,   -22,   145,   -22,   133,   134,    88,   135,
     151,   153,   205,   121,   157,   165,   164,   210,   162,   167,
     212,   169,   175,   182,   215,    80,    81,    82,    83,     1,
       2,     3,     4,     5,   202,     6,    78,     8,     9,    10,
     184,    11,   188,   189,    12,    13,    14,   190,   195,    15,
     196,    16,    17,    18,   198,   201,   203,   206,     1,     2,
       3,     4,     5,   -19,     6,    78,     8,     9,    10,   209,
      11,   211,    57,    12,    13,    14,   137,   214,    15,   197,
      16,    17,    18,   185,   154,   168,     1,     2,     3,     4,
       5,   -13,     6,     7,     8,     9,    10,   200,    11,     0,
     171,    12,    13,    14,     0,     0,    15,     0,    16,    17,
      18,     0,    80,    81,    82,    83,     0,     0,     0,   160,
      80,    81,    82,    83,     0,     0,   106,    80,    81,    82,
      83,     0,     0,    86,    78,     8,     9,    10,     0,    11
};

static const yytype_int16 yycheck[] =
{
      39,    37,    16,    58,    18,   142,   100,    52,   127,   125,
      46,   130,    17,    14,    37,    29,    89,     0,    19,    37,
       4,    39,    37,     7,     4,     5,     6,     7,    40,    43,
      44,    45,    77,    38,    14,    19,    35,    36,   154,    19,
      85,    13,   115,    26,    40,    59,    60,    26,    27,     4,
       5,     6,     7,   172,    14,    60,    37,    37,   195,    14,
      37,     4,     5,    38,    19,    40,   203,    37,    37,   105,
     109,    14,     0,    37,   113,    80,    81,    82,    83,   124,
     125,    86,    37,   156,    37,   158,     4,     5,     6,     7,
      40,    39,   131,    39,    37,   168,    14,    43,    44,    45,
     114,   106,    38,   139,    40,   150,    37,    40,   122,   154,
       4,     5,    38,   127,   187,     8,   130,   211,    41,    37,
      14,    38,    38,    40,   160,    19,   165,    38,    38,    38,
     169,    15,    16,    17,    18,    38,    20,    38,    40,   184,
      47,    38,    37,    37,    40,    37,    37,   202,    33,    34,
      35,    36,    39,    38,    40,    40,    39,    39,   172,    39,
      14,    38,   198,    47,    40,    29,    41,   206,    42,    38,
     209,     3,    38,    28,   213,    33,    34,    35,    36,     8,
       9,    10,    11,    12,   189,    14,    15,    16,    17,    18,
      31,    20,    38,    40,    23,    24,    25,    40,    46,    28,
      42,    30,    31,    32,    37,    40,    46,     3,     8,     9,
      10,    11,    12,    42,    14,    15,    16,    17,    18,    38,
      20,    38,    26,    23,    24,    25,   105,   212,    28,   180,
      30,    31,    32,   168,   124,   150,     8,     9,    10,    11,
      12,    41,    14,    15,    16,    17,    18,   187,    20,    -1,
     156,    23,    24,    25,    -1,    -1,    28,    -1,    30,    31,
      32,    -1,    33,    34,    35,    36,    -1,    -1,    -1,    40,
      33,    34,    35,    36,    -1,    -1,    39,    33,    34,    35,
      36,    -1,    -1,    39,    15,    16,    17,    18,    -1,    20
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,     9,    10,    11,    12,    14,    15,    16,    17,
      18,    20,    23,    24,    25,    28,    30,    31,    32,    49,
      52,    53,    54,    58,    62,    63,    64,    65,    66,    68,
      73,    74,    80,    81,    82,    89,    90,    37,    37,    50,
      40,    40,    13,    37,    37,    37,    37,    73,     4,     5,
      14,    19,    37,    72,    83,    73,     0,    58,    73,    37,
      39,    60,     6,     7,    69,    70,    72,    72,    55,    57,
      37,    73,    19,    67,    73,    73,    69,    37,    15,    68,
      33,    34,    35,    36,    40,    37,    39,    60,    73,    92,
      93,     6,     7,    14,    19,    61,    69,    72,    73,    40,
      38,    39,    43,    44,    45,    71,    39,     8,    41,    38,
      38,    38,    38,    38,    68,    87,    38,    72,    72,    72,
      72,    47,    68,    85,    86,    88,    40,    86,    94,    38,
      37,    50,    39,    39,    39,    39,    69,    70,    72,    37,
      55,    59,    64,    55,    40,    40,    51,    55,    73,    84,
      86,    14,    73,    38,    88,    85,    92,    40,    92,    55,
      40,    69,    42,    59,    41,    29,    79,    38,    87,     3,
      85,    94,    86,    91,    69,    38,    56,    26,    27,    76,
      77,    78,    28,    55,    31,    84,    55,    92,    38,    40,
      40,     4,     7,    19,    75,    46,    42,    76,    37,    68,
      91,    40,    72,    46,    59,    69,     3,    60,    59,    38,
      55,    38,    55,    50,    79,    55
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    50,    51,    52,    52,    52,    53,    53,
      54,    55,    56,    57,    58,    58,    58,    59,    59,    59,
      60,    60,    60,    60,    60,    60,    61,    62,    63,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    65,    66,    67,    67,    68,    68,    68,    68,    68,
      69,    69,    70,    70,    70,    70,    71,    71,    71,    71,
      71,    71,    72,    72,    72,    72,    72,    72,    72,    72,
      73,    74,    74,    75,    75,    75,    76,    76,    76,    77,
      78,    79,    79,    79,    80,    80,    81,    82,    83,    83,
      83,    84,    84,    85,    85,    85,    85,    86,    87,    88,
      89,    90,    91,    91,    92,    93,    93,    94,    94
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     0,     6,    13,     8,     2,     2,
       5,     5,     0,     0,     2,     1,     0,     2,     1,     0,
       2,     2,     2,     2,     2,     0,     1,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     5,     5,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     2,     2,     2,     2,
       1,     1,     3,     3,     3,     3,     1,     1,     1,     4,
       1,     6,     8,     1,     1,     1,     2,     1,     0,     4,
       3,     7,     2,     0,    10,     0,     7,     3,     1,     1,
       0,     3,     0,     2,     0,     3,     0,     1,     2,     2,
       8,     5,     3,     0,     1,     2,     0,     3,     0
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
#line 354 "sintatica.y" /* yacc.c:1646  */
    {
				desempilhaContexto();

				cout << "#define TRUE 1\n#define FALSE 0\n#define BOOL int\n\n" 
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
#line 1795 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 394 "sintatica.y" /* yacc.c:1646  */
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
#line 1814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 411 "sintatica.y" /* yacc.c:1646  */
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
#line 1832 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 427 "sintatica.y" /* yacc.c:1646  */
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
#line 1859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 450 "sintatica.y" /* yacc.c:1646  */
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
#line 1881 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 467 "sintatica.y" /* yacc.c:1646  */
    {

				//Pega as labels na pilha do Laço atual
				laco lacoAtual = pilhaLaco.back();

				(yyval).traducao = "\t" + lacoAtual.labelinicio + "\n"+ (yyvsp[-5]).traducao + "\tif(" + (yyvsp[-2]).label + ") goto " + lacoAtual.labelinicio + "\n\t" + lacoAtual.labelfim + "\n";

				//Desempilha laço
				pilhaLaco.pop_back();

			}
#line 1897 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 481 "sintatica.y" /* yacc.c:1646  */
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
#line 1917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 497 "sintatica.y" /* yacc.c:1646  */
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
#line 1955 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 533 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\nint main (void)\n{\n" + (yyvsp[0]).traducao;
			}
#line 1963 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 539 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao;

			}
#line 1972 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 546 "sintatica.y" /* yacc.c:1646  */
    {

				desempilhaContexto();
			}
#line 1981 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 553 "sintatica.y" /* yacc.c:1646  */
    {

				
				empilhaContexto();
			}
#line 1991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 560 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 1999 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 564 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 568 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 2015 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 574 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
				
			}
#line 2024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 579 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 583 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 2040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 591 "sintatica.y" /* yacc.c:1646  */
    {

				(yyval).label = (yyvsp[0]).label;
				(yyval).tipo = "char";
			}
#line 2050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 597 "sintatica.y" /* yacc.c:1646  */
    {

				//$$.label = $2.label;
			if ((yyvsp[0]).label.compare("true"))
				(yyval).label = "FALSE";
			else
				(yyval).label = "TRUE";

				(yyval).tipo = "BOOL";
			}
#line 2065 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 608 "sintatica.y" /* yacc.c:1646  */
    {
				
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2076 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 616 "sintatica.y" /* yacc.c:1646  */
    {
				
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2087 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 623 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 629 "sintatica.y" /* yacc.c:1646  */
    {
			
				(yyval).tipo = "null";
			}
#line 2106 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 637 "sintatica.y" /* yacc.c:1646  */
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
#line 2130 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 660 "sintatica.y" /* yacc.c:1646  */
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
#line 2154 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 683 "sintatica.y" /* yacc.c:1646  */
    {

				//Criação de variável temporária
				string nomeTemp = genTemp();
				
				
				//Tenta inserir variável
				insereVariavel((yyvsp[-2]).label, (yyvsp[-3]).tipo , nomeTemp);

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
#line 2180 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 708 "sintatica.y" /* yacc.c:1646  */
    {
				//Transfere para tradução de comando a tradução de DECLARACAO
				(yyval).traducao =  (yyvsp[0]).traducao;
			}
#line 2189 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 713 "sintatica.y" /* yacc.c:1646  */
    {
				//Transfere para tradução de comando a tradução de ATRIBUICAO
				(yyval).traducao =  (yyvsp[0]).traducao;
			}
#line 2198 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 718 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2206 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 722 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 726 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2222 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 730 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2230 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 734 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2238 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 738 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2246 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 742 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2254 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 746 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2262 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 750 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 754 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2278 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 760 "sintatica.y" /* yacc.c:1646  */
    {
				//Busca na tabela
				variavel var;

				//Tenta buscar a variável
				buscaVariavel((yyvsp[-2]).label, var);

				(yyval).traducao = "\tcin >> " + var.identificacao + ";\n";
			}
#line 2292 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 772 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao 
				+ "\tcout >> " + (yyvsp[-2]).label + ";\n";	
			}
#line 2301 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 778 "sintatica.y" /* yacc.c:1646  */
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
#line 2327 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 800 "sintatica.y" /* yacc.c:1646  */
    {
				//Busca na tabela
				variavel var;

				//Tenta buscar a variável
				buscaVariavel((yyvsp[0]).label, var);

				(yyval).label = var.identificacao;
			}
#line 2341 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 812 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "int";

			}
#line 2350 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 817 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "real";
			}
#line 2358 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 821 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "BOOL";
			}
#line 2366 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 825 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
			}
#line 2374 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 829 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char*";

			}
#line 2383 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 837 "sintatica.y" /* yacc.c:1646  */
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
#line 2472 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 922 "sintatica.y" /* yacc.c:1646  */
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
#line 2493 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 941 "sintatica.y" /* yacc.c:1646  */
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
#line 2515 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 959 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2524 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 964 "sintatica.y" /* yacc.c:1646  */
    {

			if ((yyvsp[0]).label.compare("true"))
				(yyval).label = "FALSE";
			else
				(yyval).label = "TRUE";

				(yyval).tipo = "BOOL";
			}
#line 2538 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 974 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = (yyvsp[0]).label;
			}
#line 2548 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 982 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " == ";
			}
#line 2556 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 986 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " != ";
			}
#line 2564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 990 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " <= ";
			}
#line 2572 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 994 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " >= ";
			}
#line 2580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 998 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " > ";
			}
#line 2588 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1002 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " < ";
			}
#line 2596 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1008 "sintatica.y" /* yacc.c:1646  */
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

				//Passa para E a tradução
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t" + nometemp + " = " + (yyvsp[-2]).label + " / " + (yyvsp[0]).label + ";\n";

				//Passa para E seu valor de temporária
				(yyval).label = nometemp;
			}
#line 2623 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1031 "sintatica.y" /* yacc.c:1646  */
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

				//Passa para E a tradução
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t" + nometemp + " = " + (yyvsp[-2]).label + " * " + (yyvsp[0]).label + ";\n";

				//Passa para E seu valor de temporária
				(yyval).label = nometemp;

			}
#line 2651 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1055 "sintatica.y" /* yacc.c:1646  */
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

				//Passa para E a tradução
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
				+ "\t" + nometemp + " = " + (yyvsp[-2]).label + " + " + (yyvsp[0]).label + ";\n";

				//Passa para E seu valor de temporária
				(yyval).label = nometemp;

			}
#line 2679 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1079 "sintatica.y" /* yacc.c:1646  */
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

				//Passa para E a tradução
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[0]).traducao 
				//+ "\t" + $1.tipo + " " + nometemp + ";\n"
				+ "\t"  + nometemp + " = " + (yyvsp[-2]).label + " - " + (yyvsp[0]).label + ";\n";

				//Passa para E seu valor de temporária
				(yyval).label = nometemp;

			}
#line 2707 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1103 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "int";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2717 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1109 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "real";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1117 "sintatica.y" /* yacc.c:1646  */
    {

				//Busca na tabela
				variavel var;
				

				//Tenta buscar a variável
				
				buscaVariavel((yyvsp[0]).label, var);

				//Passa o tipo e o nome para E
				(yyval).tipo = var.tipo;
				(yyval).label = var.identificacao; 
			}
#line 2746 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1132 "sintatica.y" /* yacc.c:1646  */
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
#line 2771 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1154 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa seu nome literal para ID

				(yyval).label = (yyvsp[0]).label;
			}
#line 2781 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1161 "sintatica.y" /* yacc.c:1646  */
    {
				
				string nometemp = genTemp();
				insereVariavel(genNomeGen(), "BOOL", nometemp);
				string label = labelStackEnd.top();
				labelStackEnd.pop();

				(yyval).traducao = (yyvsp[-3]).traducao + "\t" + nometemp + " = !" + 
				(yyvsp[-3]).label + ";\n" + "\tif" + "(" + nometemp + ")" + "\n\tgoto " + label + ";\n" + 
				(yyvsp[-1]).traducao + "\tgoto " + (yyvsp[0]).tipo + ";\n" + (yyvsp[0]).traducao +"\t"+ (yyvsp[0]).tipo + ":\n";
			}
#line 2797 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1173 "sintatica.y" /* yacc.c:1646  */
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
#line 2822 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1195 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "int";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2832 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1201 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2841 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1206 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "string";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2850 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1212 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 2858 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1216 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2866 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1220 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 2874 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1226 "sintatica.y" /* yacc.c:1646  */
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
#line 2908 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1257 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2916 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1262 "sintatica.y" /* yacc.c:1646  */
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
#line 2938 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1280 "sintatica.y" /* yacc.c:1646  */
    {
				
				string label = gerarLabelEndif();
				string labelelse = gerarLabel(0);
				labelStackEnd.push(label);
				labelStackEnd.push(labelelse);
				(yyval).tipo = label;
				(yyval).traducao = "\t" + labelelse + ":\n" + (yyvsp[0]).traducao;

			}
#line 2953 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1291 "sintatica.y" /* yacc.c:1646  */
    {
				string label = gerarLabelEndif();
				labelStackEnd.push(label);
				(yyval).tipo = label;
			}
#line 2963 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1298 "sintatica.y" /* yacc.c:1646  */
    {
				_function func;
				
				func.functionContext = funcContext;
				func.label =  (yyvsp[-8]).label;
				funcContext.erase(funcContext.begin(), funcContext.end());
				functions.push_back(func);
				funcQtt++;
				(yyval).traducao = (yyvsp[-2]).tipo + " " + (yyvsp[-8]).label  + 
				"(" + (yyvsp[-6]).traducao + (yyvsp[-5]).traducao + ")\n{\n" + (yyvsp[0]).traducao + "\n}\n";
			}
#line 2979 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1312 "sintatica.y" /* yacc.c:1646  */
    {
				_procedure proc;
				proc.procedureContext = procContext;
				proc.label = (yyvsp[-5]).label;
				procContext.erase(procContext.begin(), procContext.end());
				procedures.push_back(proc);

				(yyval).traducao = "void " + (yyvsp[-5]).label + "(" + (yyvsp[-3]).traducao + ")" +
				"\n{\n" + (yyvsp[0]).traducao + "\n}\n";
			}
#line 2994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1324 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\treturn " + (yyvsp[-1]).traducao + ";\n";
			}
#line 3002 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1329 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).label;
			}
#line 3010 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1333 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1337 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3026 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1342 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3034 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1346 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1352 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1356 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3058 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1361 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1365 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3074 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1370 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = ", ";
			}
#line 3082 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1375 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp();
				var.tipo = (yyvsp[-1]).tipo;
				var.nome = (yyvsp[0]).label;//genNomeGen();
				funcContext.push_back(var);

				(yyval).traducao = (yyvsp[-1]).tipo + " " + var.identificacao;
			}
#line 3097 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1388 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp();
				var.tipo = (yyvsp[-1]).tipo;
				var.nome = (yyvsp[0]).label;//genNomeGen();
				procContext.push_back(var);

				(yyval).traducao = (yyvsp[-1]).tipo + " " + var.identificacao;
			}
#line 3112 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1402 "sintatica.y" /* yacc.c:1646  */
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
#line 3134 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1421 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\t" + (yyvsp[-4]).label + "(" + (yyvsp[-2]).traducao + ")"+ ";\n";
			}
#line 3142 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1426 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3150 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1430 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1435 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				buscaVariavel((yyvsp[0]).label, var);

				(yyval).traducao = var.identificacao;
				(yyval).label = (yyvsp[0]).label;
			}
#line 3171 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1445 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3179 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1449 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3187 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1454 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3195 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1458 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3203 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3207 "y.tab.c" /* yacc.c:1646  */
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
#line 1463 "sintatica.y" /* yacc.c:1906  */


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
