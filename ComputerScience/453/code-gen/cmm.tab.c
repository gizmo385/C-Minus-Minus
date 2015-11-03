/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     WHILE = 258,
     FOR = 259,
     INT = 260,
     CHAR = 261,
     VOID = 262,
     IF = 263,
     ELSE = 264,
     EXTERN = 265,
     RETURN = 266,
     COMMA = 267,
     SEMICOLON = 268,
     LEFT_PAREN = 269,
     RIGHT_PAREN = 270,
     LEFT_SQUARE_BRACKET = 271,
     RIGHT_SQUARE_BRACKET = 272,
     LEFT_CURLY_BRACKET = 273,
     RIGHT_CURLY_BRACKET = 274,
     EQ = 275,
     NEQ = 276,
     LTE = 277,
     LT = 278,
     GT = 279,
     GTE = 280,
     NOT = 281,
     AND = 282,
     OR = 283,
     ADD = 284,
     MINUS = 285,
     MUL = 286,
     DIV = 287,
     ASSIGN = 288,
     INTCON = 289,
     CHARCON = 290,
     STRINGCON = 291,
     ID = 292,
     TEXT = 293,
     SPACE = 294,
     WITHOUT_ELSE = 295,
     WITH_ELSE = 296,
     equality_op = 297,
     relop = 298,
     add_sub = 299,
     mul_div = 300,
     UMINUS = 301
   };
#endif
/* Tokens.  */
#define WHILE 258
#define FOR 259
#define INT 260
#define CHAR 261
#define VOID 262
#define IF 263
#define ELSE 264
#define EXTERN 265
#define RETURN 266
#define COMMA 267
#define SEMICOLON 268
#define LEFT_PAREN 269
#define RIGHT_PAREN 270
#define LEFT_SQUARE_BRACKET 271
#define RIGHT_SQUARE_BRACKET 272
#define LEFT_CURLY_BRACKET 273
#define RIGHT_CURLY_BRACKET 274
#define EQ 275
#define NEQ 276
#define LTE 277
#define LT 278
#define GT 279
#define GTE 280
#define NOT 281
#define AND 282
#define OR 283
#define ADD 284
#define MINUS 285
#define MUL 286
#define DIV 287
#define ASSIGN 288
#define INTCON 289
#define CHARCON 290
#define STRINGCON 291
#define ID 292
#define TEXT 293
#define SPACE 294
#define WITHOUT_ELSE 295
#define WITH_ELSE 296
#define equality_op 297
#define relop 298
#define add_sub 299
#define mul_div 300
#define UMINUS 301




/* Copy the first part of user declarations.  */
#line 1 "cmm.y"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "symtab.h"
#include "utils.h"
#include "ast.h"
#include "globals.h"
#include "cmm.tab.h"

extern char *yytext;

// Parse state information
int mylineno;
int mycolno;
bool foundError = false;

// Typing and scoping
bool funcTypeSet = false;
bool declaredExtern = false;
Type currentFunctionReturnType = VOID_TYPE;
Scope *globalScope;
Scope *scope;
Type baseDeclType;

// Helper functions
bool addFunctionDeclarationToScope(Type type, char *identifier, FunctionParameter *parameters);
void resetFunctionType();



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 33 "cmm.y"
{
    Expression *expression;
    Statement *statement;
    Type type;
    FunctionParameter *parameter;
    FunctionDeclaration *functionDeclaration;
    VariableDeclaration *variableDeclaration;
    char *string;
}
/* Line 193 of yacc.c.  */
#line 230 "cmm.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 243 "cmm.tab.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   312

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  79
/* YYNRULES -- Number of states.  */
#define YYNSTATES  173

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    11,    15,    19,    23,    28,
      33,    36,    38,    44,    50,    56,    62,    70,    76,    86,
      90,    93,    99,   103,   105,   108,   111,   114,   117,   121,
     125,   129,   133,   137,   141,   145,   149,   153,   157,   161,
     165,   170,   175,   177,   181,   183,   185,   187,   189,   194,
     201,   203,   208,   210,   214,   216,   218,   220,   222,   224,
     226,   230,   233,   238,   240,   244,   246,   251,   253,   255,
     257,   259,   261,   263,   266,   268,   272,   279,   281,   285
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    49,    48,    -1,    52,    48,    -1,    69,
      -1,    59,    57,    13,    -1,    59,    55,    13,    -1,    58,
      55,    13,    -1,    50,    59,    55,    13,    -1,    50,    58,
      55,    13,    -1,     1,    13,    -1,    10,    -1,    59,    37,
      14,    60,    15,    -1,    58,    37,    14,    60,    15,    -1,
      51,    18,    63,    66,    19,    -1,     8,    14,    54,    15,
      53,    -1,     8,    14,    54,    15,    53,     9,    53,    -1,
       3,    14,    54,    15,    53,    -1,     4,    14,    64,    13,
      65,    13,    64,    15,    53,    -1,    11,    65,    13,    -1,
      67,    13,    -1,    37,    14,    68,    15,    13,    -1,    18,
      66,    19,    -1,    13,    -1,     1,    13,    -1,     1,    19,
      -1,    30,    54,    -1,    26,    54,    -1,    54,    29,    54,
      -1,    54,    30,    54,    -1,    54,    27,    54,    -1,    54,
      28,    54,    -1,    54,    31,    54,    -1,    54,    32,    54,
      -1,    54,    20,    54,    -1,    54,    21,    54,    -1,    54,
      25,    54,    -1,    54,    22,    54,    -1,    54,    24,    54,
      -1,    54,    23,    54,    -1,    37,    14,    68,    15,    -1,
      37,    16,    54,    17,    -1,    37,    -1,    14,    54,    15,
      -1,    34,    -1,    35,    -1,    36,    -1,     1,    -1,    37,
      14,    60,    15,    -1,    55,    12,    37,    14,    60,    15,
      -1,    37,    -1,    37,    16,    34,    17,    -1,    56,    -1,
      57,    12,    56,    -1,    69,    -1,     7,    -1,     6,    -1,
       5,    -1,    58,    -1,    61,    -1,    62,    12,    61,    -1,
      59,    37,    -1,    59,    37,    16,    17,    -1,    61,    -1,
      62,    12,    61,    -1,    69,    -1,    59,    57,    13,    63,
      -1,    69,    -1,    67,    -1,     1,    -1,    69,    -1,    54,
      -1,    69,    -1,    53,    66,    -1,    69,    -1,    37,    33,
      54,    -1,    37,    16,    54,    17,    33,    54,    -1,    65,
      -1,    68,    12,    54,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    91,    91,    92,    93,    95,   102,   107,   112,   118,
     124,   127,   129,   135,   142,   160,   161,   162,   163,   167,
     168,   169,   174,   175,   176,   177,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   204,   210,
     218,   223,   238,   239,   240,   242,   249,   257,   267,   268,
     269,   272,   277,   293,   294,   299,   302,   306,   308,   309,
     310,   313,   314,   317,   326,   329,   333,   339,   340,   342
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "WHILE", "FOR", "INT", "CHAR", "VOID",
  "IF", "ELSE", "EXTERN", "RETURN", "COMMA", "SEMICOLON", "LEFT_PAREN",
  "RIGHT_PAREN", "LEFT_SQUARE_BRACKET", "RIGHT_SQUARE_BRACKET",
  "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET", "EQ", "NEQ", "LTE", "LT",
  "GT", "GTE", "NOT", "AND", "OR", "ADD", "MINUS", "MUL", "DIV", "ASSIGN",
  "INTCON", "CHARCON", "STRINGCON", "ID", "TEXT", "SPACE", "WITHOUT_ELSE",
  "WITH_ELSE", "equality_op", "relop", "add_sub", "mul_div", "UMINUS",
  "$accept", "prog", "decl", "extern", "func_header", "func", "stmt",
  "expr", "name_args_lists", "var_decl", "var_decl_list", "void", "type",
  "param_types", "non_void_param_type", "param_types_list",
  "optional_var_decl_list", "optional_assign", "optional_expr",
  "stmt_list", "assg", "expr_list", "epsilon", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    48,    48,    49,    49,    49,    49,    49,
      49,    50,    51,    51,    52,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    55,    55,
      56,    56,    57,    57,    57,    58,    59,    59,    60,    60,
      60,    61,    61,    62,    62,    62,    63,    63,    64,    64,
      64,    65,    65,    66,    66,    67,    67,    68,    68,    69
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     3,     3,     3,     4,     4,
       2,     1,     5,     5,     5,     5,     7,     5,     9,     3,
       2,     5,     3,     1,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     4,     1,     3,     1,     1,     1,     1,     4,     6,
       1,     4,     1,     3,     1,     1,     1,     1,     1,     1,
       3,     2,     4,     1,     3,     1,     4,     1,     1,     1,
       1,     1,     1,     2,     1,     3,     6,     1,     3,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    57,    56,    55,    11,     0,     0,     0,     0,
       0,     0,    79,     4,    10,     1,     2,     0,     0,    79,
       3,     0,     0,    50,     0,    52,     0,    54,     0,     0,
       0,    79,     0,    67,    79,     0,     7,    79,     0,     6,
       0,     5,    79,     9,     8,    50,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,     0,     0,    74,    58,
       0,     0,    59,     0,    65,     0,     0,     0,    53,     0,
      79,    24,    25,     0,     0,     0,    47,     0,     0,     0,
      44,    45,    46,    42,    71,     0,    72,     0,     0,     0,
       0,    73,    14,    20,    61,    48,     0,    79,    48,    51,
      48,    66,     0,    69,     0,     0,    68,    70,     0,     0,
      27,    26,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,    22,    77,     0,
       0,    75,     0,    60,     0,     0,     0,     0,    43,     0,
       0,    34,    35,    37,    39,    38,    36,    30,    31,    28,
      29,    32,    33,     0,     0,     0,    62,    49,    17,     0,
      15,    40,    41,    78,    21,     0,     0,     0,    76,     0,
      16,     0,    18
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,     7,     8,     9,    10,    55,    84,    22,    25,
      26,    59,    60,    61,    62,    63,    32,   105,   128,    56,
      57,   129,    64
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -133
static const yytype_int16 yypact[] =
{
     296,     4,  -133,  -133,  -133,  -133,    21,   296,   141,    12,
     296,   -11,    30,  -133,  -133,  -133,  -133,    45,    45,    43,
    -133,    93,    49,    63,    59,  -133,   117,  -133,    97,   125,
     138,    76,    72,  -133,   141,   106,  -133,   141,   100,  -133,
      76,  -133,   141,  -133,  -133,   136,   149,    46,   144,   151,
     154,   105,  -133,    72,    27,    72,   162,   150,  -133,  -133,
     153,   158,   176,   179,  -133,   178,   185,   191,  -133,   194,
      43,  -133,  -133,   119,     1,   119,  -133,   119,   119,   119,
    -133,  -133,  -133,    73,   237,   197,  -133,   199,    10,   119,
     119,  -133,  -133,  -133,   195,   209,    43,   141,   216,  -133,
    -133,  -133,   155,  -133,    17,   213,  -133,  -133,   174,   192,
    -133,  -133,    10,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,  -133,  -133,  -133,    22,
     208,   237,   225,   231,   235,   156,   105,   156,  -133,    51,
     224,    92,    92,    96,    96,    96,    96,   263,   250,   140,
     140,  -133,  -133,   119,   265,   230,  -133,  -133,  -133,   276,
     267,  -133,  -133,   237,  -133,   119,     5,   156,   237,   275,
    -133,   156,  -133
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -133,    74,  -133,  -133,  -133,  -133,  -132,   -21,    11,   251,
     268,   300,     8,   -33,   202,  -133,   234,   139,   -50,    55,
     -61,   200,     0
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -80
static const yytype_int16 yytable[] =
{
      13,    85,   103,   158,    66,   160,   103,    13,    12,    69,
      13,    76,    27,   106,   -79,    12,    18,    14,    12,    33,
     -79,    15,   -79,    24,    77,   -79,    21,    31,    29,    30,
      19,    27,    58,    89,   153,   170,    78,   154,   104,   172,
      79,    88,   104,    89,    80,    81,    82,    83,     2,     3,
      90,    86,   102,    58,   108,    58,   109,   110,   111,    71,
      90,    35,    36,   153,   134,    72,   161,    23,   130,   131,
      33,    35,    39,    47,   107,    48,    49,    37,    31,    38,
      50,    16,    28,    51,    20,    52,   159,   112,    86,   113,
      53,   -79,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   106,    76,    34,    87,    54,
      91,    42,    86,    45,   116,   117,   118,   119,   -79,    77,
      76,   122,   123,   124,   125,   122,   123,   124,   125,    40,
      41,    78,   163,    77,    67,    79,    86,    35,    43,    80,
      81,    82,    83,    65,   168,    78,     2,     3,     4,    79,
      35,    44,    38,    80,    81,    82,    83,    47,    73,    48,
      49,    40,    70,    93,    50,    74,   107,    51,    75,    52,
     135,   124,   125,    95,    53,   114,   115,   116,   117,   118,
     119,    92,   120,   121,   122,   123,   124,   125,   -63,   137,
      94,    96,    97,    54,   114,   115,   116,   117,   118,   119,
      98,   120,   121,   122,   123,   124,   125,   138,    99,   100,
     126,   132,   114,   115,   116,   117,   118,   119,   127,   120,
     121,   122,   123,   124,   125,   155,   136,   -13,   114,   115,
     116,   117,   118,   119,   -12,   120,   121,   122,   123,   124,
     125,   162,   156,   -64,   114,   115,   116,   117,   118,   119,
     157,   120,   121,   122,   123,   124,   125,   114,   115,   116,
     117,   118,   119,   165,   120,   121,   122,   123,   124,   125,
     114,   115,   116,   117,   118,   119,   167,   120,   164,   122,
     123,   124,   125,   114,   115,   116,   117,   118,   119,   166,
     171,    68,   122,   123,   124,   125,   -79,     1,   133,    46,
      11,     2,     3,     4,   101,   169,     5,    11,    17,     0,
      11,     0,   139
};

static const yytype_int16 yycheck[] =
{
       0,    51,     1,   135,    37,   137,     1,     7,     0,    42,
      10,     1,    12,    74,    13,     7,     8,    13,    10,    19,
      15,     0,    12,    12,    14,    15,    37,    19,    17,    18,
      18,    31,    32,    16,    12,   167,    26,    15,    37,   171,
      30,    14,    37,    16,    34,    35,    36,    37,     5,     6,
      33,    51,    73,    53,    75,    55,    77,    78,    79,    13,
      33,    12,    13,    12,    97,    19,    15,    37,    89,    90,
      70,    12,    13,     1,    74,     3,     4,    14,    70,    16,
       8,     7,    37,    11,    10,    13,   136,    14,    88,    16,
      18,    19,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   166,     1,    14,    53,    37,
      55,    14,   112,    37,    22,    23,    24,    25,    13,    14,
       1,    29,    30,    31,    32,    29,    30,    31,    32,    12,
      13,    26,   153,    14,    34,    30,   136,    12,    13,    34,
      35,    36,    37,    37,   165,    26,     5,     6,     7,    30,
      12,    13,    16,    34,    35,    36,    37,     1,    14,     3,
       4,    12,    13,    13,     8,    14,   166,    11,    14,    13,
      15,    31,    32,    15,    18,    20,    21,    22,    23,    24,
      25,    19,    27,    28,    29,    30,    31,    32,    12,    15,
      37,    12,    14,    37,    20,    21,    22,    23,    24,    25,
      15,    27,    28,    29,    30,    31,    32,    15,    17,    15,
      13,    16,    20,    21,    22,    23,    24,    25,    19,    27,
      28,    29,    30,    31,    32,    17,    13,    18,    20,    21,
      22,    23,    24,    25,    18,    27,    28,    29,    30,    31,
      32,    17,    17,    12,    20,    21,    22,    23,    24,    25,
      15,    27,    28,    29,    30,    31,    32,    20,    21,    22,
      23,    24,    25,    33,    27,    28,    29,    30,    31,    32,
      20,    21,    22,    23,    24,    25,     9,    27,    13,    29,
      30,    31,    32,    20,    21,    22,    23,    24,    25,    13,
      15,    40,    29,    30,    31,    32,     0,     1,    96,    31,
       0,     5,     6,     7,    70,   166,    10,     7,     8,    -1,
      10,    -1,   112
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     5,     6,     7,    10,    48,    49,    50,    51,
      52,    58,    59,    69,    13,     0,    48,    58,    59,    18,
      48,    37,    55,    37,    55,    56,    57,    69,    37,    55,
      55,    59,    63,    69,    14,    12,    13,    14,    16,    13,
      12,    13,    14,    13,    13,    37,    57,     1,     3,     4,
       8,    11,    13,    18,    37,    53,    66,    67,    69,    58,
      59,    60,    61,    62,    69,    37,    60,    34,    56,    60,
      13,    13,    19,    14,    14,    14,     1,    14,    26,    30,
      34,    35,    36,    37,    54,    65,    69,    66,    14,    16,
      33,    66,    19,    13,    37,    15,    12,    14,    15,    17,
      15,    63,    54,     1,    37,    64,    67,    69,    54,    54,
      54,    54,    14,    16,    20,    21,    22,    23,    24,    25,
      27,    28,    29,    30,    31,    32,    13,    19,    65,    68,
      54,    54,    16,    61,    60,    15,    13,    15,    15,    68,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    12,    15,    17,    17,    15,    53,    65,
      53,    15,    17,    54,    13,    33,    13,     9,    54,    64,
      53,    15,    53
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

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
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 96 "cmm.y"
    {
            // TODO: Make this better
            globalScope = flattenScope(scope);
            scope = newScope(globalScope); // Clear out the scope the declaration was made in (ew)
            resetFunctionType();
        ;}
    break;

  case 6:
#line 103 "cmm.y"
    {
            scope = newScope(globalScope);
            resetFunctionType();
        ;}
    break;

  case 7:
#line 108 "cmm.y"
    {
            scope = newScope(globalScope);
            resetFunctionType();
        ;}
    break;

  case 8:
#line 113 "cmm.y"
    {
            scope = newScope(globalScope);
            declaredExtern = false;
            resetFunctionType();
        ;}
    break;

  case 9:
#line 119 "cmm.y"
    {
            scope = newScope(globalScope);
            declaredExtern = false;
            resetFunctionType();
        ;}
    break;

  case 11:
#line 127 "cmm.y"
    { declaredExtern = true; ;}
    break;

  case 12:
#line 130 "cmm.y"
    {
                    debug(E_DEBUG, "Declaring function %s with type %s\n", (yyvsp[(2) - (5)].string), typeName((yyvsp[(1) - (5)].type)));
                    addFunctionDeclarationToScope((yyvsp[(1) - (5)].type), (yyvsp[(2) - (5)].string), (yyvsp[(4) - (5)].parameter));
                    (yyval.string) = (yyvsp[(2) - (5)].string);
                ;}
    break;

  case 13:
#line 136 "cmm.y"
    {
                    debug(E_DEBUG, "Declaring function %s with type %s\n", (yyvsp[(2) - (5)].string), "VOID_TYPE");
                    addFunctionDeclarationToScope(VOID_TYPE, (yyvsp[(2) - (5)].string), (yyvsp[(4) - (5)].parameter));
                    (yyval.string) = (yyvsp[(2) - (5)].string);
                ;}
    break;

  case 14:
#line 143 "cmm.y"
    {
            char *identifier = (yyvsp[(1) - (5)].string);
            ScopeElement *elem = findScopeElement(globalScope, identifier);
            if(elem->elementType == SCOPE_FUNC) {
                ScopeFunction *func = elem->function;
                FunctionDeclaration *decl = newFunction(currentFunctionReturnType, identifier,
                                                        func->parameters, (yyvsp[(3) - (5)].variableDeclaration), (yyvsp[(4) - (5)].statement));
                scope = newScope(globalScope);

                (yyval.functionDeclaration) = decl;
            } else {
                (yyval.functionDeclaration) = NULL;
            }
            resetFunctionType();
        ;}
    break;

  case 15:
#line 160 "cmm.y"
    { (yyval.statement) = newIfStatement(scope, (yyvsp[(3) - (5)].expression), (yyvsp[(5) - (5)].statement)); ;}
    break;

  case 16:
#line 161 "cmm.y"
    { (yyval.statement) = newIfElseStatement(scope, (yyvsp[(3) - (7)].expression), (yyvsp[(5) - (7)].statement), (yyvsp[(7) - (7)].statement)); ;}
    break;

  case 17:
#line 162 "cmm.y"
    { (yyval.statement) = newWhileStatement(scope, (yyvsp[(3) - (5)].expression), (yyvsp[(5) - (5)].statement)); ;}
    break;

  case 18:
#line 164 "cmm.y"
    {
            (yyval.statement) = newForStatement(scope, (AssignmentStatement *) (yyvsp[(3) - (9)].statement), (yyvsp[(5) - (9)].expression), (AssignmentStatement *) (yyvsp[(7) - (9)].statement), (yyvsp[(9) - (9)].statement));
        ;}
    break;

  case 19:
#line 167 "cmm.y"
    { (yyval.statement) = newReturnStatement(scope, (yyvsp[(2) - (3)].expression)); ;}
    break;

  case 20:
#line 168 "cmm.y"
    { (yyval.statement) = (yyvsp[(1) - (2)].statement); ;}
    break;

  case 21:
#line 170 "cmm.y"
    {
            Expression *func = newFunctionExpression(scope, (yyvsp[(1) - (5)].string), (yyvsp[(3) - (5)].expression));
            (yyval.statement) = newFunctionCallStatement(scope, func);
        ;}
    break;

  case 22:
#line 174 "cmm.y"
    { (yyval.statement) = (yyvsp[(2) - (3)].statement); ;}
    break;

  case 23:
#line 175 "cmm.y"
    { (yyval.statement) = NULL; ;}
    break;

  case 24:
#line 176 "cmm.y"
    { (yyval.statement) = NULL; ;}
    break;

  case 25:
#line 177 "cmm.y"
    { (yyval.statement) = NULL; ;}
    break;

  case 26:
#line 180 "cmm.y"
    { (yyval.expression) = newUnaryExpression(NEG_OP, (yyvsp[(2) - (2)].expression)); ;}
    break;

  case 27:
#line 181 "cmm.y"
    { (yyval.expression) = newUnaryExpression(NOT_OP, (yyvsp[(2) - (2)].expression)); ;}
    break;

  case 28:
#line 182 "cmm.y"
    { (yyval.expression) = newBinaryExpression(ADD_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 29:
#line 183 "cmm.y"
    { (yyval.expression) = newBinaryExpression(SUB_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 30:
#line 184 "cmm.y"
    { (yyval.expression) = newBinaryExpression(AND_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 31:
#line 185 "cmm.y"
    { (yyval.expression) = newBinaryExpression(OR_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 32:
#line 186 "cmm.y"
    { (yyval.expression) = newBinaryExpression(MUL_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 33:
#line 187 "cmm.y"
    { (yyval.expression) = newBinaryExpression(DIV_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 34:
#line 188 "cmm.y"
    { (yyval.expression) = newBinaryExpression(EQ_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 35:
#line 189 "cmm.y"
    { (yyval.expression) = newBinaryExpression(NEQ_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 36:
#line 190 "cmm.y"
    { (yyval.expression) = newBinaryExpression(GTE_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 37:
#line 191 "cmm.y"
    { (yyval.expression) = newBinaryExpression(LTE_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 38:
#line 192 "cmm.y"
    { (yyval.expression) = newBinaryExpression(GT_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 39:
#line 193 "cmm.y"
    { (yyval.expression) = newBinaryExpression(LT_OP, (yyvsp[(1) - (3)].expression), (yyvsp[(3) - (3)].expression)); ;}
    break;

  case 40:
#line 194 "cmm.y"
    { (yyval.expression) = newFunctionExpression(scope, (yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].expression)); ;}
    break;

  case 41:
#line 195 "cmm.y"
    { (yyval.expression) = newVariableExpression(scope, (yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].expression)); ;}
    break;

  case 42:
#line 196 "cmm.y"
    { (yyval.expression) = newVariableExpression(scope, (yyvsp[(1) - (1)].string), NULL); ;}
    break;

  case 43:
#line 197 "cmm.y"
    { (yyval.expression) = (yyvsp[(2) - (3)].expression); ;}
    break;

  case 44:
#line 198 "cmm.y"
    { (yyval.expression) = newIntConstExpression(atoi(yytext)); ;}
    break;

  case 45:
#line 199 "cmm.y"
    { (yyval.expression) = newCharConstExpression(yytext[0]); ;}
    break;

  case 46:
#line 200 "cmm.y"
    { (yyval.expression) = newCharArrayConstExpression(strdup(yytext)); ;}
    break;

  case 47:
#line 201 "cmm.y"
    { (yyval.expression) = NULL; ;}
    break;

  case 48:
#line 205 "cmm.y"
    {
                        debug(E_DEBUG, "Declaring prototype for %s with type %s\n", (yyvsp[(1) - (4)].string), typeName(currentFunctionReturnType));
                        declareFunction(globalScope, currentFunctionReturnType, (yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].parameter), declaredExtern, true);
                        scope = newScope(globalScope);
                    ;}
    break;

  case 49:
#line 211 "cmm.y"
    {
                        debug(E_DEBUG, "Declaring prototype for %s with type %s\n", (yyvsp[(3) - (6)].string), typeName(currentFunctionReturnType));
                        declareFunction(globalScope, currentFunctionReturnType, (yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].parameter), declaredExtern, true);
                        scope = newScope(globalScope);
                    ;}
    break;

  case 50:
#line 219 "cmm.y"
    {
                declareVar(scope, baseDeclType, (yyvsp[(1) - (1)].string));
                (yyval.variableDeclaration) = newVariable(baseDeclType, (yyvsp[(1) - (1)].string));
            ;}
    break;

  case 51:
#line 224 "cmm.y"
    {
                if(baseDeclType == INT_TYPE) {
                    declareVar(scope, INT_ARRAY_TYPE, (yyvsp[(1) - (4)].string));
                    (yyval.variableDeclaration) = newVariable(INT_ARRAY_TYPE, (yyvsp[(1) - (4)].string));
                } else if(baseDeclType == CHAR_TYPE) {
                    declareVar(scope, CHAR_ARRAY_TYPE, (yyvsp[(1) - (4)].string));
                    (yyval.variableDeclaration) = newVariable(CHAR_ARRAY_TYPE, (yyvsp[(1) - (4)].string));
                } else {
                    fprintf(stderr, "ERROR: Cannot determine type when declaring %s on line %d!\n", (yyvsp[(1) - (4)].string), mylineno);
                    foundError = true;
                }
            ;}
    break;

  case 52:
#line 238 "cmm.y"
    { (yyval.variableDeclaration) = (yyvsp[(1) - (1)].variableDeclaration); ;}
    break;

  case 53:
#line 239 "cmm.y"
    { (yyvsp[(3) - (3)].variableDeclaration)->next = (yyvsp[(1) - (3)].variableDeclaration); (yyval.variableDeclaration) = (yyvsp[(3) - (3)].variableDeclaration); ;}
    break;

  case 54:
#line 240 "cmm.y"
    { (yyval.variableDeclaration) = NULL; ;}
    break;

  case 55:
#line 243 "cmm.y"
    {
            if(!funcTypeSet) {
                currentFunctionReturnType = VOID_TYPE;
                funcTypeSet = true;
            }
        ;}
    break;

  case 56:
#line 250 "cmm.y"
    {
            if(!funcTypeSet) {
                currentFunctionReturnType = CHAR_TYPE;
                funcTypeSet = true;
            }
            baseDeclType = CHAR_TYPE; (yyval.type) = CHAR_TYPE;
        ;}
    break;

  case 57:
#line 258 "cmm.y"
    {
            if(!funcTypeSet) {
                currentFunctionReturnType = INT_TYPE;
                funcTypeSet = true;
            }
            baseDeclType = INT_TYPE; (yyval.type) = INT_TYPE;
        ;}
    break;

  case 58:
#line 267 "cmm.y"
    { (yyval.parameter) = NULL; ;}
    break;

  case 59:
#line 268 "cmm.y"
    { (yyval.parameter) = (yyvsp[(1) - (1)].parameter); ;}
    break;

  case 60:
#line 269 "cmm.y"
    { (yyvsp[(3) - (3)].parameter)->next = (yyvsp[(1) - (3)].parameter); (yyval.parameter) = (yyvsp[(3) - (3)].parameter); ;}
    break;

  case 61:
#line 273 "cmm.y"
    {
                            declareVar(scope, baseDeclType, (yyvsp[(2) - (2)].string));
                            (yyval.parameter) = newFunctionParameter(baseDeclType, (yyvsp[(2) - (2)].string));
                        ;}
    break;

  case 62:
#line 278 "cmm.y"
    {
                            if(baseDeclType == INT_TYPE) {
                                declareVar(scope, INT_ARRAY_TYPE, (yyvsp[(2) - (4)].string));
                                (yyval.parameter) = newFunctionParameter(INT_ARRAY_TYPE, (yyvsp[(2) - (4)].string));
                            } else if(baseDeclType == CHAR_TYPE) {
                                declareVar(scope, CHAR_ARRAY_TYPE, (yyvsp[(2) - (4)].string));
                                (yyval.parameter) = newFunctionParameter(CHAR_ARRAY_TYPE, (yyvsp[(2) - (4)].string));
                            } else {
                                fprintf(stderr, "Type error, on line %d: Arrays of type %s are not supported.\n",
                                        mylineno, typeName(baseDeclType));
                                foundError = true;
                            }
                        ;}
    break;

  case 63:
#line 293 "cmm.y"
    { (yyval.parameter) = (yyvsp[(1) - (1)].parameter); ;}
    break;

  case 64:
#line 295 "cmm.y"
    {
                        (yyvsp[(3) - (3)].parameter)->next = (yyvsp[(1) - (3)].parameter);
                        (yyval.parameter) = (yyvsp[(3) - (3)].parameter);
                    ;}
    break;

  case 65:
#line 299 "cmm.y"
    { (yyval.parameter) = NULL; ;}
    break;

  case 66:
#line 303 "cmm.y"
    {
                            baseDeclType = (yyvsp[(1) - (4)].type); (yyvsp[(2) - (4)].variableDeclaration)->next = (yyvsp[(4) - (4)].variableDeclaration); (yyval.variableDeclaration) = (yyvsp[(2) - (4)].variableDeclaration);
                        ;}
    break;

  case 67:
#line 306 "cmm.y"
    { (yyval.variableDeclaration) = NULL; ;}
    break;

  case 68:
#line 308 "cmm.y"
    { (yyval.statement) = (yyvsp[(1) - (1)].statement); ;}
    break;

  case 69:
#line 309 "cmm.y"
    { (yyval.statement) = NULL; ;}
    break;

  case 70:
#line 310 "cmm.y"
    { (yyval.statement) = NULL; ;}
    break;

  case 71:
#line 313 "cmm.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 72:
#line 314 "cmm.y"
    { (yyval.expression) = NULL; ;}
    break;

  case 73:
#line 318 "cmm.y"
    {
                if((yyvsp[(1) - (2)].statement)) {
                    (yyvsp[(1) - (2)].statement)->next = (yyvsp[(2) - (2)].statement);
                    (yyval.statement) = (yyvsp[(1) - (2)].statement);
                } else {
                    (yyval.statement) = NULL;
                }
            ;}
    break;

  case 74:
#line 326 "cmm.y"
    { (yyval.statement) = NULL; ;}
    break;

  case 75:
#line 330 "cmm.y"
    {
            (yyval.statement) = newAssignmentStatement(scope, (yyvsp[(1) - (3)].string), NULL, (yyvsp[(3) - (3)].expression));
        ;}
    break;

  case 76:
#line 334 "cmm.y"
    {
            (yyval.statement) = newAssignmentStatement(scope, (yyvsp[(1) - (6)].string), (yyvsp[(3) - (6)].expression), (yyvsp[(6) - (6)].expression));
        ;}
    break;

  case 77:
#line 339 "cmm.y"
    { (yyval.expression) = (yyvsp[(1) - (1)].expression); ;}
    break;

  case 78:
#line 340 "cmm.y"
    { (yyvsp[(3) - (3)].expression)->next = (yyvsp[(1) - (3)].expression); (yyval.expression) = (yyvsp[(3) - (3)].expression); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2095 "cmm.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 345 "cmm.y"


bool addFunctionDeclarationToScope(Type type, char *identifier, FunctionParameter *parameters) {
    bool success = declareFunction(globalScope, type, identifier, parameters, declaredExtern, false);

    // Mark the function as implemented
    if(success) {
        ScopeElement *elem = findScopeElement(globalScope, identifier);
        ScopeFunction *func = elem->function;
        func->implemented = true;
    }

    return success;
}

void resetFunctionType() {
    funcTypeSet = false;
    currentFunctionReturnType = VOID_TYPE;
}

int main(int argc, char **argv){
#ifdef DEBUG
    setDebuggingLevel(E_ALL);
#endif
    globalScope = newScope(NULL);
    scope = newScope(globalScope);
    yyparse();

    if(foundError) {
        return 1;
    } else {
        return 0;
    }
}

int yyerror() {
    foundError = true;

    if(yytext[0] == 0) {
        fprintf(stderr, "Encountered unexpected EOF while parsing \"%s\" starting on line %d.\n",
                yytext, mylineno);
    } else {
        fprintf(stderr, "%d:%d - Encountered error while parsing: \"%s\"\n", mylineno, mycolno,
                yytext);
    }

    return 1;
}

