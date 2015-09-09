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


myhtml2text: myhtml2text.l
	flex -o myhtml2text.c myhtml2text.l
	${CC} ${CFLAGS} -o myhtml2text myhtml2text.c ${FLEX_LIB}

.PHONY : clean
clean:
	@rm *.o myhtml2text myhtml2text.c 2> /dev/null || true
