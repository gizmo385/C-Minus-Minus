CC = gcc
CFLAGS = -g
CFLAGS += -Wall
CFLAGS += -std=gnu99

OS = $(shell uname -s)

# Load the proper library for Flex
ifeq ($(OS),Darwin)
	FLEX_LIB = -ll
endif

ifeq ($(OS),Linux)
	FLEX_LIB = -lfl
endif


compile: cmm.l cmm.y
	bison -d cmm.y
	flex -o cmm.lex.c cmm.l
	${CC} ${CFLAGS} -o compile cmm.tab.c cmm.lex.c ${FLEX_LIB}

.PHONY : clean
clean:
	/bin/rm -f compile cmm.lex.c cmm.tab.c cmm.tab.h a.out
