CC = gcc
CFLAGS = -g
#CFLAGS += -Wall
CFLAGS += -w
CFLAGS += -DYYDEBUG=1
CFLAGS += -std=gnu99

OS = $(shell uname -s)

# Load the proper library for Flex
ifeq ($(OS),Darwin)
	FLEX_LIB = -ll
endif

ifeq ($(OS),Linux)
	FLEX_LIB = -lfl
endif

compile: cmm.l cmm.y utils.o list.o ast.o
	bison -d --debug --verbose cmm.y
	flex -o cmm.lex.c cmm.l
	${CC} ${CFLAGS} -o compile cmm.tab.c cmm.lex.c list.o ast.o utils.o ${FLEX_LIB}

debug: CFLAGS += -DDEBUG
debug: cmm.l cmm.y utils.o list.o ast.o
	bison -d --debug --verbose cmm.y
	flex -o cmm.lex.c cmm.l
	${CC} ${CFLAGS} -o compile cmm.tab.c cmm.lex.c list.o ast.o utils.o ${FLEX_LIB}

ast.o: ast.c ast.h
	${CC} ${CFLAGS} -c ast.c ast.h

utils.o: utils.c utils.h
	${CC} ${CFLAGS} -c utils.c utils.h

list.o: list.c list.h utils.o
	${CC} ${CFLAGS} -c list.c

.PHONY : clean
clean:
	/bin/rm -f compile cmm.lex.c cmm.tab.c cmm.tab.h a.out *.o *.gch
