CC = gcc
CFLAGS = -Wall -O2 -g

calculator : calculator.l
	flex calculator.l
	cc lex.yy.c -lfl -o calculator

.PHONY : clean
clean:
	/bin/rm -f calculator lex.yy.c a.out

