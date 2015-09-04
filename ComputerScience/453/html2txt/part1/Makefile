CC = gcc
CFLAGS = -g -Wall -std=gnu99 -ll

myhtml2text: myhtml2text.l
	flex -o myhtml2text.c myhtml2text.l
	${CC} ${CFLAGS} -o myhtml2text myhtml2text.c

clean:
	@rm *.o myhtml2text 2> /dev/null || true
