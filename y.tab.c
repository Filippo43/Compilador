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

#line 519 "y.tab.c" /* yacc.c:358  */

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
#define YYFINAL  57
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   351

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  248

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
       2,     2,     2,    45,     2,     2,     2,     2,     2,     2,
      37,    38,    35,    33,    49,    34,     2,    36,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    48,    40,
      46,    39,    47,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    43,     2,    44,     2,     2,     2,     2,     2,     2,
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
       0,   354,   354,   395,   412,   427,   450,   468,   481,   497,
     533,   539,   547,   554,   560,   564,   569,   574,   579,   584,
     591,   597,   608,   616,   623,   629,   644,   659,   666,   689,
     709,   739,   772,   797,   826,   863,   868,   873,   877,   881,
     885,   889,   893,   897,   901,   905,   909,   915,   927,   933,
     955,   967,   972,   976,   980,   984,   992,  1077,  1096,  1114,
    1119,  1129,  1137,  1141,  1145,  1149,  1153,  1157,  1163,  1193,
    1223,  1266,  1286,  1317,  1323,  1329,  1349,  1370,  1385,  1408,
    1415,  1427,  1449,  1455,  1460,  1466,  1470,  1475,  1480,  1511,
    1516,  1534,  1546,  1552,  1565,  1577,  1582,  1586,  1591,  1595,
    1600,  1605,  1610,  1614,  1619,  1623,  1628,  1641,  1655,  1674,
    1679,  1684,  1688,  1698,  1703,  1707,  1712
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
  "'*'", "'/'", "'('", "')'", "'='", "';'", "'{'", "'}'", "'['", "']'",
  "'!'", "'<'", "'>'", "':'", "','", "$accept", "S", "EMPLACO",
  "EMPSWITCH", "LACO", "INTLACO", "MAIN", "BLOCO", "DESCONTEXTO",
  "EMPCONTEXTO", "CMDSGLOBAL", "COMANDOS", "OPATRIB", "ES", "ATRIBUICAO",
  "DECLARACAO", "COMANDO", "INPUT", "OUTPUT", "OUTTERM", "TIPO", "EL",
  "OPNDOLOGIC", "OPLOGIC", "E", "ID", "CONDICIONAL", "CASE_COMP", "CASES",
  "CASE", "DEFAULT", "CONDMODIF", "FUNCTION", "PROCEDURE", "RETURN", "IT",
  "ARGS", "PARGS", "VIRG", "PAR", "PPAR", "CALL_FUNC", "CALL_PROC",
  "MORE_PARS", "ONE_PAR", "PARS", "P_MORE_ARGS", YY_NULLPTR
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
      59,   123,   125,    91,    93,    33,    60,    62,    58,    44
};
# endif

#define YYPACT_NINF -136

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-136)))

#define YYTABLE_NINF -80

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     270,   -24,   -14,  -136,    -8,     7,  -136,    41,  -136,  -136,
    -136,  -136,    75,    80,    97,   108,    72,    96,    72,   129,
    -136,  -136,  -136,  -136,  -136,  -136,   270,  -136,  -136,    72,
      -9,  -136,  -136,  -136,  -136,  -136,  -136,    32,   104,  -136,
    -136,  -136,   117,    72,    49,    72,    32,   120,  -136,  -136,
     -10,  -136,   327,   315,    93,   118,   133,  -136,  -136,    40,
      72,    88,   104,   134,  -136,  -136,  -136,   131,    11,   315,
     295,   165,   136,   141,   154,  -136,   155,  -136,   156,   158,
     327,  -136,   160,   104,   104,   104,   104,   104,  -136,     4,
      88,   104,   149,  -136,   153,   162,   -26,    26,    92,  -136,
      99,   115,    22,   128,  -136,  -136,   166,   167,   173,   174,
      32,   104,   177,   213,  -136,   164,   194,  -136,  -136,    72,
     153,   221,    70,    70,  -136,  -136,   132,  -136,    72,   201,
     327,     4,   197,   147,  -136,    72,  -136,   202,    72,   104,
      45,  -136,  -136,  -136,  -136,  -136,  -136,    11,   287,    32,
    -136,   204,   242,  -136,  -136,  -136,   206,   219,  -136,   211,
     327,  -136,   218,  -136,   260,     4,  -136,    56,   153,  -136,
     153,   151,   104,   224,  -136,    32,   230,  -136,  -136,    94,
     241,  -136,   240,   153,   104,  -136,  -136,  -136,   104,  -136,
      72,   237,    84,   175,  -136,   236,   249,  -136,    48,   229,
     250,    94,  -136,   254,  -136,   327,  -136,   182,  -136,   271,
     153,   256,   104,   258,   104,  -136,  -136,  -136,  -136,   251,
     213,  -136,  -136,    32,   300,  -136,   272,  -136,  -136,   275,
     273,   302,   242,  -136,   276,  -136,  -136,   106,  -136,   278,
    -136,  -136,  -136,  -136,   219,  -136,  -136,  -136
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      16,     0,     0,     3,     0,     0,    79,    51,    52,    53,
      54,    55,     0,     0,     0,     0,     0,    98,     0,     0,
      38,    39,    15,     2,    36,    35,    16,    40,    41,     0,
      27,    37,    42,    45,    44,    43,    46,     0,     0,    13,
       8,     9,     0,     0,     0,     0,     0,     0,    73,    74,
      77,    71,     0,    96,     0,     0,     0,     1,    14,    27,
     114,     0,     0,     0,    60,    59,    71,    61,    57,    58,
       0,     0,     0,     0,     0,    49,     0,    50,     0,    61,
       0,    51,     0,     0,     0,     0,     0,     0,    95,   102,
       0,     0,     0,   112,   116,     0,    60,    59,    71,    24,
      61,    58,     0,     0,    29,     3,     0,     0,    67,    66,
       0,     0,     0,    13,    13,     0,     0,     4,    13,     0,
     100,     0,    70,    72,    69,    68,     0,   105,     0,     0,
       0,   102,     0,     0,    32,     0,   113,     0,     0,     0,
      27,    13,    62,    63,    64,    65,    61,    56,     0,     0,
      18,     0,    19,    10,    47,    48,     0,    92,   106,     0,
       0,    78,    75,   107,     0,   102,   101,     0,   116,   109,
     111,     0,     0,     0,     5,     0,    61,    12,    17,    87,
      13,    80,     0,   100,     0,    13,   103,    33,     0,   115,
       0,     0,    75,     0,    30,    61,     0,    11,     0,     0,
       0,    87,    86,     0,    91,     0,    99,     0,    94,     0,
     111,     0,     0,    27,     0,     7,    82,    83,    84,     0,
      13,    81,    85,     0,     0,    76,     0,   110,   108,     0,
       0,    27,    19,    89,    61,    13,    34,    76,    31,     0,
      88,    13,    93,     3,    92,    13,    90,     6
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -136,  -136,  -100,  -136,  -136,  -136,  -136,   -38,  -136,  -136,
     291,  -135,   -53,  -136,  -136,  -136,     9,  -136,  -136,  -136,
     -49,   -35,   208,  -136,   -13,     0,  -136,  -136,   123,  -136,
    -136,    81,  -136,  -136,  -136,  -136,   143,  -123,   -79,   172,
     203,  -136,  -136,   130,  -128,  -136,   171
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    19,    39,   156,    20,    21,    22,   150,   197,    72,
      23,   151,    63,    99,    24,    25,   152,    27,    28,    76,
      29,   100,    68,   110,    69,    54,    31,   219,   200,   201,
     202,   181,    32,    33,    34,    55,   159,   129,   130,   120,
     131,    35,    36,   191,    94,    95,   136
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      30,    71,    67,    82,    53,   141,    92,   168,   166,    26,
     170,    79,   -21,    37,   -21,   135,    47,   178,    56,    81,
       8,     9,    10,    38,    11,    70,    30,   -79,    60,    59,
      61,   119,    40,   -79,    62,    26,    48,    49,    64,    65,
     128,   160,   186,    74,    77,    78,    50,    41,   101,   103,
     106,    66,   216,   127,    42,   217,   107,   108,   109,   138,
      93,   102,   210,     6,   -20,   139,   -20,   218,    75,    52,
     122,   123,   124,   125,   126,   146,   153,   101,   133,    90,
     157,   128,   128,    91,    90,   233,     6,   173,   172,   135,
     132,   190,    48,    49,    96,    97,   187,   240,   148,   188,
      48,    49,    50,   174,   160,    85,    86,    98,    48,    49,
      50,   119,    43,    30,   176,    51,   128,    44,    50,   158,
     198,   199,   -25,    66,   -25,    52,   171,   212,   163,    57,
     -28,   190,   -28,    52,    45,    93,    87,   -23,    93,   -23,
     195,    52,   204,   245,   -26,    46,   -26,   208,    83,    84,
      85,    86,    30,   -22,    73,   -22,   224,    80,    88,   193,
     230,    83,    84,    85,    86,    83,    84,    85,    86,   105,
      89,   207,   140,   112,   104,   209,   162,   113,   239,   114,
      83,    84,    85,    86,    83,    84,    85,    86,   234,   134,
      93,   167,   115,   116,   117,   192,   118,   242,   121,   229,
     137,   231,   127,   244,   154,   142,   143,   247,    83,    84,
      85,    86,   144,   145,   149,    83,    84,    85,    86,   213,
      30,     1,     2,     3,     4,     5,   225,     6,    81,     8,
       9,    10,    30,    11,   155,   161,    12,    13,    14,   164,
     139,    15,   169,    16,    17,    18,   177,   179,   180,   182,
       1,     2,     3,     4,     5,   -19,     6,    81,     8,     9,
      10,   184,    11,   185,   194,    12,    13,    14,   196,   203,
      15,   205,    16,    17,    18,   211,   214,   220,     1,     2,
       3,     4,     5,   -13,     6,     7,     8,     9,    10,   215,
      11,   223,   221,    12,    13,    14,   228,    90,    15,   232,
      16,    17,    18,   235,    83,    84,    85,    86,    83,    84,
      85,    86,   236,   238,   241,   226,   243,    58,   147,   237,
      83,    84,    85,    86,   222,   246,   206,   175,    83,    84,
      85,    86,   183,   165,   111,    83,    84,    85,    86,   189,
     227,    90,    81,     8,     9,    10,     0,    11,    83,    84,
      85,    86
};

static const yytype_int16 yycheck[] =
{
       0,    39,    37,    52,    17,   105,    59,   135,   131,     0,
     138,    46,    38,    37,    40,    94,    16,   152,    18,    15,
      16,    17,    18,    37,    20,    38,    26,    37,    37,    29,
      39,    80,    40,    43,    43,    26,     4,     5,     6,     7,
      89,   120,   165,    43,    44,    45,    14,    40,    61,    62,
      39,    19,     4,    49,    13,     7,    45,    46,    47,    37,
      60,    61,   190,    14,    38,    43,    40,    19,    19,    37,
      83,    84,    85,    86,    87,   110,   114,    90,    91,    39,
     118,   130,   131,    43,    39,   220,    14,   140,    43,   168,
      90,   170,     4,     5,     6,     7,    40,   232,   111,    43,
       4,     5,    14,   141,   183,    35,    36,    19,     4,     5,
      14,   160,    37,   113,   149,    19,   165,    37,    14,   119,
      26,    27,    38,    19,    40,    37,   139,    43,   128,     0,
      38,   210,    40,    37,    37,   135,    43,    38,   138,    40,
     175,    37,   180,   243,    38,    37,    40,   185,    33,    34,
      35,    36,   152,    38,    37,    40,   205,    37,    40,   172,
     213,    33,    34,    35,    36,    33,    34,    35,    36,    38,
      37,   184,    44,     8,    40,   188,    44,    41,   231,    38,
      33,    34,    35,    36,    33,    34,    35,    36,   223,    40,
     190,    44,    38,    38,    38,    44,    38,   235,    38,   212,
      38,   214,    49,   241,    40,    39,    39,   245,    33,    34,
      35,    36,    39,    39,    37,    33,    34,    35,    36,    44,
     220,     8,     9,    10,    11,    12,    44,    14,    15,    16,
      17,    18,   232,    20,    40,    14,    23,    24,    25,    38,
      43,    28,    40,    30,    31,    32,    42,    41,    29,    38,
       8,     9,    10,    11,    12,    42,    14,    15,    16,    17,
      18,    43,    20,     3,    40,    23,    24,    25,    38,    28,
      28,    31,    30,    31,    32,    38,    40,    48,     8,     9,
      10,    11,    12,    41,    14,    15,    16,    17,    18,    40,
      20,    37,    42,    23,    24,    25,    40,    39,    28,    48,
      30,    31,    32,     3,    33,    34,    35,    36,    33,    34,
      35,    36,    40,    40,    38,    44,    38,    26,   110,    44,
      33,    34,    35,    36,   201,   244,   183,    40,    33,    34,
      35,    36,   160,   130,    39,    33,    34,    35,    36,   168,
     210,    39,    15,    16,    17,    18,    -1,    20,    33,    34,
      35,    36
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,     9,    10,    11,    12,    14,    15,    16,    17,
      18,    20,    23,    24,    25,    28,    30,    31,    32,    51,
      54,    55,    56,    60,    64,    65,    66,    67,    68,    70,
      75,    76,    82,    83,    84,    91,    92,    37,    37,    52,
      40,    40,    13,    37,    37,    37,    37,    75,     4,     5,
      14,    19,    37,    74,    75,    85,    75,     0,    60,    75,
      37,    39,    43,    62,     6,     7,    19,    71,    72,    74,
      74,    57,    59,    37,    75,    19,    69,    75,    75,    71,
      37,    15,    70,    33,    34,    35,    36,    43,    40,    37,
      39,    43,    62,    75,    94,    95,     6,     7,    19,    63,
      71,    74,    75,    74,    40,    38,    39,    45,    46,    47,
      73,    39,     8,    41,    38,    38,    38,    38,    38,    70,
      89,    38,    74,    74,    74,    74,    74,    49,    70,    87,
      88,    90,    75,    74,    40,    88,    96,    38,    37,    43,
      44,    52,    39,    39,    39,    39,    71,    72,    74,    37,
      57,    61,    66,    57,    40,    40,    53,    57,    75,    86,
      88,    14,    44,    75,    38,    90,    87,    44,    94,    40,
      94,    74,    43,    62,    57,    40,    71,    42,    61,    41,
      29,    81,    38,    89,    43,     3,    87,    40,    43,    96,
      88,    93,    44,    74,    40,    71,    38,    58,    26,    27,
      78,    79,    80,    28,    57,    31,    86,    74,    57,    74,
      94,    38,    43,    44,    40,    40,     4,     7,    19,    77,
      48,    42,    78,    37,    70,    44,    44,    93,    40,    74,
      62,    74,    48,    61,    71,     3,    40,    44,    40,    62,
      61,    38,    57,    38,    57,    52,    81,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    52,    53,    54,    54,    54,    55,    55,
      56,    57,    58,    59,    60,    60,    60,    61,    61,    61,
      62,    62,    62,    62,    62,    62,    62,    62,    63,    64,
      64,    64,    65,    65,    65,    66,    66,    66,    66,    66,
      66,    66,    66,    66,    66,    66,    66,    67,    68,    69,
      69,    70,    70,    70,    70,    70,    71,    71,    72,    72,
      72,    72,    73,    73,    73,    73,    73,    73,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    75,
      76,    76,    77,    77,    77,    78,    78,    78,    79,    80,
      81,    81,    81,    82,    83,    84,    85,    85,    85,    86,
      86,    87,    87,    87,    87,    88,    89,    90,    91,    92,
      93,    93,    94,    95,    95,    96,    96
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     0,     6,    13,     8,     2,     2,
       5,     5,     0,     0,     2,     1,     0,     2,     1,     0,
       2,     2,     2,     2,     2,     5,     8,     0,     1,     3,
       6,     9,     4,     6,     9,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     5,     5,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     1,     1,     3,     3,
       3,     1,     3,     1,     1,     4,     7,     1,     4,     1,
       6,     8,     1,     1,     1,     2,     1,     0,     4,     3,
       7,     2,     0,    10,     7,     3,     1,     1,     0,     3,
       0,     2,     0,     3,     0,     1,     2,     2,     8,     5,
       3,     0,     1,     2,     0,     3,     0
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
#line 1822 "y.tab.c" /* yacc.c:1646  */
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
#line 1841 "y.tab.c" /* yacc.c:1646  */
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
#line 1859 "y.tab.c" /* yacc.c:1646  */
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
#line 1886 "y.tab.c" /* yacc.c:1646  */
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
#line 1908 "y.tab.c" /* yacc.c:1646  */
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
#line 1924 "y.tab.c" /* yacc.c:1646  */
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
#line 1944 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 498 "sintatica.y" /* yacc.c:1646  */
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
#line 1982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 534 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\nint main (void)\n{\n" + (yyvsp[0]).traducao;
			}
#line 1990 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 540 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao;

			}
#line 1999 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 547 "sintatica.y" /* yacc.c:1646  */
    {

				desempilhaContexto();
			}
#line 2008 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 554 "sintatica.y" /* yacc.c:1646  */
    {

				
				empilhaContexto();
			}
#line 2018 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 561 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 2026 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 565 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2034 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 569 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 2042 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 575 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
				
			}
#line 2051 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 580 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2059 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 584 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 2067 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 592 "sintatica.y" /* yacc.c:1646  */
    {

				(yyval).label = (yyvsp[0]).label;
				(yyval).tipo = "char";
			}
#line 2077 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 598 "sintatica.y" /* yacc.c:1646  */
    {

				//$$.label = $2.label;
			if ((yyvsp[0]).label.compare("true"))
				(yyval).label = "FALSE";
			else
				(yyval).label = "TRUE";

				(yyval).tipo = "BOOL";
			}
#line 2092 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 609 "sintatica.y" /* yacc.c:1646  */
    {
				
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2103 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 617 "sintatica.y" /* yacc.c:1646  */
    {
				
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 624 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).label = (yyvsp[0]).label;
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
			}
#line 2124 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 630 "sintatica.y" /* yacc.c:1646  */
    {

				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-3]).label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				(yyval).traducao = "";
				(yyval).tipo = var.tipo;
				(yyval).label = "*(" + var.identificacao + " + " + (yyvsp[-1]).label + ")";

			}
#line 2143 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 645 "sintatica.y" /* yacc.c:1646  */
    {
				//Variavel ID
				variavel var;
				
				buscaVariavel((yyvsp[-6]).label, var);

				var.tipo = var.tipo.substr(0, var.tipo.size() - 1);

				(yyval).traducao = "";
				(yyval).tipo = var.tipo;
				(yyval).label = "*(" + var.identificacao + " + " + (yyvsp[-4]).label + " * " + var.identificacao + "_d + " + (yyvsp[-1]).label + ")";

			}
#line 2161 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 659 "sintatica.y" /* yacc.c:1646  */
    {
			
				(yyval).tipo = "null";
			}
#line 2170 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 667 "sintatica.y" /* yacc.c:1646  */
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
#line 2194 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 690 "sintatica.y" /* yacc.c:1646  */
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
#line 2218 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 710 "sintatica.y" /* yacc.c:1646  */
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
#line 2252 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 740 "sintatica.y" /* yacc.c:1646  */
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
#line 2286 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 773 "sintatica.y" /* yacc.c:1646  */
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
#line 2315 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 798 "sintatica.y" /* yacc.c:1646  */
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
#line 2348 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 827 "sintatica.y" /* yacc.c:1646  */
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
#line 2386 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 864 "sintatica.y" /* yacc.c:1646  */
    {
				//Transfere para tradução de comando a tradução de DECLARACAO
				(yyval).traducao =  (yyvsp[0]).traducao;
			}
#line 2395 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 869 "sintatica.y" /* yacc.c:1646  */
    {
				//Transfere para tradução de comando a tradução de ATRIBUICAO
				(yyval).traducao =  (yyvsp[0]).traducao;
			}
#line 2404 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 874 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2412 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 878 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2420 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 882 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2428 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 886 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2436 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 890 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2444 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 894 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2452 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 898 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2460 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 902 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2468 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 906 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2476 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 910 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 2484 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 916 "sintatica.y" /* yacc.c:1646  */
    {
				//Busca na tabela
				variavel var;

				//Tenta buscar a variável
				buscaVariavel((yyvsp[-2]).label, var);

				(yyval).traducao = "\tcin >> " + var.identificacao + ";\n";
			}
#line 2498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 928 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao 
				+ "\tcout >> " + (yyvsp[-2]).label + ";\n";	
			}
#line 2507 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 934 "sintatica.y" /* yacc.c:1646  */
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
#line 2533 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 956 "sintatica.y" /* yacc.c:1646  */
    {
				//Busca na tabela
				variavel var;

				//Tenta buscar a variável
				buscaVariavel((yyvsp[0]).label, var);

				(yyval).label = var.identificacao;
			}
#line 2547 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 968 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "int";

			}
#line 2556 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 973 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "REAL";
			}
#line 2564 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 977 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "BOOL";
			}
#line 2572 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 981 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
			}
#line 2580 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 985 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char*";

			}
#line 2589 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 993 "sintatica.y" /* yacc.c:1646  */
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
#line 2678 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1078 "sintatica.y" /* yacc.c:1646  */
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
#line 2699 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1097 "sintatica.y" /* yacc.c:1646  */
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
#line 2721 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1115 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2730 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1120 "sintatica.y" /* yacc.c:1646  */
    {

			if ((yyvsp[0]).label.compare("true"))
				(yyval).label = "FALSE";
			else
				(yyval).label = "TRUE";

				(yyval).tipo = "BOOL";
			}
#line 2744 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1130 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
				(yyval).tipo = (yyvsp[0]).tipo;
				(yyval).label = (yyvsp[0]).label;
			}
#line 2754 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1138 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " == ";
			}
#line 2762 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1142 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " != ";
			}
#line 2770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1146 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " <= ";
			}
#line 2778 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1150 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " >= ";
			}
#line 2786 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1154 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " > ";
			}
#line 2794 "y.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 1158 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = " < ";
			}
#line 2802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1164 "sintatica.y" /* yacc.c:1646  */
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
#line 2836 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1194 "sintatica.y" /* yacc.c:1646  */
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
#line 2870 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1224 "sintatica.y" /* yacc.c:1646  */
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
#line 2917 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1267 "sintatica.y" /* yacc.c:1646  */
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
#line 2941 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1287 "sintatica.y" /* yacc.c:1646  */
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
#line 2976 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1318 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "int";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2986 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1324 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "REAL";
				(yyval).label = (yyvsp[0]).label;
			}
#line 2996 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1330 "sintatica.y" /* yacc.c:1646  */
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
#line 3020 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1350 "sintatica.y" /* yacc.c:1646  */
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
#line 3044 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1371 "sintatica.y" /* yacc.c:1646  */
    {

				//Busca na tabela
				variavel var;
				

				//Tenta buscar a variável
				
				buscaVariavel((yyvsp[0]).label, var);

				//Passa o tipo e o nome para E
				(yyval).tipo = var.tipo;
				(yyval).label = var.identificacao; 
			}
#line 3063 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1386 "sintatica.y" /* yacc.c:1646  */
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
#line 3088 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1409 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa seu nome literal para ID

				(yyval).label = (yyvsp[0]).label;
			}
#line 3098 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1416 "sintatica.y" /* yacc.c:1646  */
    {
				
				string nometemp = genTemp();
				insereVariavel(genNomeGen(), "BOOL", nometemp);
				string label = labelStackEnd.top();
				labelStackEnd.pop();

				(yyval).traducao = (yyvsp[-3]).traducao + "\t" + nometemp + " = !" + 
				(yyvsp[-3]).label + ";\n" + "\tif" + "(" + nometemp + ")" + "\n\tgoto " + label + ";\n" + 
				(yyvsp[-1]).traducao + "\tgoto " + (yyvsp[0]).tipo + ";\n" + (yyvsp[0]).traducao +"\t"+ (yyvsp[0]).tipo + ":\n";
			}
#line 3114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1428 "sintatica.y" /* yacc.c:1646  */
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
#line 3139 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1450 "sintatica.y" /* yacc.c:1646  */
    {
				//Passa para E o tipo e seu valor
				(yyval).tipo = "int";
				(yyval).label = (yyvsp[0]).label;
			}
#line 3149 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1456 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "char";
				(yyval).label = (yyvsp[0]).label;
			}
#line 3158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1461 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).tipo = "string";
				(yyval).label = (yyvsp[0]).label;
			}
#line 3167 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1467 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3175 "y.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 1471 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3183 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1475 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3191 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1481 "sintatica.y" /* yacc.c:1646  */
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
#line 3225 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1512 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3233 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1517 "sintatica.y" /* yacc.c:1646  */
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
#line 3255 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1535 "sintatica.y" /* yacc.c:1646  */
    {
				
				string label = gerarLabelEndif();
				string labelelse = gerarLabel(0);
				labelStackEnd.push(label);
				labelStackEnd.push(labelelse);
				(yyval).tipo = label;
				(yyval).traducao = "\t" + labelelse + ":\n" + (yyvsp[0]).traducao;

			}
#line 3270 "y.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 1546 "sintatica.y" /* yacc.c:1646  */
    {
				string label = gerarLabelEndif();
				labelStackEnd.push(label);
				(yyval).tipo = label;
			}
#line 3280 "y.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 1553 "sintatica.y" /* yacc.c:1646  */
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
#line 3296 "y.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1566 "sintatica.y" /* yacc.c:1646  */
    {
				_procedure proc;
				proc.procedureContext = procContext;
				proc.label = (yyvsp[-5]).label;
				procContext.erase(procContext.begin(), procContext.end());
				procedures.push_back(proc);

				(yyval).traducao = "void " + (yyvsp[-5]).label + "(" + (yyvsp[-3]).traducao + ")" +
				"\n{\n" + (yyvsp[0]).traducao + "\n}\n";
			}
#line 3311 "y.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1578 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\treturn " + (yyvsp[-1]).traducao + ";\n";
			}
#line 3319 "y.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1583 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).label;
			}
#line 3327 "y.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1587 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[0]).traducao;
			}
#line 3335 "y.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1591 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3343 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1596 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3351 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1600 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3359 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1606 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3367 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1610 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3375 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1615 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3383 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1619 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3391 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1624 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = ", ";
			}
#line 3399 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1629 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp();
				var.tipo = (yyvsp[-1]).tipo;
				var.nome = (yyvsp[0]).label;//genNomeGen();
				funcContext.push_back(var);

				(yyval).traducao = (yyvsp[-1]).tipo + " " + var.identificacao;
			}
#line 3414 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1642 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				var.identificacao = genTemp();
				var.tipo = (yyvsp[-1]).tipo;
				var.nome = (yyvsp[0]).label;//genNomeGen();
				procContext.push_back(var);

				(yyval).traducao = (yyvsp[-1]).tipo + " " + var.identificacao;
			}
#line 3429 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1656 "sintatica.y" /* yacc.c:1646  */
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
#line 3451 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1675 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "\t" + (yyvsp[-4]).label + "(" + (yyvsp[-2]).traducao + ")"+ ";\n";
			}
#line 3459 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1680 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3467 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1684 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3475 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1689 "sintatica.y" /* yacc.c:1646  */
    {
				variavel var;

				buscaVariavel((yyvsp[0]).label, var);

				(yyval).traducao = var.identificacao;
				(yyval).label = (yyvsp[0]).label;
			}
#line 3488 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1699 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3496 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1703 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3504 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1708 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = (yyvsp[-2]).traducao + (yyvsp[-1]).traducao + (yyvsp[0]).traducao;
			}
#line 3512 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1712 "sintatica.y" /* yacc.c:1646  */
    {
				(yyval).traducao = "";
			}
#line 3520 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3524 "y.tab.c" /* yacc.c:1646  */
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
#line 1717 "sintatica.y" /* yacc.c:1906  */


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
