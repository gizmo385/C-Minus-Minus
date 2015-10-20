CC = gcc
CFLAGS = -g
CFLAGS += -Wall
#CFLAGS += -w
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

compile: clean cmm.l cmm.y utils.o list.o ast.o symtab.o typecheck.o
	bison -d --debug --verbose cmm.y
	flex -o cmm.lex.c cmm.l
	${CC} ${CFLAGS} -o compile cmm.tab.c cmm.lex.c *.o ${FLEX_LIB}

debug: CFLAGS += -DDEBUG
debug: clean compile

%.o: %.c %.h
	${CC} ${CFLAGS} -c $^

todo:
	@grep -ni "TODO" *.c *.h

.PHONY : clean
clean:
	/bin/rm -f compile cmm.lex.c cmm.tab.c cmm.tab.h a.out *.o *.gch
