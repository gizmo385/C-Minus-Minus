/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
/* Line 1529 of yacc.c.  */
#line 151 "cmm.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

