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


myhtml2txt: myhtml2txt.l
	flex -o myhtml2txt.c myhtml2txt.l
	${CC} ${CFLAGS} -o myhtml2txt myhtml2txt.c ${FLEX_LIB}

clean:
	@rm *.o myhtml2txt 2> /dev/null || true
