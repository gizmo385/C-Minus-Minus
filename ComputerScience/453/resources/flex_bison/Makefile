CC = gcc
CFLAGS = -Wall -O2 -g

calculator : calculator.l calculator.y
	bison -d calculator.y
	flex -o calculator.lex.c calculator.l
	cc -o calculator calculator.tab.c calculator.lex.c

.PHONY : clean
clean:
	/bin/rm -f calculator calculator.lex.c calculator.tab.c calculator.tab.h a.out

