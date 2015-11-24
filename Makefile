CC = gcc
CFLAGS = -g
CFLAGS += -Wall
CFLAGS += -DYYDEBUG=1
CFLAGS += -std=gnu99
CFLAGS += -Iinclude

OS = $(shell uname -s)

# Load the proper library for Flex
ifeq ($(OS),Darwin)
	FLEX_LIB = -ll
endif

ifeq ($(OS),Linux)
	FLEX_LIB = -lfl
endif

files=utils.o vector.o ast.o symtab.o typecheck.o errors.o three.o codegen.o mips.o cmm.l cmm.y

compile: $(files)
	bison -o cmm.tab.c -d --debug --verbose cmm.y
	flex -o cmm.lex.c cmm.l
	${CC} ${CFLAGS} -o compile cmm.tab.c cmm.lex.c *.o ${FLEX_LIB}

debug: CFLAGS += -DDEBUG
debug: compile

%.o: %.c %.h
	${CC} ${CFLAGS} -c $^

todo:
	@grep -ni "TODO" *.c *.h *.y *.l

.PHONY : clean
clean:
	/bin/rm -f compile cmm.lex.c cmm.tab.c cmm.tab.h a.out *.o *.gch *.s
