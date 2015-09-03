CC = gcc
CFLAGS = -g -Wall -std=gnu99 -ll

myhtml2text: lex.yy.c
	flex -o lex.c lex.yy.c
	${CC} ${CFLAGS} -o myhtml2text lex.c

clean:
	@rm *.o myhtml2text 2> /dev/null || true
